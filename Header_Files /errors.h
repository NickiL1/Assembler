#ifndef ERRORS_H
#define ERRORS_H
#include "../Header_Files /globals.h"

/* defining all the possible errors codes displayed and found by the assembler*/
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
    Error_Code_Name name; /* the error code */
    char* error_msg; /* the error message content*/
} Error;

typedef struct Error_Location{
    char filename[MAX_LABEL_LEN]; /* the file in which the error occurred*/
    int line; /* the line at which the error occurred*/
} Error_Location;


/* 
    the function prints an error message based on an error that occurred inside a specific file.
    
    @param error - the error code to print
    @param location - the location aat which the error was discovered.
*/
void print_infile_error(Error_Code_Name error,Error_Location location);


/*
    the function prints an error message based on an error that occurred outside the file, but related to
    a certain file.

    @param error - the error code to print.
    @param filename - the name of the file to which the error is connected.
*/
void print_file_related_error(Error_Code_Name error,char* filename);


/*
    the function prints a system related error(mainly memory allocation errors). 

    @param error - the error code to print.
*/
void print_sys_error(Error_Code_Name error);

#endif
