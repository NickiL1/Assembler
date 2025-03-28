#include <string.h>
#include <stdlib.h>
#include "../Header_Files /util.h"
#include "../Header_Files /globals.h"
#include "../Header_Files /lexer.h"
#include "../Header_Files /preassem.h"
#include "../Header_Files /datastrct.h"

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


void free_all_memory(CodeTable *code_table, DataTable *data_table, Label *head){
    free(code_table->table);
    free(code_table);
    free(data_table->data);
    free(data_table);        
    freeLabelList(head);
    freeMacroList(macro_head_node);
}

