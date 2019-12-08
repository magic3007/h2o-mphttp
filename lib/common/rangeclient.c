#include <h2o/rangeclient.h>
#include <fcntl.h>
#include <zconf.h>

/**
 * See the syntax of HTTP |range| header at
 * https://developer.mozilla.org/en-US/docs/Web/HTTP/Headers/Range
 */
static h2o_iovec_t range_str=(h2o_iovec_t){H2O_STRLIT("range")};

size_t h2o_rangeclient_get_bw(h2o_rangeclient_t *ra){
   return ra->bw_sampler.bw;
}

uint64_t h2o_rangeclient_get_rtt(h2o_rangeclient_t *ra){
    return h2o_httpclient_get_ping_rtt(ra->httpclient);
}

size_t h2o_rangeclient_get_remain(h2o_rangeclient_t *ra){
    return ra->range.end == 0 ? 0
        : ra->range.end - ra->range.begin - ra->range.received;
}

uint32_t h2o_rangeclient_get_remaining_time(h2o_rangeclient_t *ra){
    uint64_t bw = h2o_rangeclient_get_bw(ra); // Bytes/s
    if(bw == 0)
        return UINT32_MAX;
    size_t remain = h2o_rangeclient_get_remain(ra); // Byte
    return (uint64_t)remain * 1000 / bw;
}

void h2o_rangeclient_adjust_range_end(h2o_rangeclient_t *ra, size_t end) {
  assert(end > ra->range.begin);
  if (end < ra->range.begin + ra->range.received) {
    h2o_error_printf("warning: overwrite existing content. cancel the stream now\n");
    h2o_timer_link(ra->ctx->loop, 0, &ra->cancel_timer);
  } else {
      ra->range.end = end;
  }
}

static void print_status_line(int version, int status, h2o_iovec_t msg){
    fprintf(stderr, "HTTP/%d", (version >> 8));
    if ((version & 0xff) != 0) {
        fprintf(stderr, ".%d", version & 0xff);
    }
    fprintf(stderr, " %d", status);
    if (msg.len != 0) {
        fprintf(stderr, " %.*s\n", (int)msg.len, msg.base);
    } else {
        fprintf(stderr, "\n");
    }
}


static int writen(int fd, void *ptr, size_t n){
    size_t cur = 0;
    int rv;
    while((rv = write(fd, ptr + cur, n - cur))){
        if(rv < 0) return rv;
        cur += rv;
    }
    assert(cur == n);
    return cur;
}

static void h2o_rangeclient_buffer_consume(h2o_rangeclient_t *ra, h2o_buffer_t *inbuf){
    size_t remain = h2o_rangeclient_get_remain(ra);
    size_t consume = inbuf->size;
    int cancel = consume >= remain;
    if(cancel) consume = remain;
    if(writen(ra->fd, inbuf->bytes, consume)<0)
        h2o_fatal("writen error: %s", strerror(errno));
    ra->range.begin += consume;
    if(cancel && !h2o_timer_is_linked(&ra->cancel_timer))
        h2o_timer_link(ra->ctx->loop, 0, &ra->cancel_timer);
}


static void h2o_rangeclient_bandwidth_update(bandwidth_sample_t *bw_sampler, size_t received, uint32_t now_ms){
    if(bw_sampler->last_receive_time == 0){
        bw_sampler->last_receive_time = now_ms;
        bw_sampler->last_receive = received;
        return;
    }
    uint64_t time_interval = now_ms - bw_sampler->last_receive_time;
    size_t nbytes = received - bw_sampler->last_receive;

    if(time_interval == 0)
        return;

    uint64_t sample = nbytes * 1000 / time_interval; // Byte/s
    if(bw_sampler->bw == 0)
        bw_sampler->bw = sample;
    else
        bw_sampler->bw = 0.9 * bw_sampler->bw + 0.1 * sample;

    bw_sampler->last_receive_time = now_ms;
    bw_sampler->last_receive = received;
}

static void cancel_stream_cb(h2o_timer_t *timer) {
  h2o_rangeclient_t *client = H2O_STRUCT_FROM_MEMBER(h2o_rangeclient_t, cancel_timer, timer);
  client->httpclient->cancel(client->httpclient);
  close(client->fd);
  client->is_closed = 1;
  client->cb.on_complete(client);
}

static int on_body(h2o_httpclient_t *httpclient, const char *errstr){

    if (errstr != NULL && errstr != h2o_httpclient_error_is_eos) {
        h2o_error_printf("Eroor in on_body: %s\n", errstr);
        return -1;
    }

    h2o_rangeclient_t *ra = httpclient->data;
    h2o_rangeclient_buffer_consume(ra, *httpclient->buf);
    h2o_rangeclient_bandwidth_update(&ra->bw_sampler, (*httpclient->buf)->size, /* ms */h2o_now(ra->ctx->loop));
    h2o_buffer_consume(&(*httpclient->buf), (*httpclient->buf)->size);

    if (errstr == h2o_httpclient_error_is_eos) {
        printf("done!\n");
        close(ra->fd);
        ra->is_closed = 1;
        ra->cb.on_complete(ra);
  }
    return 0;
}

static int parse_range_header(h2o_header_t *headers, size_t num_headers, size_t *file_size){
    /**
     * See the syntax of HTTP |content-Range| header at
     * https://developer.mozilla.org/en-US/docs/Web/HTTP/Headers/Content-Range
     */
    static h2o_iovec_t content_range_str=(h2o_iovec_t){H2O_STRLIT("Content-Range")};

    for(size_t i = 0; i != num_headers; i++){
        const char *name = headers[i].orig_name;
        size_t name_len = headers[i].name->len;
        if(name == NULL)
            name = headers[i].name->base;
        if(h2o_memis(name, name_len, content_range_str.base, content_range_str.len)){
            if(sscanf(headers[i].value.base, "bytes %*zu-%*zu/%zu", file_size) < 1){
                h2o_error_printf("Can't read the file size in Content-Range header!");
                return -1;
            }
            return 0;
        }
    }
    h2o_error_printf("Can't find Content-Range header!");
    return -1;
}

static h2o_httpclient_body_cb
on_head(h2o_httpclient_t *htttpclient, const char *errstr, int version, int status, h2o_iovec_t msg,
        h2o_header_t *headers, size_t num_headers, int header_requires_dup){
    if (errstr != NULL && errstr != h2o_httpclient_error_is_eos) {
        h2o_error_printf("|On head error: %s\n",errstr);
        return NULL;
    }
    print_status_line(version, status, msg);
    h2o_rangeclient_t *ra = htttpclient->data;

     if(ra->range.end==0){
         size_t file_size;
         if(parse_range_header(headers, num_headers, &file_size)==0){
             ra->range.end = file_size;
             if(ra->cb.on_get_size!=NULL)
                 ra->cb.on_get_size();
         }
    }

    if (errstr == h2o_httpclient_error_is_eos) {
        h2o_error_printf("On head error: no body");
        return NULL;
    }

    return on_body;
}

static h2o_httpclient_head_cb
on_connect(h2o_httpclient_t *httpclient, const char *errstr, h2o_iovec_t *_method, h2o_url_t *url,
        const h2o_header_t **headers, size_t *num_headers, h2o_iovec_t *body,
        h2o_httpclient_proceed_req_cb *proceed_req_cb, h2o_httpclient_properties_t *props,
        h2o_url_t *origin){

    if(errstr != NULL){
        h2o_error_printf("On connect error: %s", errstr);
        return NULL;
    }

    h2o_rangeclient_t *ra = (h2o_rangeclient_t*)httpclient->data;

    *_method = h2o_iovec_init(H2O_STRLIT("GET"));
    *url = *ra->url_parsed;
    *body = h2o_iovec_init(NULL, 0);
    *proceed_req_cb = NULL;


    size_t buf_len;
    if(ra->range.end > 0){
        buf_len = snprintf(ra->buf, 256, "bytes=%zu-%zu",
                ra->range.begin, ra->range.end-1);
    }else{
        buf_len = snprintf(ra->buf, 256, "bytes=%zu-",
                ra->range.begin);
    }

    ra->range_header->name = &range_str;
    ra->range_header->value = h2o_iovec_init(ra->buf, buf_len);

    *headers = ra->range_header;
    *num_headers = 1;

    return on_head;
}

h2o_rangeclient_t *
h2o_rangeclient_create(
        h2o_httpclient_connection_pool_t *connpool,
        void *data,
        h2o_httpclient_ctx_t *ctx,
        char *save_to_file,
        h2o_url_t *target,
        size_t bytes_begin,
        size_t bytes_end
        ){
    h2o_rangeclient_t *ra = h2o_mem_alloc(sizeof(h2o_rangeclient_t));

    ra->pool = h2o_mem_alloc(sizeof(struct st_h2o_mem_pool_t));
    h2o_mem_init_pool(ra->pool);

    ra->ctx = ctx;
    ra->data = data;

    ra->save_to_file = h2o_mem_alloc(strlen(save_to_file) + 1);
    memcpy(ra->save_to_file, save_to_file, strlen(save_to_file));

    ra->range.begin = bytes_begin;
    ra->range.end = bytes_end;
    ra->range.received = 0;

    if(bytes_end > 0)
        assert(bytes_begin < bytes_end);

    ra->range_header = h2o_mem_alloc_pool(ra->pool, h2o_header_t, 1);

    ra->url_parsed = h2o_mem_alloc_pool(ra->pool, h2o_url_t, 1);
    h2o_url_copy(ra->pool, ra->url_parsed, target);


    ra->connpool = connpool;

    ra->fd = open(save_to_file,
            /* oflag */ O_WRONLY | O_TRUNC,
            /* mode */ S_IRWXU | S_IRWXG | S_IRWXO);
    if(ra->fd < 0){
        h2o_fatal("fail to open %s: %s\n", save_to_file, strerror(errno));
    }
    if(lseek(ra->fd, bytes_begin, SEEK_SET) < 0){
        h2o_fatal("fd(%d) fail to lseek %s:\n", ra->fd, save_to_file);
    }

    ra->buf = h2o_mem_alloc_pool(ra->pool, char, DEFAULT_RANGECLIENT_BUF_SIZE);

    h2o_timer_init(&ra->cancel_timer, cancel_stream_cb);

    ra->is_closed = 0;
    h2o_httpclient_connect(&ra->httpclient, ra->pool, ra, ctx, connpool, target, on_connect);
    return ra;
}

void h2o_rangeclient_destroy(h2o_rangeclient_t *ra) {
  h2o_mem_clear_pool(ra->pool);
  free(ra->pool);
  free(ra);
}