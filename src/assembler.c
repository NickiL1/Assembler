#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../Header_Files /preassem.h"
#include "../Header_Files /errors.h"
#include "../Header_Files /table.h"
#include "../Header_Files /firstpass.h"

int main(int argc, char* argv[]){
    int arg = 1;
    while (arg < argc){
        if(strstr(argv[arg], ".as") == NULL){
            print_file_related_error(AS_EXTENSION, argv[arg]);
            arg++;
            continue;
        }
        pre_assembler(argv[arg]);
        arg++;
    }
    return 0;
}
