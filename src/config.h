#ifndef CONFIG_H
#define CONFIG_H

#define CONFIG_PATHS "/etc/ficus.conf"
#define DEFAULT_CONTENT_PATH "/var/www/html"
#define DEFAULT_PORT 9000

typedef struct server_configuration {
  int port;
  char* content_path;
} server_configuration;

void load_config(server_configuration *config);

void setup_server(int *server_socket, server_configuration *config);

#endif
