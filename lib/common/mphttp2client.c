#include <h2o/mphttp2client.h>

h2o_mphttp2client_t *
h2o_mpclient_create(char *host_address,
        h2o_httpclient_ctx_t *ctx,
        on_get_size_cb_t on_get_size_cb,
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

static void h2o_mpclient_update(h2o_mphttp2client_t *mp){
    h2o_mpclient_update_bw(mp);
    if(!mp->rangeclients.running && mp->rangeclients.pending){
        mp->rangeclients.running = mp->rangeclients.pending;
        mp->rangeclients.pending = NULL;
    }
}

size_t h2o_mpclient_get_bw(h2o_mphttp2client_t *mp){
    h2o_mpclient_update_bw(mp);
    if(mp->bandwith > 0)
        return mp->bandwith;
    return 128 * 1024; // guess! 128KB/S
}

int h2o_mpclient_fetch(h2o_mphttp2client_t *mp){

}