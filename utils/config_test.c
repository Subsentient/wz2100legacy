#include "config.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
	char* str;
	char** arr;

	str = ReadConfig("test.txt");
	if (str == NULL) {
		printf("%s\n", "String obtained is null!");
		exit(1);
	}
	/* printf("%s\n", str); */
	
	arr = ParseConfig(str);
	if (arr == NULL) {
		printf("%s\n", "Array obtained is null!");
		exit(1);
	}
	
	puts(arr[0]);
	
	free(str);
	free(arr);
	return 0;
}
