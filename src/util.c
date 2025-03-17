#include <string.h>
#include "../Header_Files /util.h"

void copyFile(FILE *src, FILE *dest) {
    int ch;
    while ((ch = fgetc(src)) != EOF) {
        fputc(ch, dest);
    }
}
