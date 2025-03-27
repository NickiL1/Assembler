#ifndef PREASSEM_H
#define PREASSEM_H

#include  "../Header_Files /datastrct.h"
#include "../Header_Files /errors.h"
#include "../Header_Files /lexer.h"

MacroNode *macro_head_node;

void pre_assembler(char* as_file_name);
int create_am_file(FILE* as_file, char* as_file_name);
int addAllMacros(FILE* as_file, char* as_file_name, MacroNode** head);
int expandMacros(FILE* as_file, char* as_file_name, FILE* am_file, MacroNode* head);
int removeMacroDecl(FILE* am_file, char* am_file_name);
#endif
