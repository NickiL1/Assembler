#include <string.h>
#include <stdlib.h>
#include "../Header_Files /util.h"
#include "../Header_Files /globals.h"

void copyFile(FILE *src, FILE *dest) {
    int ch;
    while ((ch = fgetc(src)) != EOF) {
        fputc(ch, dest);
    }
}


void create_file(char file_name[], char extension[], char buffer[]){
    char* dot;
    strcpy(buffer,file_name);
    dot = strrchr(buffer, '.');
    if (dot == NULL) dot = buffer + strlen(buffer);
    strcpy(dot, extension);
}


