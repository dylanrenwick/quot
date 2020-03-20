#include <stdio.h>
#include <time.h>
#include <string.h>

#include "util.c"
#include "config.h"
#include "log.h"


enum qu_log_level qu_log_file_lvl = LVL_NONE;
enum qu_log_level qu_log_console_lvl;
#ifdef VER_RELEASE
qu_log_console_lvl = LVL_INFO;
#else
qu_log_console_lvl = LVL_DEBUG;
#endif

FILE *qu_log_fh = NULL;
uint32_t qu_log_filesize_limit;

static void qu_log_rotate();

int qu_log_init() {
	qu_log_console_lvl = qu_config_values.log_console_lvl;
	qu_log_file_lvl = qu_config_values.log_file_lvl;

	// Don't bother getting a handle if we're not going to log anything
	if (qu_log_file_lvl != LVL_NONE) {
		char* logname = qu_util_strcat(qu_config_values.log_path, "latest.log");
		qu_log_fh = fopen(logname, "w");
		free(logname);
		if (qu_log_fh == NULL) {
			fprintf(stderr, "Failed to open log file for writing. errno = %d", errno);
			return 0;
		}
	}

	qu_log_filesize_limit = qu_config_values.log_maxfilesize;

	return 1;
}

static void qu_log_common(enum qu_log_level lvl, char* lvlstr, const char* module, char* msg) {
	// Abort early if we can
	if (qu_log_file_lvl < lvl && qu_log_console_lvl < lvl) {
		return;
	}

	// ---Timestamp ---
	char timestamp[22];

	time_t curtime = time(NULL);
	struct tm* curtime_loc = localtime(curtime);
	sprintf(timestamp, "[%d-%02d-%02d %02d:%02d:%02d]",
		curtime_loc->tm_year + 1900,
		curtime_loc->tm_mon + 1,
		curtime_loc->tm_mday,
		curtime_loc->tm_hour,
		curtime_loc->tm_min,
		curtime_loc->tm_sec
	);

	// --- Severity level indicator ---
	char* lvlstamp = "[XXX]";
	strncpy(lvlstamp + 1, lvlstr, 3);

	// --- Module nameplate ---
	size_t module_len = strlen(module);
	char modstamp[module_len + 3];
	strncpy(modstamp + 1, module, module_len);
	modstamp[0] = '[';
	modstamp[module_len + 1] = ']';
	modstamp[module_len + 2] = '\0';

	// --- Log to console ---
	if (lvl <= qu_log_console_lvl) {
		// Choose an appropriate output stream based on severity level
		FILE* outstream = lvl <= LVL_WARN ? stdout : stderr;
		fputs(timestamp, outstream);
		fputs( lvlstamp, outstream);
		fputs( modstamp, outstream);
		fputs(      msg, outstream);
		fputs(     "\n", outstream);
	}

	// --- Log to file ---
	// Here we stick our head in the sand and pretend there are no errors.
	// After all, what are we going to do, log an error?
	// TODO maybe not stick our head in the sand
	if (lvl <= qu_log_console_lvl && qu_log_fh != NULL) {
		fputs(timestamp, qu_log_fh);
		fputs( lvlstamp, qu_log_fh);
		fputs( modstamp, qu_log_fh);
		fputs(      msg, qu_log_fh);
		fputs(     "\n", qu_log_fh);

		if (ftell(qu_log_fh) >= qu_log_filesize_limit) {
			qu_log_rotate();
		}
	}
}

static void qu_log_rotate() {
	// Generate a temporal name for the archived log
	char archive_name[24];
	time_t curtime = time(NULL);
	struct tm* curtime_loc = localtime(curtime);
	sprintf(archive_name, "%d-%02d-%02d_%02d.%02d,%02d",
		curtime_loc->tm_year + 1900,
		curtime_loc->tm_mon + 1,
		curtime_loc->tm_mday,
		curtime_loc->tm_hour,
		curtime_loc->tm_min,
		curtime_loc->tm_sec
	);
	strcpy(archive_name + 19, ".log");

	char* current_log = qu_util_strcat(qu_config_values.log_path, "latest.log");
	char* archive_log = qu_util_strcat(qu_config_values.log_path, archive_name);

	fclose(qu_log_fh);

	// TODO figure out what to do if we can't rename()
	rename(current_log, archive_log);

	qu_log_fh = fopen(current_log, "w");
	if (qu_log_fh == NULL) {
		fprintf(stderr, "Failed to open log file for writing. errno = %d", errno);
	}

	free(current_log);
	free(archive_log);
}

void qu_log_fatal(const char* module, char* msg) {
	qu_log_common(LVL_FATAL, "FTL", module, msg);
}

void qu_log_error(const char* module, char* msg) {
	qu_log_common(LVL_ERROR, "ERR", module, msg);
}

void qu_log_warn(const char* module, char* msg) {
	qu_log_common(LVL_WARN, "WRN", module, msg);
}

void qu_log_info(const char* module, char* msg) {
	qu_log_common(LVL_INFO, "INF", module, msg);
}

void qu_log_debug(const char* module, char* msg) {
	qu_log_common(LVL_DEBUG, "DBG", module, msg);
}
