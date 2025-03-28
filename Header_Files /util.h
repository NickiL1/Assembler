#ifndef UTIL_H
#define UTIL_H
#include <stdio.h>

void copyFile(FILE *src, FILE *dest);
void create_file(char file_name[], char extension[], char buffer[]);
void free_all_memory(CodeTable *code_table, DataTable *data_table, Label *head);
#endif
