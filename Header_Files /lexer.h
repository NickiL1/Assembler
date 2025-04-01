#ifndef LEXER_H
#define LEXER_H

#include  "../Header_Files /errors.h"
#include "../Header_Files /datastrct.h"
#include "../Header_Files /globals.h"
#include "../Header_Files /table.h"




typedef struct OpCode {
    char *opcode;    /* The name of the opcode corresponding to the operation */
    int code;   /* the decimal opcode */
    int funct; 
} OpCode;

typedef enum {
    TOKEN_OPCODE,      
    TOKEN_MCRO,    /* the string "mcro"*/
    TOKEN_MCROEND,     /* the string "mcroend"*/ 
    TOKEN_COMMA,       
    TOKEN_EOL,         
    TOKEN_NUMBER,   /* An integer - not a double*/
    TOKEN_DOUBLE,   /* strictly double*/
    TOKEN_INSTRUCTION, /* .entry, .extern, .data or .string */
    TOKEN_REGISTER, /* r0 - r7 */
    TOKEN_COMMAND_NUMBER, /* a number with "#" before it */
    TOKEN_AMP_LABEL, /* a label with a "&" before it*/
    TOKEN_LABEL, /* plain label(without ':')*/
    TOKEN_ERROR,
    TOKEN_NULL /* just an empty token */
} TokenType;


typedef struct {
    TokenType type;
    char data[MAX_LABEL_LEN + 1];
} Token;

typedef struct Command{
    char opCode[5];
    Token arg1;
    Token arg2;
    int totalWords; /* the total number of words for this command */
    int line; /* the line on which the command was incounterred in the .am file  */
}Command;


typedef struct CodeTable{
    int capacity; /* max capacity*/
    int IC;
    int size; /* actual used size*/
    Command* table; /* table of commands */
}CodeTable;

typedef struct DataTable{
    int capacity; /* max capacity*/
    int DC;
    int size; /* actual used size*/
    int* data; /* table of the data image  */
}DataTable;


/*
    the function skips all the white spaces. 
    at the end the *pos param will point to the first non space char. 

    @param input - the string in which to skip the spaces. 
    @param pos - an int pointer to the current position in the input string. 
*/
void skip_whitespace(char *input, int* pos);


/* 
    the function returns the next token in input string, starting from *pos index.

    @param input - the input string. 
    @param pos - the int pointer that holds the current position in the input string. 

    @return - a Token that holds the next tokens data(type, data)
*/
Token get_next_token(char *input, int* pos);


/* 
    a function that checks if a line is empty. 

    @param line - the line to check. 

    @return 1 if empty, 0 otherwise. 
*/
int is_empty_line(char line[]);


/* 
    a function that checks if a line is a comment line. 

    @param line - the line to check. 

    @return 1 if comment line , 0 otherwise. 
*/
int is_comment_line(char line[]);


/* 
    a function that checks if a string is one of the instructions(.data, .string .entry, or .external). 

    @param str - the string to check. 

    @return 1 if instruction , 0 otherwise. 
*/
int is_instr(char str[]);


/* 
    a function that checks if a string is an integer number. 

    @param str - the string to check. 

    @return 1 if integer, 0 otherwise. 
*/
int isNum(char *str);


/* 
    a function that checks if a string is a double. 

    @param str - the string to check. 

    @return 1 if double, 0 otherwise. 
*/
int is_double(char str[]);


/* 
    a function that checks if a string is one of the opcodes.

    @param str - the string to check. 

    @return a number >=0 if opcode , -1 otherwise. 
*/
int what_opcode(char str[]);


/* 
    a function that checks if a string is one of the registers.

    @param str - the string to check. 

    @return a number >=0 if register , -1 otherwise. 
*/
int what_reg(char str[]);


/* 
    a function that checks if a certain keyword is present in a string. 

    @param str - the string in which to look for the keyword. 
    @param keyword - the keyword to look for. 

    @return 1 if keyword is present, 0 otherwise. 
*/
int contains_keyword(char *str, char *keyword);


/* 
    a function that checks if a given string is a valid name for a macro. 

    @param head - the head pointer to the macro list. 
    @param name - the name to check if is valid. 

    @return 1 if valid, 0 otherwise. 
*/
int is_valid_macro_name(MacroNode *head, char name[]);
/* 
    a function that checks if a given string is a valid name for a label. 

    @param name - the name to check if is valid.
    @param head - the head pointer to the label list.
    @param macro_head_node - the head pointer to the macro list.

    @return 1 if valid, 0 otherwise. 
*/
int isLegalLabelName(char name[], Label* head, MacroNode *macro_head_node);


/* 
    a function that checks if a string is a label declaration line(meaning label: ...). 

    @param str - the input string. 
    @param head - the head pointer to the list of labels. 
    @param location - the location of the string(file name and line). 
    @param macro_head_node - the head pointer to the macro list. 

    @return -1 if there is no label declaration, 0 if it is a label declaration but the name of the label is illegal, 1 if valid label declaration. 
*/
int isLabelDecl(char str[], Label* head, Error_Location location, MacroNode *macro_head_node);


/* 
    the function reads a line with a label declaration, stores the label in the label list, and parse the command or instruction after the label 
    declaration. 

    @param line - the line to read. 
    @param head - a double pointer to the head of the label list. 
    @param location - the location of the line (file name and line number).
    @param code_table - the code table. 
    @param data_table - the data table. 

    @return 1 if the line is a valid line according to the assembler rules, 0 if an error was raised at one of the stages of the analysis, -1 if after the 
    label there was a .entry or .extern instruction. 
*/
int readLabel(char line[], Label** head, Error_Location location, CodeTable* code_table, DataTable* data_table);


/*
    the function reads a .string or .data instruction, stores the data in the data table and updates the data counter accordingly. 

    @param line - the string with the instruction to analyze. 
    @param location - the location of the line (file name and line number).
    @param data_table - the data table.

    @return -1 if instruction is .extern or .entry, 0 if there was an error during the analysis of the line, 1 if there were no errors. 

*/
int readInstruction(char line[], Error_Location location, DataTable* data_table);


/* 
    the function reads and parses a .entry or .extern instruction. if it is a .entry instruction the function just checks 
    syntax clarity. if instruction is .extern the function also adds the label to the label list with type "external"

    @param line - the line to parse. 
    @param head - a double pointer to the head of the label list. 
    @param location - the location of the line (file name and line number).

    @return -1 if .entry or .extern were not found in the line, 0 if there was an error with the line(like a missing label after .extern), 
    1 if line was parsed successfully without errors. 
*/
int read_entry_extern(char line[], Label** head, Error_Location location);


/* 
    the function receives an expected token type and the actual token and it handles matches / mismatches 
    and the printing of these errors. 

    @param type - the TokenType to be expected to get. 
    @param token - the actual token we got. 
    @param location - the location of the line (file name and line number).
    @param printError - controls if the function prints an error for mismatching types strictly for 
    command arguments types. 1 to print an error for mismatched types, 0 to not print an error. 
    
    @return 0 if there was a mismatch of types, 1 if types match, 2 if there is a command argument mismatch, for 
    example for opcode "add" expected label but got register, this is not necessarily an error. 
*/
int expect(TokenType type, Token token, Error_Location location, int printError);


/*
    the function reads a command, and stores it in the code_table. if the command has an error, it will 
    not be stored, and an error will be printed. 

    @param line - the line to parse. 
    @param location - the location of the line (file name and line number).
    @param code_table - the code table.

    @return 1 if command is valid and stored successfully, 0 if there was an error. 
*/
int readCommand(char line[], Error_Location location, CodeTable* code_table);

#endif
