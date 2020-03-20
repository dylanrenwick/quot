#include <errno.h>
#include <netdb.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "config.h"
#include "log.h"
#include "util.h"

#define BACKLOG 10

static const char* const MOD = "netwk";

void sigchld_handler(int s) {
	int saved_errno = errno;
	while(waitpid(-1, NULL, WNOHANG) > 0);
	errno = saved_errno;
}

static int sock_fd;
static int new_fd;

int qu_netwk_init() {
	uint16_t port = qu_config_values.network_port;
	char port_buffer[6];

	qu_util_itoan(port, port_buffer, 6);

	struct addrinfo hints;
	struct addrinfo* servinfo;
	int yes = 1;
	
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	int rv;

	if ((rv = getaddrinfo(NULL, port_buffer, &hints, &servinfo)) != 0) {
		char* gai_err = gai_strerror(rv);
		char* msg = qu_util_strcatn("Failed getaddrinfo(): ", 22, gai_err, strlen(gai_err));
		qu_log_fatal(MOD, "msg");
		free(msg);
		return 0;
	}

	struct addrinfo* p;
	for (p = servinfo; p != NULL; p = p->ai_next) {
		if ((sock_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
			continue;
		}
		if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
			continue;
		}
		if (bind(sock_fd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sock_fd);
			continue;
		}

		break;
	}

	freeaddrinfo(servinfo);

	if (p == NULL) {
		qu_log_fatal(MOD, "Failed to bind to configured port!");
		return 0;
	}

	if (listen(sock_fd, BACKLOG) == -1) {
		qu_log_fatal(MOD, "Could not listen on port!");
		return 0;
	}

	struct sigaction sa;

	sa.sa_handler = sigchld_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if (sigaction(SIGCHLD, &sa, NULL) == -1) {
		qu_log_fatal(MOD, "Could not reap dead processes!");
		return 0;
	}

	qu_log_info(MOD, "Server now waiting for connections...");
	return 1;
}
