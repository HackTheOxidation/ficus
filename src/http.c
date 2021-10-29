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

#include "ficus.h"
#include "http.h"

char *get_mime_type(int type) {
  switch (type) {
  case TYPE_JS:
    return CONTENT_TYPE(MIME_JS);
  case TYPE_JSON:
    return CONTENT_TYPE(MIME_JSON);
  case TYPE_JPEG:
    return CONTENT_TYPE(MIME_JPEG);
  case TYPE_PNG:
    return CONTENT_TYPE(MIME_PNG);
  case TYPE_CSS:
    return CONTENT_TYPE(MIME_CSS);
  case TYPE_HTML:
    return CONTENT_TYPE(MIME_HTML);
  case TYPE_PLAIN:
    return CONTENT_TYPE(MIME_PLAIN);
  case TYPE_ICO:
    return CONTENT_TYPE(MIME_ICO);
  default:
    return CONTENT_TYPE(MIME_DEFAULT);
  }
}

int format_response_header(char *header, int type, int len, int isBinary) {
  char *mime = get_mime_type(type);

  char *err = strcat(header, mime);
  if (err == NULL) {
    perror("ERROR - Could not format header with mime type.");
    exit(EXIT_FAILURE);
  }

  strcat(header, "\n");
  char num[8];
  sprintf(num, "%d\n", len);

  err = strcat(header, CONTENT_LENGTH);
  if (err == NULL) {
    perror("ERROR - Could not format header with content length.");
    exit(EXIT_FAILURE);
  }

  strcat(header, num);
  if (isBinary) {
    err = strcat(header, CONTENT_ENCODING("gzip"));
    if (err == NULL) {
      perror("ERROR - Could not format header with content encoding.");
      exit(EXIT_FAILURE);
    }
    strcat(header, "\n");
  }

  err = strcat(header, CONNECTION("close"));
  strcat(header, "\r\n\n");

  return strlen(header);
}

int get_request_file_type(char *header, int *isBinary) {
  if (strcmp(header, "/") == 0) {
    isBinary = 0;
    return TYPE_HTML;
  }

  char copy[strlen(header) + 1];
  strcpy(copy, header);
  strtok(copy, ".");
  char *extention = strtok(NULL, ".");

  if (strcmp(extention, "js") == 0) {
    *isBinary = 0;
    return TYPE_JS;
  } else if (strcmp(extention, "json") == 0) {
    *isBinary = 0;
    return TYPE_JSON;
  } else if ((strcmp(extention, "jpeg") == 0) ||
             (strcmp(extention, "jpg") == 0)) {
    *isBinary = 1;
    return TYPE_JPEG;
  } else if (strcmp(extention, "png") == 0) {
    *isBinary = 1;
    return TYPE_PNG;
  } else if (strcmp(extention, "css") == 0) {
    *isBinary = 0;
    return TYPE_CSS;
  } else if ((strcmp(extention, "html") == 0) ||
             (strcmp(extention, "htm") == 0)) {
    *isBinary = 0;
    return TYPE_HTML;
  } else if (strcmp(extention, "txt") == 0) {
    *isBinary = 0;
    return TYPE_PLAIN;
  } else if (strcmp(extention, "ico") == 0) {
    *isBinary = 1;
    return TYPE_ICO;
  } else {
    *isBinary = 0;
    return TYPE_DEFAULT;
  }
}
