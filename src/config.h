#include <stdint.h>

struct qu_config_config {
	uint16_t network_port;	
};

struct qu_config_config qu_config_values;

int qu_config_init(char* fileName);
