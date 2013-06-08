#include "config.h"
#include <stdio.h>

int main() {
	char* str;
	str = ReadConfig("test.txt");
	printf("%s\n", str);
	return 0;
}
