#ifndef SECOND_PASS_H
#define SECOND_PASS_H
#include <stdlib.h>
#include <stdio.h>


int secondPass(char* am_file_name, FILE* am_file, CodeTable* code_table, DataTable* data_table, Label* head, int prevError);
int add_entry(char label[], Label* head, Error_Location location);
int create_ent_file(char ent_file_name[], Label* head);
int create_ob_ext_files(char* am_file_name, char* ob_file_name, char* ext_file_name, Label* head, 
    CodeTable* code_table, DataTable* data_table);
int write_ob_ext(Command command, int *IC, Label* head, FILE* ob_file, FILE* ext_file, Error_Location location);
int parse_arg1(int *IC, Label* head, FILE* ob_file, FILE* ext_file, Error_Location location, Token arg1);
int parse_arg2(int *IC, Label* head, FILE* ob_file,FILE* ext_file, Error_Location location, Token arg2);

#endif
