#include <stdio.h>
#include "../Header_Files /errors.h"

Error errors[] = {
    {NO_ERROR, "No errors."},
    {FILE_HANDLE, "Couldn't open file."},
    {AS_EXTENSION, "Missing .as extension"},
    {CREATE_FILE, "Couldn't create file."},
    {MEMORY_ALOC, "Failed to allocate memory"},
    {EXTRA_TEXT, "Extra text after end of command."},
    {INVALID_MACRO_NAME, "Invalid name for a macro."},
    {MISSING_ARG, "Missing argument."},
    {INVALID_LABEL_NAME,"Invalid name for a label."},
    {MISSING_INFO,"Missing command or instruction."},
    {UNRECOGNIZED_COMMAND, "Unrecognized command."},
    {NOT_A_NUMBER, "Not a  number"},
    {MISSING_COMMA, "Missing comma."},
    {MISSING_STRING, "Missing String."},
    {STRING_ENCLOSURE, "String is not surrounded by \" \"."},
    {MISSING_DATA, "Missing data."},
    {ILLEGAL_COMMA,"Illegal comma."},
    {MISSING_NET,"Missing # before number."},
    {ILLEGAL_ARG,"Illegal argument for this command."},
    {ILLEGAL_DOUBLE, "Doubles are not allowed in this language, only integers."},
    {ILLEGAL_INSTR, "Illegal instruction, has to be .data or .string."}
};

void print_sys_error(Error_Code_Name error){
    printf("ERROR: %s\n", errors[error].error_msg);
}

void print_file_related_error(Error_Code_Name error,char* filename){
    printf("ERROR: There was an error with file: %s - %s\n",filename,errors[error].error_msg);
}

void print_infile_error(Error_Code_Name error,Error_Location location){
    printf("ERROR: Error in file %s, at line: %d - %s\n", location.filename,
    location.line, errors[error].error_msg);
}
