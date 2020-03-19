#include <stdio.h>
#include <time.h>
#include <string.h>

#include "util.c"
#include "log.h"


enum qu_log_level qu_log_file_lvl;
enum qu_log_level qu_log_console_lvl;
#ifdef VER_RELEASE
qu_log_console_lvl = LVL_INFO;
#else
qu_log_console_lvl = LVL_DEBUG;
#endif

FILE *qu_log_fh = NULL;
uint32_t qu_log_filesize_limit;


int qu_log_init() {
	qu_log_console_lvl = console_lvl;
	qu_log_file_lvl = file_lvl;

	// Don't bother getting a handle if we're not going to log anything
	if (qu_log_file_lvl != LVL_NONE) {
		log_fh = fopen(qu_util_strcat(path, "latest.log"), "w");
		if (log_fh == NULL) {
			fprintf(stderr, "Failed to open log file for writing. errno = %d", errno);
			return 0;
		}
	}

	qu_log_filesize_limit = filesize_limit;
}

void qu_log_common(enum qu_log_level lvl, char* lvlstr, char* module, char* msg) {
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
		FILE* outstream = lvl <= WARN ? stdout : stderr;
		fputs(timestamp, outstream);
		fputs( lvlstamp, outstream);
		fputs( modstamp, outstream);
		fputs(      msg, outstream);
		fputs(     "\n", outstream);
	}

	// --- Log to file ---
	// Here we just stick our head in the ground and pretend there are no errors.
	// After all, what are we going to do, log an error?
	// TODO maybe not stick our head in the ground
	if (lvl <= qu_log_console_lvl && qu_log_fh != NULL) {
		fputs(timestamp, qu_log_fh);
		fputs( lvlstamp, qu_log_fh);
		fputs( modstamp, qu_log_fh);
		fputs(      msg, qu_log_fh);
		fputs(     "\n", qu_log_fh);
	}
}
