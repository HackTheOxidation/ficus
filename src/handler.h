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
#ifndef HANDLER_H_

#define HANDLER_H_

#include "config.h"

// HTTP Responses
#define OK_200 "HTTP/1.1 200 OK\n"
#define CREATED_201 "HTTP/1.1 201 Created\n"
#define ACCEPTED_202 "HTTP/1.1 202 Accepted\n"
#define NO_CONTENT_204 "HTTP/1.1 204 No Content\n"
#define BAD_REQUEST_400 "HTTP/1.1 400 Bad Request\r\n\n"
#define FORBIDDEN_403 "HTTP/1.1 403 Forbidden\n"
#define NOT_FOUND_404 "HTTP/1.1 404 Not Found\n"
#define NOT_IMPLEMENTED_501 "HTTP/1.1 501 Not Implemented\n"

// HTTP Requests
#define HTTP_GET "GET"
#define HTTP_HEAD "HEAD"
#define HTTP_PUT "PUT"
#define HTTP_POST "POST"
#define HTTP_DELETE "DELETE"
#define HTTP_CONNECT "CONNECT"
#define HTTP_OPTIONS "OPTIONS"
#define HTTP_TRACE "TRACE"
#define HTTP_PATCH "PATCH"

void handle_request(int client_socket, server_configuration *config);

#endif
