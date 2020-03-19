enum qu_log_level {
	LVL_NONE,
	LVL_FATAL,
	LVL_ERROR,
	LVL_WARN,
	LVL_INFO,
	LVL_DEBUG
};

int qu_log_init();

void qu_log_raw(char*);

// args are module name and message
void qu_log_fatal(char*, char*);
void qu_log_error(char*, char*);
void qu_log_warn (char*, char*);
void qu_log_info (char*, char*);
void qu_log_debug(char*, char*);
