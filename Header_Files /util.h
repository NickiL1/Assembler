#ifndef UTIL_H
#define UTIL_H
#include <stdio.h>
#include "../Header_Files /lexer.h"
#include "../Header_Files /preassem.h"


/* 
    the function copies all the content from the src file to the dest file. 

    @param src - the file to copy. 
    @param dest - the file that the copied content gets written to. 
*/
void copyFile(FILE *src, FILE *dest);


/* 
    the function creates a new file name with a specified extension. 

    @param file_name - the original file name(could have any extension or no extension at all). the function 
    does not change this string. 
    @param extension - the extension to add. 
    @param buffer - the new file name will be stored in this string. 
*/
void create_file_name(char file_name[], char extension[], char buffer[]);


/* 
    the function frees all the memory allocated by the assembler. 

    @param code_table - the code table. 
    @param data_table - the data_table. 
    @param head - the head pointer to the label list. 
    @param macro_head_node - the head pointer to the macro list. 
*/
void free_all_memory(CodeTable *code_table, DataTable *data_table, Label *head, MacroNode *macro_head_node);
#endif
