#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "util.h"

char* qu_util_strcatn(const char* str_a, int str_a_len, const char* str_b, int str_b_len) {
	char* str = malloc(sizeof(char) * (str_a_len + str_b_len + 1));
	memcpy((void*)str, (void*)str_a, str_a_len);
	memcpy((void*)(str + str_a_len), (void*)str_b, str_b_len);
	*(str + str_a_len + str_b_len) = 0;
	return str;
}

char* qu_util_strcat(const char* str_a, const char* str_b) {
	return qu_util_strcatn(str_a, strlen(str_a), str_b, strlen(str_b));
}

void qu_util_itoan(int val, char* buffer, int buffer_size) {
	for (int i = buffer_size - 1; i > 0 && val > 0; i--, val /= 10) {
		buffer[i] = "0123456789"[val % 10];
	}
}

void qu_util_itoa(int val, char* buffer) {
	return qu_util_itoan(val, buffer, strlen(buffer));
}

char* qu_util_strdup(const char* str) {
	char* buff = malloc(sizeof(char) * (strlen(str) + 1));
	return strcpy(buff, str);
}

