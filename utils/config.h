/* config.h: This code is used for the reading of config files. */

#include <stdio.h>

char* ReadConfig(char* filename);
char** ParseConfig(char* string);
long GetFileSize(FILE *file);
long NumLines(char* str);
