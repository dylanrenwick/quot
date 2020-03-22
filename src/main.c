#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "config.h"
#include "netwk.h"

#define ART_BUFFER_SIZE 80

void print_ascii_art(bool ignore_file) {
	FILE* art_file;

	if (ignore_file || !(art_file = fopen("resources/quot_ascii_art.txt", "r"))) {
		puts("Starting Quot, a fast and flexible chat system licensed under the Affero GPL.");
		return;
	}

	char buffer[ART_BUFFER_SIZE];
	while (fgets(buffer, ART_BUFFER_SIZE, art_file) != NULL) {
		fputs(buffer, stdout);
	}

	fclose(art_file);
}

int main(int argc, char** argv) {
	bool ignore_file = false;
	if (argc > 1 && strcmp(argv[1], "--no-ascii") == 0) {
		ignore_file = true;
	}

	print_ascii_art(ignore_file);

	// Initialize modules
	int config_status = qu_config_init("./quot.cfg");
	if (!config_status) {
		return 1;
	}

	int netwk_status = qu_netwk_init();
	if (!netwk_status) {
		return 2;
	}

	return 0;
}

