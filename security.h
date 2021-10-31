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

#ifndef SECURITY_H_
#define SECURITY_H_

#include <tls.h>
#include "config.h"

int init_security(struct server_configuration *config);
int accept_and_shake_hands(struct server_configuration *config, int *server_socket, int *client_socket);

#endif
