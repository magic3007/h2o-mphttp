#ifndef H2O_RANGECLIENT_H
#define H2O_RANGECLIENT_H

#ifdef __cplusplus
extern "C" {
#endif

#define DEFAULT_RANGECLIENT_BUF_SIZE 256

#include "h2o/httpclient.h"
typedef struct st_h2o_rangeclient_t h2o_rangeclient_t;
typedef struct st_bandwidth_sampler_t bandwidth_sample_t;

typedef int (*on_get_size_cb_t)();
typedef int (*on_complete_cb_t)(h2o_rangeclient_t *);
typedef int (*on_almost_complete_cb_t)(h2o_rangeclient_t *);
typedef void (*on_buffer_consume_cb_t)(void*, size_t, size_t);

struct st_bandwidth_sampler_t{
    uint64_t last_receive_time;
    size_t last_receive;
    uint64_t bw; // including rtt
};

struct st_h2o_rangeclient_t{
    h2o_httpclient_t *httpclient;

    h2o_mem_pool_t *pool;
    h2o_httpclient_connection_pool_t  *connpool;
    h2o_httpclient_ctx_t *ctx;
    void *data;
    h2o_url_t *url_parsed;
    char *save_to_file;
    int mpclientID;

    struct{
        size_t begin;
        size_t end;
        size_t received;
    }range;

    int fd;
    char *buf;
    h2o_header_t *range_header;
    h2o_timer_t cancel_timer;
    bandwidth_sample_t bw_sampler;

    int is_closed;
    int is_almost_complete;

    struct{
        on_complete_cb_t on_complete;
        on_almost_complete_cb_t on_almost_complete;
        on_get_size_cb_t on_get_size;
        on_buffer_consume_cb_t on_buffer_consume;
    }cb;
};

h2o_rangeclient_t *h2o_rangeclient_create(
        h2o_httpclient_connection_pool_t *connpool,
        void *data,
        h2o_httpclient_ctx_t *ctx,
        char *save_to_file,
        h2o_url_t *target,
        size_t bytes_begin,
        size_t bytes_end,
        on_complete_cb_t on_complete,
        on_almost_complete_cb_t on_almost_complete,
        on_get_size_cb_t on_get_size,
        on_buffer_consume_cb_t on_buffer_consume,
        int mpclientID
        );
void h2o_rangeclient_destroy(h2o_rangeclient_t *ra);

void h2o_rangeclient_adjust_range_end(h2o_rangeclient_t *ra, size_t end);

size_t h2o_rangeclient_get_bw(h2o_rangeclient_t *ra); // Bytes/s
uint64_t h2o_rangeclient_get_rtt(h2o_rangeclient_t *ra); // us
size_t h2o_rangeclient_get_remain(h2o_rangeclient_t *ra); // Bytes
uint64_t h2o_rangeclient_get_remaining_time(h2o_rangeclient_t *ra); // us

#ifdef __cplusplus
}
#endif
#endif //H2O_RANGECLIENT_H
