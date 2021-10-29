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

#include "config.h"

void load_config(server_configuration *config) {
  config->port = DEFAULT_PORT;
  config->content_path = DEFAULT_CONTENT_PATH;
}

void setup_server(int *server_socket, server_configuration *config) {
  int err;
  *server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (*server_socket < 0) {
    perror("\nERROR: Could not create server socket.\n");
    exit(EXIT_FAILURE);
  }

  // Define the address
  struct sockaddr_in server_address;
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(config->port);
  server_address.sin_addr.s_addr = INADDR_ANY;

  // Get rid of "Address already in use"!
  int yes = 1;
  if (setsockopt(*server_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes) ==
      -1) {
    perror("\nERROR: Failed setsockopt.\n");
    exit(EXIT_FAILURE);
  }
  // Bind the address to the socket
  err = bind(*server_socket, (struct sockaddr *)&server_address,
             sizeof(server_address));
  if (err < 0) {
    perror("\nERROR: Could not bind server to address.\n");
    exit(EXIT_FAILURE);
  }

  printf("IP Address: %d\n", server_address.sin_addr.s_addr);
}
