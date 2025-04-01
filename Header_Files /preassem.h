#ifndef PREASSEM_H
#define PREASSEM_H

#include  "../Header_Files /datastrct.h"
#include "../Header_Files /errors.h"
#include "../Header_Files /lexer.h"


/* 
    the function starts the pre assembler on the .as file. if no errors were found during 
    the pre assembly, a .am file is created, with all the macros expanded, macro declarations removed, comment lines 
    removed, and empty lines removed.  

    @param as_file_name - the name of the .as file. 
*/
void pre_assembler(char* as_file_name);


/* 
    the function creates a .am file with all the macros expanded, macro declarations removed, comment lines 
    removed, and empty lines removed. if there was an error, the .am file is not created, and an error message
    is printed. 

    @param as_file - the .as file. 
    @param as_file_name - the name of the .as file. 
    @param macro_head_node - the  double head pointer to the macro list. 

    @return 1 successfully created .am file, 0 otherwise. 
*/
int create_am_file(FILE* as_file, char* as_file_name, MacroNode **macro_head_node);


/* 
    the function adds all the macros and their bodys into the macro list. the function also handles 
    errors in declarations of macros and end of declarations. 

    @param as_file - the .as file. 
    @param as_file_name - the name of the .as file. 
    @param head - the head pointer to the macro list.

    @return 1 if all macros were added successfully, 0 otherwise. 
*/
int addAllMacros(FILE* as_file, char* as_file_name, MacroNode** head);


/* 
    the function expands all the  macros bodies from the .as file in the .am file. 

    @param as_file - the .as file. 
    @param as_file_name - the name of the .as file. 
    @param am_file - the .am file. 
    @param head - the head pointer to the macro list.

    @return 1 if expansion process was successful and there were no errors, 0 otherwise. 
*/
int expandMacros(FILE* as_file, char* as_file_name, FILE* am_file, MacroNode* head);


/* 
    the function removes all the macro declarations from the .am file. 

    @param am_file - the .am file. 
    @param am_file_name - the .am file name. 

    @return 1 if removal was successful, 0 otherwise. 
*/
int removeMacroDecl(FILE* am_file, char* am_file_name);

#endif
