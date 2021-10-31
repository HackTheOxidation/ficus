#ifndef CONFIG_H
#define CONFIG_H

#include <tls.h>

#define CONFIG_PATH "/etc/ficus.conf"

#define DEFAULT_CONTENT_PATH "/var/www/html"
#define DEFAULT_PORT 9000
#define DEFAULT_INDEX "index.html"
#define DEFAULT_ROOT_CA "server.crt"
#define DEFAULT_TLS_CERT "server.crt"
#define DEFAULT_TLS_KEY "server.key"

// Any configurable identifier
#define IDENTIFIER_CONTENT_PATH "content_path"
#define IDENTIFIER_PORT "port"
#define IDENTIFIER_INDEX "index"
#define IDENTIFIER_ROOT_CA "root_CA"
#define IDENTIFIER_TLS_CERT "tls_cert"
#define IDENTIFIER_TLS_KEY "tls_key"

typedef struct server_configuration {
  int port;
  char *content_path;
  char *index;
  struct tls_config *tls_cfg;
  struct tls *tls_ctx;
  struct tls *tls_cctx;
  char *root_CA;
  char *tls_cert;
  char *tls_key;
  int use_tls;
} server_configuration;

server_configuration *create_def_server_cfg();

void free_server_config(server_configuration *config);

void load_config(server_configuration *config);

void setup_server(int *server_socket, server_configuration *config);

#endif
