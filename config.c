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

#define LINESIZE 256

void parse_line(char* line, server_configuration *config) {
  char* identifier = strtok(line, "=");
  char* value = strtok(NULL, "=");

  if (strcmp(identifier, IDENTIFIER_CONTENT_PATH) == 0) {
    config->content_path = calloc(strlen(value), sizeof(char));
    strncpy(config->content_path, value, strlen(value) - 1);
  } else if (strcmp(identifier, IDENTIFIER_PORT) == 0)
    config->port = atoi(value);
}

void load_config(server_configuration *config) {
  FILE* fp = fopen(CONFIG_PATH, "r");
  
  if (fp == NULL) {
    printf("No configuration file found. Using defaults.\n");
    return;
  }

  printf("Using configuration: %s\n", CONFIG_PATH);

  char line[LINESIZE];
  while (fgets(line, LINESIZE, fp) != NULL) {
    parse_line(line, config);
  }

  fclose(fp);
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
