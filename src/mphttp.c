#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <string.h>
#include <h2o/httpclient.h>
#include <h2o/mphttp2client.h>
#include "picotls/openssl.h"
#include "h2o.h"

#define IO_TIMEOUT 10 * 1000 /* 10s */

static FILE *keylog_file_fp;

typedef void (*SSL_CTX_keylog_cb_func)(const SSL *ssl, const char *line);

static void ossl_keylog_callback(const SSL *ssl, const char *line){
  (void)ssl;

  /* Using fputs here instead of fprintf since libcurl's fprintf replacement
     may not be thread-safe. */
  if(keylog_file_fp && line && *line) {
    char stackbuf[256];
    char *buf;
    size_t linelen = strlen(line);

    if(linelen <= sizeof(stackbuf) - 2)
      buf = stackbuf;
    else {
      buf = malloc(linelen + 2);
      if(!buf)
        return;
    }
    memcpy(buf, line, linelen);
    buf[linelen] = '\n';
    buf[linelen + 1] = '\0';

    fputs(buf, keylog_file_fp);
    if(buf != stackbuf)
      free(buf);
  }
}

static void usage(const char *progname){
    fprintf(stderr,
            "Usage: %s <host#1> <host#2> <host#3> -t <path> -o <file>\n",
            progname);
}

static h2o_mphttp2client_t *interface[3];

h2o_mphttp2client_t *get_slowest_interface(h2o_mphttp2client_t *mp_idle){
    h2o_mphttp2client_t *rv = NULL;
    for(int i = 0; i < 3; i++)if(interface[i] != mp_idle && h2o_mpclient_get_remain(interface[i])>0) {
            if (rv == NULL || h2o_mpclient_get_remain_time(rv) < h2o_mpclient_get_remain_time(interface[i]))
                rv = interface[i];
        }
    return rv;
}

int on_get_size(){
    h2o_mpclient_reschedule(interface[1]);
    h2o_mpclient_reschedule(interface[2]);
    return 0;
}


int is_complete(){
    for(int i = 0; i < 3; i++){
        if(interface[i]->rangeclients.running || interface[i]->rangeclients.pending)
            return 0;
    }
    return 1;
}

int main(int argc, char **argv){
    int opt;
    char *save_to_file = NULL;
    char *path_of_url = NULL;

    char *cdns[3];
    int num_cdns = 0;

    /* If the first character of optstring is '-', then each nonoption argv-element is
     * handled as if it were the argument of an option with character code 1.
     */
    while((opt = getopt(argc, argv, "-o:t:"))!= -1){
        switch (opt){
            case 1:
                cdns[num_cdns++] = optarg;
                break;
            case 'o':
                save_to_file = optarg;
                break;
            case 't':
                path_of_url = optarg;
                break;
            default:
                usage(argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    if(num_cdns!=3 || !save_to_file || !path_of_url){
        usage(argv[0]);
        exit(EXIT_FAILURE);
    }

    /* setup SSL */
    SSL_load_error_strings();
    SSL_library_init();
    OpenSSL_add_all_algorithms();
    keylog_file_fp = fopen("./ssl_keylog", "w");

    /* setup context */
    h2o_multithread_queue_t *queue;
    h2o_multithread_receiver_t getaddr_receiver;
    h2o_httpclient_ctx_t ctx = {
        NULL, /* loop */
        &getaddr_receiver,
        IO_TIMEOUT,                              /* io_timeout */
        IO_TIMEOUT,                              /* connect_timeout */
        IO_TIMEOUT,                              /* first_byte_timeout */
        NULL,                                    /* websocket_timeout */
        IO_TIMEOUT,                              /* keepalive_timeout */
        H2O_SOCKET_INITIAL_INPUT_BUFFER_SIZE * 2 /* max_buffer_size */
    };
    ctx.http2.ratio = 100;
    ctx.loop = h2o_evloop_create();
    queue = h2o_multithread_create_queue(ctx.loop);
    h2o_multithread_register_receiver(queue, ctx.getaddr_receiver, h2o_hostinfo_getaddr_receiver);

    for(int i = 0; i < 3; i++)
        interface[i] = h2o_mpclient_create(cdns[i], &ctx, on_get_size,
                get_slowest_interface, /* ssl_verify_none */1);

    h2o_mpclient_fetch(interface[0], path_of_url, save_to_file, 0, 0);

    while(!is_complete()){
        h2o_evloop_run(ctx.loop, 1000);
    }

    fclose(keylog_file_fp);

    return 0;
}