#include <stdlib.h>

#include "../lib/inih/ini.h"
#include "config.h"
#include "log.h"

static const char* const MOD = "config";

static int new_entry_handler(void* user, const char* section, const char* name, const char* value) {
	struct qu_config_config* config = (struct qu_config_config*)user;

	#define MATCH(s, n) strcmp(section, s) == 0 && strcmp(name, n) == 0
	if (MATCH("network", "port")) {
		int port = atoi(value);
		if (port < 1024 || port > 65535) {
			qu_log_fatal(MOD, "Invalid network_port value in config!");
			return 0;
		}
		config->network_port = (uint16_t)port;
	}

	return 1;
}

int qu_config_init(char* fileName) {
	return ini_parse(fileName, new_entry_handler, &qu_config_values);
}
