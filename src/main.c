#include <stdio.h>

#include "config.h"
#include "netwk.h"

#define ART_BUFFER_SIZE 80
void print_ascii_art();

int main(int argc, char* argv[]) {
	print_ascii_art();

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

void print_ascii_art() {
	FILE* art_file;

	if (! (art_file = fopen("resources/quot_ascii_art.txt", "r"))) {
		puts("Starting Quot, a fast and flexible chat system licensed under the Affero GPL.");
		return;
	}

	char buffer[ART_BUFFER_SIZE];
	while (fgets(buffer, ART_BUFFER_SIZE, art_file) != NULL) {
		fputs(buffer, stdout);
	}

	fclose(art_file);
}
