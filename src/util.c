#include <stdlib.h>
#include <string.h>

char* qu_util_strcatn(char* str_a, int str_a_len, char* str_b, int str_b_len) {
	char* str = malloc(sizeof(char) * (str_a_len + str_b_len + 1));
	memcpy((void*)str, (void*)str_a, str_a_len);
	memcpy((void*)(str + str_a_len), (void*)str_b, str_b_len);
	*(str + str_a_len + str_b_len) = 0;
	return str;
}

char* qu_util_strcat(char* str_a, char* str_b) {
	return qu_util_strcatn(str_a, strlen(str_a), str_b, strlen(str_b));
}



