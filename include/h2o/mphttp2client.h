#ifndef H2O_MPHTTP2CLIENT_H
#define H2O_MPHTTP2CLIENT_H

//#ifdef __cplusplus
//extern "C"
//{
//#endif

#include "h2o/httpclient.h"
#include "h2o/rangeclient.h"

typedef struct st_h2o_mphttp2client_t h2o_mphttp2client_t;



struct st_h2o_mphttp2client_t{
    h2o_mem_pool_t *pool;
    h2o_httpclient_ctx_t *ctx;
    h2o_httpclient_connection_pool_t *connpool;
    size_t url_len;
    char *request_url;

    struct{
        h2o_rangeclient_t *running;
        h2o_rangeclient_t *pending;
    }rangeclients;

    size_t bandwith;

    on_get_size_cb_t  on_get_size_cb;
};

h2o_mphttp2client_t *
h2o_mpclient_create(char *host_address,
        h2o_httpclient_ctx_t *ctx,
        on_get_size_cb_t on_get_size_cb,
        int ssl_verify_none);

size_t
h2o_mpclient_get_bw(h2o_mphttp2client_t *mp);

int h2o_mpclient_fetch(h2o_mphttp2client_t *mp);

//#ifdef __cplusplus
//}
//#endif
#endif //H2O_MPHTTP2CLIENT_H
