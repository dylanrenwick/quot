enum qu_log_level {
	LVL_NONE,
	LVL_FATAL,
	LVL_ERROR,
	LVL_WARN,
	LVL_INFO,
	LVL_DEBUG
};

int qu_log_init();

//void qu_log_raw(char*);

// args are module name and message
void qu_log_fatal(const char*, char*);
void qu_log_error(const char*, char*);
void qu_log_warn (const char*, char*);
void qu_log_info (const char*, char*);
void qu_log_debug(const char*, char*);
