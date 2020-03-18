#include <stdio.h>

#include "config.h"


#define ART_BUFFER_SIZE 80
void print_ascii_art();

int main(int argc, char* argv[]) {
	

	qu_config_init("./quot.cfg");

	return 0;
}

void print_ascii_art() {
	FILE* art_file;

	if (! (art_file = fopen("resources/quot_ascii_art.txt", "r"))) {
		
	}

	char buffer[ART_BUFFER_SIZE];


}
