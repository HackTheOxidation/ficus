/*  This file is part of Ficus.

    Ficus is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Ficus is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Ficus.  If not, see <https://www.gnu.org/licenses/>.
*/
#include <errno.h>
#include <err.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Header files needed for socket programming
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <tls.h>

#include "ficus.h"
#include "handler.h"
#include "config.h"
#include "security.h"

int server_socket;
server_configuration *config;

void on_close_interrupt(int sig) {
  printf("\nStopping server. Closing server_socket...\n");
  shutdown(server_socket, SHUT_RDWR);
  close(server_socket);
  printf("Socket closed.\n");
  if (config->use_tls) {
    tls_free(config->tls_cctx);
    tls_free(config->tls_ctx);
    tls_config_free(config->tls_cfg);
    printf("TLS configuration freed.\n");
  }
  free_server_config(config);
  printf("Ficus has been terminated.\n");
  exit(EXIT_SUCCESS);
}

int main() {
  signal(SIGTSTP, on_close_interrupt);

  config = create_def_server_cfg();
  load_config(config);

  if (init_security(config) == 0) {
    config->use_tls=1;
    printf("TLS configured properly. Connections will be secure.\n");
  } else {
    printf("Connections will be INSECURE.\n");
  }
  // Create a socket
  setup_server(&server_socket, config);

  int err = listen(server_socket, SERVER_BACKLOG);
  if (err < 0) {
    perror("\nERROR: Could not start listening.\n");
    exit(EXIT_FAILURE);
  }
  printf("Server started.\nNOTE: Use <CTRL-Z> to stop the server.\nListening "
         "for connections...\n\n");

  // Define and accept the client. Then respond with the http_header
  int client_socket;
  while (1) {
    if (accept_and_shake_hands(config, &server_socket, &client_socket) == -1)
      continue;

    handle_request(client_socket, config);

    if (config->use_tls)
      tls_close(config->tls_cctx);
    close(client_socket);
  }

  if (config->use_tls) {
    tls_free(config->tls_cctx);
    tls_free(config->tls_ctx);
    tls_config_free(config->tls_cfg);
  }
  free_server_config(config);

  return 0;
}
