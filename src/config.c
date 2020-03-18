#include "../lib/inih/ini.h"
#include "config.h"

static int new_entry_handler(void* user, const char* section, const char* name, const char* value) {
	struct qu_config* config = (struct qu_config*)user;

	#define MATCH(s, n) strcmp(section, s) == 0 && strcmp(name, n) == 0



	return 1;
}

int qu_config_init(char* fileName) {
	return ini_parse(fileName, new_entry_handler, &config);
}
