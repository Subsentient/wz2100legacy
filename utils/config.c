/* config.c: This code is used for the reading of config files. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"

char* ReadConfig(char* filename)
{
    FILE* file;
    char* str;
    unsigned long i;
    
    file = fopen(filename, "r");
    if (file == NULL)
    {
        fprintf(stderr, "Input file %s cannot be opened!\n", filename);
        exit(1);
    }
    
	str = malloc(GetFileSize(filename));
	for (i = 0; i < GetFileSize(filename); ++i)
    {
        str[i] = getc(file);
    }
    str[i] = '\0';
	fclose(file);
    
	return str;
}

char** ParseConfig(char* string) 
{
    unsigned long i;
    char* delims = "\n";
    char* result = NULL;
    char** arr;
    
    arr = malloc(sizeof(char*) * NumLines(string));
    
    for (i = 0; i < NumLines(string); ++i)
    {
    	arr[i] = malloc(8192);
	}
	
	i = 0; do
	{
        result = strtok(string, delims);
        strcpy(arr[i], result); /*Here, we are copying into an empty pointer.*/
    } while (++i, result != NULL);

    
    return arr;
}


long GetFileSize(char* filename) 
{
    char tmpChar;
    FILE *tmpFile = NULL;
    unsigned long FileSize = 0;
    
    tmpFile = fopen(filename, "r");
    do
    {
        tmpChar = getc(tmpFile);
    } while (++FileSize, tmpChar != EOF);
    
    return FileSize;
}

long NumLines(char* str) 
{
	unsigned long NumLines = 1;
	unsigned long i;
	for (i = 0; str[i] != '\0'; ++i)
	{
		if (str[i] == '\n')
		{
			++NumLines;
		}
	}
	return NumLines;
}
