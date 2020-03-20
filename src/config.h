#include <stdint.h>

struct qu_config_config {
	uint16_t network_port;

	int log_console_lvl;
	int log_file_lvl;
	uint32_t log_maxfilesize;
	const char* log_path;
};

struct qu_config_config qu_config_values;

int qu_config_init(char* fileName);
