#ifndef H2O_RANGECLIENT_H
#define H2O_RANGECLIENT_H

//#ifdef __cplusplus
//extern "C" {
//#endif

#define DEFAULT_RANGECLIENT_BUF_SIZE 256

#include "h2o/httpclient.h"
typedef struct st_h2o_rangeclient_t h2o_rangeclient_t;

typedef int (*on_get_size_cb_t)();

struct st_h2o_rangeclient_t{
    h2o_httpclient_t *httpclient;

    h2o_mem_pool_t *pool;
    h2o_httpclient_connection_pool_t  *connpool;
    h2o_httpclient_ctx_t *ctx;
    void *data;
    h2o_url_t *url_parsed;

    struct{
        size_t begin;
        size_t end;
        size_t received;
    }range;

    int fd;
    char *buf;
    h2o_header_t *range_header;

    struct{
        on_get_size_cb_t on_get_size;
    }cb;
};

h2o_rangeclient_t *h2o_rangeclient_create(
        h2o_httpclient_connection_pool_t *connpool,
        void *data,
        h2o_httpclient_ctx_t *ctx,
        char *save_to_file,
        h2o_url_t *target,
        size_t bytes_begin,
        size_t bytes_end
        );

size_t h2o_rangeclient_get_bw(h2o_rangeclient_t *ra);


//#ifdef __cplusplus
//}
//#endif
#endif //H2O_RANGECLIENT_H
