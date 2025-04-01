#ifndef SECOND_PASS_H
#define SECOND_PASS_H
#include <stdlib.h>
#include <stdio.h>

/* 
    the function executes the second pass of the assembly program. 
    the function handles the translations of the commands, and instructions to hex, the creation of the .ob file, 
    and if needed the creation of the .ent, .ext files. the function also handles errors. 

    @param am_file_name - the name of the .am file. 
    @param am_file - the .am file. 
    @param code_table - the code table. 
    @param data_table - the data table. 
    @param head - the head pointer to the label list. 
    @param prevError - 1 if there were errors in the first pass, 0 otherwise. 
    @param macro_head_node - the head pointer to the macro list. 
*/
void secondPass(char* am_file_name, FILE* am_file, CodeTable* code_table, DataTable* data_table, 
    Label* head, int prevError, MacroNode *macro_head_node);


/* 
    the function adds an "entry" type to a label in the list of labels. if the label does not exist 
    in the list already, an error is printed. 

    @param label - the name of the label. 
    @param head - the head pointer to the label list. 
    @param location - the location of the line where the .entry instruction was found(file name, line number). 

    @return 1 if added "entry" type successfully , o otherwise. 
*/
int add_entry(char label[], Label* head, Error_Location location);


/* 
    the function creates a .ent file. if there are no .entry labels in the program, the file 
    is not created. 

    @param ent_file_name - the name of the .ent file to create. 
    @param head - the head pointer to the label list.

    @return 1 if creation was successful(or there were none entries), 0 if there was an error 
    creating the file. 
*/
int create_ent_file(char ent_file_name[], Label* head);


/* 
    the function creates the .ob and the .ext files. if there are errors in the commands, they are printed. 
    if there are no external labels in the file, the .ext file is removed. 

    @param am_file_name - the name of the .am file. 
    @param ob_file_name - the name of the .ob file. 
    @param ext_file_name - the name of the .ext file. 
    @param head - the head pointer to the label list.
    @param code_table - the code table. 
    @param data_table - the data table. 

    @return 1 if files created successfully, 0 if there was an error. 
*/
int create_ob_ext_files(char* am_file_name, char* ob_file_name, char* ext_file_name, Label* head, 
    CodeTable* code_table, DataTable* data_table);


/*
    the function writes a command to the .ob file and to the .ext file if needed. the function also makes sure 
    to discover errors in the command. 

    @param command - the command to translate and write. 
    @param IC - an int pointer to the instruction counter. 
    @param head - the head pointer to the label list.
    @param ob_file - the .ob file. 
    @param ext_file - the .ext file. 
    @param location - the location of the command(file name, line number). 

    @return 1 if added command successfully, 0 otherwise. 
*/
int write_ob_ext(Command command, int *IC, Label* head, FILE* ob_file, FILE* ext_file, Error_Location location);


/*
    the function parses the first(source) argument of the command, and writes the additional word to the .ob 
    file if needed. the function also discovers errors in this argument of the command. 

    @param IC - an int pointer to the instruction counter. 
    @param head - the head pointer to the label list.
    @param ob_file - the .ob file. 
    @param ext_file - the .ext file. 
    @param location - the location of the command(file name, line number). 
    @param arg1 - the token that represents the first argument of the command. 

    @return 1 if parsing was successful, 0 otherwise. 
*/
int parse_arg1(int *IC, Label* head, FILE* ob_file, FILE* ext_file, Error_Location location, Token arg1);


/*
    the function parses the second(destination) argument of the command, and writes the additional word to the .ob 
    file if needed. the function also discovers errors in this argument of the command. 

    @param IC - an int pointer to the instruction counter. 
    @param head - the head pointer to the label list.
    @param ob_file - the .ob file. 
    @param ext_file - the .ext file. 
    @param location - the location of the command(file name, line number). 
    @param arg1 - the token that represents the second argument of the command. 

    @return 1 if parsing was successful, 0 otherwise. 
*/
int parse_arg2(int *IC, Label* head, FILE* ob_file,FILE* ext_file, Error_Location location, Token arg2);

#endif
