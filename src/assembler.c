#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../Header_Files /preassem.h"
#include "../Header_Files /errors.h"
#include "../Header_Files /table.h"
#include "../Header_Files /firstpass.h"
#include "../Header_Files /util.h"

int main(int argc, char* argv[]){
    int arg = 1;
    char filename[MAX_LINE_LEN];
    while (arg < argc){
        create_file_name(argv[arg],".as",filename);
        pre_assembler(filename);
        arg++;
    }
    return 0;
}
