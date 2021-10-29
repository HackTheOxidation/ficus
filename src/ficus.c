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

#include "ficus.h"
#include "handler.h"
#include "config.h"

int server_socket;

void on_close_interrupt(int sig) {
  printf("\nStopping server. Closing server_socket...\n");
  shutdown(server_socket, SHUT_RDWR);
  close(server_socket);
  printf("Socket closed.\n");
  exit(EXIT_SUCCESS);
}

int main() {
  signal(SIGTSTP, on_close_interrupt);

  server_configuration config = { DEFAULT_PORT, DEFAULT_CONTENT_PATH };
  load_config(&config);

  // Create a socket
  setup_server(&server_socket, &config);

  int err;
  err = listen(server_socket, SERVER_BACKLOG);
  if (err < 0) {
    perror("\nERROR: Could not start listening.\n");
    exit(EXIT_FAILURE);
  }
  printf("Server started.\nNOTE: Use <CTRL-Z> to stop the server.\nListening "
         "for connections...\n\n");

  // Define and accept the client. Then respond with the http_header
  int client_socket;
  while (1) {
    client_socket = accept(server_socket, NULL, NULL);
    handle_request(client_socket, &config);

    close(client_socket);
  }

  return 0;
}
