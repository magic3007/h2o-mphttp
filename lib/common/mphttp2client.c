#include <h2o/mphttp2client.h>



h2o_mphttp2client_t *
h2o_mpclient_create(char *host_address,
        h2o_httpclient_ctx_t *ctx,
        on_get_size_cb_t on_get_size_cb,
        on_reschedule_cb_t on_reschedule_cb,
        int ssl_verify_none){

    // allocate memory on stack
    char request_url[512];
    h2o_url_t url_parsed;
    snprintf(request_url, 512,"%s%s", "https://", host_address);

    if(h2o_url_parse(request_url, SIZE_MAX, &url_parsed) != 0 )
        h2o_fatal("unrecognized type of URL: %s", request_url);

    h2o_mphttp2client_t * mp = h2o_mem_alloc(sizeof(h2o_mphttp2client_t));

    mp->pool = h2o_mem_alloc(sizeof(h2o_mem_pool_t));
    h2o_mem_init_pool(mp->pool);

    mp->rangeclients.running = NULL;
    mp->rangeclients.pending = NULL;

    mp->bandwith = 0;
    mp->ctx = ctx;

    mp->url_len =  strlen(request_url);
    mp->request_url = h2o_mem_alloc_pool(mp->pool, char, mp->url_len + 1);
    memcpy(mp->request_url, request_url, mp->url_len);

    mp->connpool = h2o_mem_alloc(sizeof(h2o_httpclient_connection_pool_t));

    mp->on_get_size_cb = on_get_size_cb;
    mp->on_reschedule_cb = on_reschedule_cb;

    h2o_socketpool_t *sockpool = h2o_mem_alloc(sizeof(*sockpool));
    h2o_socketpool_target_t *target = h2o_socketpool_create_target(&url_parsed, NULL);
    h2o_socketpool_init_specific(sockpool, 10, &target, 1, NULL);
    h2o_socketpool_set_timeout(sockpool, ctx->io_timeout);
    h2o_socketpool_register_loop(sockpool, ctx->loop);
    h2o_httpclient_connection_pool_init(mp->connpool, sockpool);

    /* obtain root */
    char *root, *crt_fullpath;
    if ((root = getenv("H2O_ROOT")) == NULL)
        root = H2O_TO_STR(H2O_ROOT);
    #define CA_PATH "/share/h2o/ca-bundle.crt"
    crt_fullpath = h2o_mem_alloc(strlen(root) + strlen(CA_PATH) + 1);
    sprintf(crt_fullpath, "%s%s", root, CA_PATH);
    #undef CA_PATH
    SSL_CTX *ssl_ctx = SSL_CTX_new(TLSv1_client_method());
    SSL_CTX_load_verify_locations(ssl_ctx, crt_fullpath, NULL);
    if (ssl_verify_none) {
        SSL_CTX_set_verify(ssl_ctx, SSL_VERIFY_NONE, NULL);
    } else {
        SSL_CTX_set_verify(ssl_ctx, SSL_VERIFY_PEER | SSL_VERIFY_FAIL_IF_NO_PEER_CERT, NULL);
    }
    h2o_socketpool_set_ssl_ctx(sockpool, ssl_ctx);
    SSL_CTX_free(ssl_ctx);

    return mp;
}

static void h2o_mpclient_update_bw(h2o_mphttp2client_t *mp){
    if(mp->rangeclients.running){
        size_t rv = h2o_rangeclient_get_bw(mp->rangeclients.running);
        if(rv > 0) mp->bandwith = rv;
    }else if (mp->rangeclients.pending){
        size_t rv = h2o_rangeclient_get_bw(mp->rangeclients.pending);
        if(rv > 0) mp->bandwith = rv;
    }
}

static void h2o_mpclient_update_running(h2o_mphttp2client_t *mp){
    if(!mp->rangeclients.running && mp->rangeclients.pending){
        mp->rangeclients.running = mp->rangeclients.pending;
        mp->rangeclients.pending = NULL;
    }
}

static void h2o_mpclient_update(h2o_mphttp2client_t *mp){
    h2o_mpclient_update_bw(mp);
    h2o_mpclient_update_running(mp);
}

size_t h2o_mpclient_get_bw(h2o_mphttp2client_t *mp){
    h2o_mpclient_update_bw(mp);
    if(mp->bandwith > 0)
        return mp->bandwith;
    return 128 * 1024; // guess! 128KB/S
}

uint64_t h2o_mpclient_get_rtt(h2o_mphttp2client_t *mp){
    h2o_mpclient_update_running(mp);
    if(!mp->rangeclients.running)
        return 0;
    else
        return h2o_rangeclient_get_rtt(mp->rangeclients.running);
}

uint32_t h2o_mpclient_get_remain(h2o_mphttp2client_t *mp){
    h2o_mpclient_update_running(mp);
    if(!mp->rangeclients.running)
        return 0;
    else
        return h2o_rangeclient_get_remain(mp->rangeclients.running);
}

uint32_t h2o_mpclient_get_remain_time(h2o_mphttp2client_t *mp){
    uint32_t remain = h2o_mpclient_get_remain(mp); // Byte
    size_t bw = h2o_mpclient_get_bw(mp); // B/s
    return remain * 1000 / bw; // ms
}


static int on_complete(h2o_rangeclient_t *ra){
    h2o_mphttp2client_t *mp = (h2o_mphttp2client_t*)ra->data;
    assert(ra->is_closed);
    if (ra == mp->rangeclients.running) {
        mp->rangeclients.running = NULL;
    } else if (ra == mp->rangeclients.pending) {
        mp->rangeclients.pending = NULL;
    }
    h2o_rangeclient_destroy(ra);
    h2o_mpclient_update(mp);
    h2o_mpclient_reschedule(mp);
    return 0;
}

int h2o_mpclient_fetch(h2o_mphttp2client_t *mp, char *request_path,
        char *save_to_file, size_t bytes_begin, size_t bytes_end){
    h2o_url_t url_parsed;

    char buf[256];
    sprintf(buf, "%s%s", mp->request_url, request_path);
    h2o_url_parse(buf, sizeof(buf), &url_parsed);

    h2o_mpclient_update(mp);
    if (mp->rangeclients.running == NULL) {
    mp->rangeclients.running = h2o_rangeclient_create(mp->connpool, mp,
                                                     mp->ctx, save_to_file,
                                                     &url_parsed,
                                                     bytes_begin , bytes_end);
    mp->rangeclients.running->cb.on_complete = on_complete;
    mp->rangeclients.running->cb.on_get_size = mp->on_get_size_cb;
    return 0;
    }
    return -1;
}

int h2o_mpclient_reschedule(h2o_mphttp2client_t *mp_idle){
    h2o_mphttp2client_t *mp_busy = mp_idle->on_reschedule_cb(mp_idle);
    if(mp_busy == NULL) return 0;
    assert(mp_busy != mp_idle);

    h2o_rangeclient_t *ra_busy = mp_busy->rangeclients.running;
    h2o_rangeclient_t *ra_idle = mp_idle->rangeclients.pending;

    assert(ra_idle == NULL);

    if(h2o_rangeclient_get_remaining_time(ra_busy) < 10 /* ms */)
        return 0;

    // file size is unknown
    if(ra_busy->range.end == 0)
        return 0;


    size_t bw_idle = h2o_mpclient_get_bw(mp_idle);
    size_t bw_busy = h2o_mpclient_get_bw(mp_busy);

    size_t remain = h2o_rangeclient_get_remain(ra_busy);

    if (remain < 1024 * 64 /* 64KB */) {
        return 0;
    }

    size_t idle_len = (uint64_t) remain * bw_idle / (bw_idle + bw_busy);
    size_t idle_end = ra_busy->range.end;
    size_t idle_begin = idle_end - idle_len;
    h2o_rangeclient_adjust_range_end(ra_busy, idle_begin);

    mp_idle->rangeclients.running = h2o_rangeclient_create(
            mp_idle->connpool, mp_idle, mp_idle->ctx, ra_busy->save_to_file, ra_busy->url_parsed,
            idle_begin, idle_end
            );
    mp_idle->rangeclients.pending->cb.on_complete = on_complete;

    h2o_mpclient_update(mp_idle);
    return 0;
}
