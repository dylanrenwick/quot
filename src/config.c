#include "../lib/inih/ini.h"
#include "config.h"

static int new_entry_handler(void* user, const char* section, const char* name, const char* value) {


    return 1;
}

int qu_config_init(char* fileName) {
    return ini_parse(fileName, new_entry_handler, &config);
}
