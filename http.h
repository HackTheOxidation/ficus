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

#ifndef HTTP_H_

#define HTTP_H_

#include "ficus.h"

// MIME-types
#define MIME_DEFAULT "application/octet-stream"
#define MIME_JS "application/javascript"
#define MIME_JSON "application/json"
#define MIME_JPEG "image/jpeg"
#define MIME_PNG "image/png"
#define MIME_CSS "text/css"
#define MIME_HTML "text/html"
#define MIME_PLAIN "text/plain"
#define MIME_ICO "image/vnd.microsoft.icon"

// MIME ENUM
enum {
  TYPE_DEFAULT = 0,
  TYPE_JS = 1,
  TYPE_JSON = 2,
  TYPE_JPEG = 3,
  TYPE_PNG = 4,
  TYPE_CSS = 5,
  TYPE_HTML = 6,
  TYPE_PLAIN = 7,
  TYPE_ICO = 8,
};

// Content formatting
#define CONTENT_TYPE(mime) "Content-Type: " mime
#define CONTENT_LENGTH "Content-Length: "
#define CONNECTION(state) "Connection: " state
#define CONTENT_ENCODING(encoding) "Content-Encoding: " encoding

int format_response_header(char *header, int type, int len, int isBinary);
int get_request_file_type(char *header, int *isBinary);

#endif
