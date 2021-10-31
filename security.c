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

#include <stdio.h>
#include <errno.h>
#include <err.h>
#include <sys/socket.h>

#include "security.h"

int init_security(struct server_configuration *config) {
	if (tls_init() == -1) {
    perror("WARNING: Unable to initialize TLS.\n");
		return -1;
	}

	if ((config->tls_cfg = tls_config_new()) == NULL) {
    perror("WARNING: Unable to allocate TLS config.\n");
		return -1;
	}

	if (tls_config_set_ca_file(config->tls_cfg, config->root_CA) == -1) {
		perror("WARNING: Unable to set root CA file.\n");
		return -1;
	}

	if (tls_config_set_cert_file(config->tls_cfg, config->tls_cert) == -1) {
		perror("WARNING: Unable to set TLS certificate file.\n");
		return -1;
	}

	if (tls_config_set_key_file(config->tls_cfg, config->tls_key) == -1) {
		perror("WARNING: Unable to set TLS key file.\n");
		return -1;
	}

	if ((config->tls_ctx = tls_server()) == NULL) {
		perror("WARNING: TLS server creation failed.\n");
		return -1;
	}

	if (tls_configure(config->tls_ctx, config->tls_cfg) == -1) {
		perror("WARNING: TLS configuration failed.\n");
		return -1;
	}

	return 0;
}

int accept_and_shake_hands(struct server_configuration *config, int *server_socket, int *client_socket) {
	*client_socket = accept(*server_socket, NULL, NULL);
  if (*client_socket == -1) {
    perror("\nERROR (Non-Critical): Could not accept client connection.\n");
		return -1;
  }

	if (config->use_tls) {
  	if (tls_accept_socket(config->tls_ctx, &config->tls_cctx, *client_socket) != 0) {
    	perror("\nERROR (Non-Critical): Could not TLS accept client connection.\n");
			return -1;
  	}

		/*
		if (tls_handshake(config->tls_cctx) != 0) {
			perror("\nERROR (Non-Critical): TLS handshake failed.\n");
			return -1;
		}
		*/
		printf("Accepted client TLS connection.\n");
	} else {
		printf("Accepted client connection.\n");
	}

	return 0;
}
