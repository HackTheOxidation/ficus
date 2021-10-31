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
#include "http.h"

#define RECV_SIZE 4096

void send_response(int client_socket, server_configuration *config, char *buf, size_t buflen) {
  if (config->use_tls)
    tls_write(config->tls_cctx, buf, buflen);
  else
    send(client_socket, buf, buflen, 0);
}

void recv_request(int client_socket, server_configuration *config, char *buf, size_t buflen) {
  if (config->use_tls) {
    int r = tls_read(config->tls_cctx, buf, buflen);
    printf("Received request (%d bytes) from client through TLS: \n%s\n\n", r, buf);
  } else {
    recv(client_socket, buf, RECV_SIZE, 0);
    printf("Received request from client: \n%s\n\n", buf);
  }
}

void respond_200(int client_socket, server_configuration *config) {
  printf("Received Request. Sending 200 OK...\n\n");
  send_response(client_socket, config, OK_200, strlen(OK_200));
}

void respond_201(int client_socket, server_configuration *config) {
  printf("Received PUT/POST. Sending 201 Created...\n\n");
  send_response(client_socket, config, CREATED_201, strlen(CREATED_201));
}

void respond_202(int client_socket, server_configuration *config) {
  printf("Received Valid Request. Sending 202 Accepted...\n\n");
  send_response(client_socket, config, ACCEPTED_202, strlen(ACCEPTED_202));
}

void respond_204(int client_socket, server_configuration *config) {
  printf("Received HEAD. Sending 204 No Content...\n");
  send_response(client_socket, config, NO_CONTENT_204, strlen(NO_CONTENT_204));
}

void respond_400(int client_socket, server_configuration *config) {
  printf("Bad request. Sending 400..\n");
  send_response(client_socket, config, BAD_REQUEST_400, strlen(BAD_REQUEST_400));
}

void respond_404(int client_socket, server_configuration *config) {
  printf("Sending 404 - Not Found...\n");
  send_response(client_socket, config, NOT_FOUND_404, strlen(NOT_FOUND_404));
}

void respond_501(int client_socket, server_configuration *config) {
  printf("Request not implemented. Sending 501...\n");
  send_response(client_socket, config, NOT_IMPLEMENTED_501, strlen(NOT_IMPLEMENTED_501));
}

char *extract_payload(char *delimiter) {
  char *payload = calloc(RECV_SIZE, sizeof(char));

  int isContent = 0;
  char *content;
  while ((content = strtok(NULL, delimiter)) != NULL) {

    if (strlen(content) == 1) {
      isContent = 1;
      continue;
    }

    if (isContent) {
      strcpy(payload, content);
    }
  }

  return payload;
}

void handle_GET(char *header, int client_socket, server_configuration *config) {
  header = strtok(NULL, " ");
  printf("Page/Data requested: %s\n", header);

  char *requested_file = calloc(FILENAME_MAX, sizeof(char));
  strcpy(requested_file, config->content_path);
  strcat(requested_file, header);
  FILE *data;
  int isBinary = 0;
  int type = get_request_file_type(header, &isBinary);

  if (strcmp(header, "/") == 0) {
    char *index = calloc(FILENAME_MAX, sizeof(char));
    strcpy(index, config->content_path);
    strcat(index, header);
    strcat(index, config->index);
    printf("opening %s\n", index);
    data = fopen(index, "r");
    free(index);
  } else {
    printf("opening %s\n", requested_file);
    if (isBinary) {
      printf("file has binary format: %d.\n", isBinary);
      data = fopen(requested_file, "rb");
    } else
      data = fopen(requested_file, "r");
    free(requested_file);
  }

  if (data == NULL) {
    respond_404(client_socket, config);
    return;
  }

  char *response_data = 0;
  fseek(data, 0, SEEK_END);
  size_t size = ftell(data);
  fseek(data, 0, SEEK_SET);
  response_data = calloc(size, sizeof(char));

  int err = fread(response_data, size, 1, data);
  if (err < 0) {
    perror("ERROR: - Could not read file");
    exit(EXIT_FAILURE);
  }
  fclose(data);

  // Concatenate HTTP header with content of index.html
  char http_header[TRANSFERABLE_SIZE] = OK_200;
  int hlen = format_response_header(http_header, type, size, isBinary);
  printf("Header after formatting: \n%s\n", http_header);

  if (isBinary) {
    memcpy(http_header, response_data, size + hlen);

    printf("strlen: %ld, hlen: %d, size: %ld\n", strlen(http_header), hlen,
           size);
    printf("Sending response: \n%s\n", http_header);
    
    send_response(client_socket, config, http_header, hlen + size);
    printf("Response to: %s sent.\n", header);

  } else {
    char *charerr = strcat(http_header, response_data);
    if (charerr == NULL) {
      perror("ERROR: Could not format http_header.\n");
      exit(EXIT_FAILURE);
    }

    printf("strlen: %ld, hlen: %d, size: %ld\n", strlen(http_header), hlen,
           size);
    printf("Sending response: \n%s\n", http_header);

    send_response(client_socket, config, http_header, strlen(http_header));
    printf("Response to: %s sent.\n", header);
  }

  free(response_data);
}

void handle_PUT_POST(char *header, int client_socket, server_configuration *config, int is_post) {

  header = strtok(NULL, " ");
  printf("Page/Data requested: %s\n", header);

  char *requested_file = config->content_path;
  strcat(requested_file, header);
  FILE *dest;

  if (is_post == 0)
    dest = fopen(requested_file, "w");
  else
    dest = fopen(requested_file, "w+");

  if (dest == NULL) {
    respond_404(client_socket, config);
    return;
  }

  char delimiter[] = "\n";

  char *payload = extract_payload(delimiter);

  printf("\npayload: %s\n", payload);
  size_t written = fwrite(payload, 1, strlen(payload), dest);

  if (written < 1) {
    perror("Could not write to requested file.\n\n");
    exit(EXIT_FAILURE);
  }
  fclose(dest);
  free(payload);

  if (is_post == 0)
    respond_202(client_socket, config);
  else
    respond_201(client_socket, config);
}

void handle_request(int client_socket, server_configuration *config) {
  char request[TRANSFERABLE_SIZE]= {0};
  recv_request(client_socket, config, request, RECV_SIZE);

  char *header;
  header = strtok(request, " ");
  printf("Request type: %s\n", header);

  if (header == NULL) {
    respond_400(client_socket, config);
    return;
  }

  if (strcmp(header, HTTP_GET) == 0) {
    handle_GET(header, client_socket, config);
  } else if (strcmp(header, HTTP_HEAD) == 0) {
    respond_204(client_socket, config);
  } else if (strcmp(header, HTTP_POST) == 0) {
    handle_PUT_POST(header, client_socket, config, 1);
  } else if (strcmp(header, HTTP_PUT) == 0) {
    handle_PUT_POST(header, client_socket, config, 0);
  } else {
    respond_501(client_socket, config);
  }
}
