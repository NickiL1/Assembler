#ifndef ERRORS_H
#define ERRORS_H
#include "../Header_Files /globals.h"
typedef enum Error_Code_Name{
    NO_ERROR,
    FILE_HANDLE,
    AS_EXTENSION,
    CREATE_FILE,
    MEMORY_ALOC,
    EXTRA_TEXT,
    INVALID_MACRO_NAME,
    MISSING_ARG,
    INVALID_LABEL_NAME,
    MISSING_INFO,
    UNRECOGNIZED_COMMAND,
    NOT_A_NUMBER,
    MISSING_COMMA,
    MISSING_STRING,
    STRING_ENCLOSURE,
    MISSING_DATA,
    ILLEGAL_COMMA,
    MISSING_NET,
    ILLEGAL_ARG,
    ILLEGAL_DOUBLE,
    ILLEGAL_INSTR,
    MEM_LIM,
    UNDEFINED_LABEL,
    LABEL_CONFLICT,
    LABEL_ALREADY_DEFINED,
    ILLEGAL_LABEL_USE
} Error_Code_Name;

typedef struct Error{
    Error_Code_Name name;
    char* error_msg;
} Error;

typedef struct Error_Location{
    char filename[MAX_LABEL_LEN];
    int line;
} Error_Location;

void print_infile_error(Error_Code_Name error,Error_Location location);
void print_file_related_error(Error_Code_Name error,char* filename);
void print_sys_error(Error_Code_Name error);

#endif
