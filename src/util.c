/* 
    @author - Nick Levin 326737046 

    the file implements some utility functions used throughout the assembler project. 
*/


#include <string.h>
#include <stdlib.h>
#include "../Header_Files /util.h"
#include "../Header_Files /globals.h"
#include "../Header_Files /datastrct.h"

void copyFile(FILE *src, FILE *dest) {
    int ch;
    /* copy all the characters from the src file to the dest file */
    while ((ch = fgetc(src)) != EOF) {
        fputc(ch, dest);
    }
}


void create_file_name(char file_name[], char extension[], char buffer[]){
    char* dot;
    strcpy(buffer,file_name);
    dot = strrchr(buffer, '.'); /* search for a '.' in the file name */
    if (dot == NULL|| dot == buffer) dot = buffer + strlen(buffer); /* no '.' in the file name or '.' in the begining */
    strcpy(dot, extension); /* add the extension */
}


void free_all_memory(CodeTable *code_table, DataTable *data_table, Label *head, MacroNode *macro_head_node){
    free(code_table->table);
    free(code_table);
    free(data_table->data);
    free(data_table);        
    freeLabelList(head);
    freeMacroList(macro_head_node);
}

