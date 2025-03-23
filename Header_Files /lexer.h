#ifndef LEXER_H
#define LEXER_H

#include  "../Header_Files /errors.h"
#include "../Header_Files /datastrct.h"
#include "../Header_Files /globals.h"
#include "../Header_Files /table.h"




typedef struct OpCode {
    char *opcode;    /* The name of the opcode corresponding to the operation */
    int code;
    int funct;
} OpCode;

typedef enum {
    TOKEN_OPCODE,      
    TOKEN_MCRO,    
    TOKEN_ENDMCRO,      
    TOKEN_COMMA,       
    TOKEN_EOL,         
    TOKEN_NUMBER,
    TOKEN_DOUBLE,
    TOKEN_INSTRUCTION,
    TOKEN_REGISTER,
    TOKEN_COMMAND_NUMBER,
    TOKEN_AMP_LABEL,
    TOKEN_LABEL,
    TOKEN_ERROR,
    TOKEN_NULL
} TokenType;


typedef struct {
    TokenType type;
    char data[MAX_LABEL_LEN + 1];
} Token;

typedef struct Command{
    char opCode[5];
    Token arg1;
    Token arg2;
    int totalWords;
    int line;
}Command;


typedef struct CodeTable{
    int capacity;
    int IC;
    int size;
    Command* table;
}CodeTable;

typedef struct DataTable{
    int capacity;
    int DC;
    int size;
    int* data;
}DataTable;


void skip_whitespace(char *input, int* pos);
Token get_next_token(char *input, int* pos);
int is_empty_line(char line[]);
int is_comment_line(char line[]);
int is_instr(char str[]);
int isNum(char *str);
int is_double(char str[]);
int what_opcode(char str[]);
int what_reg(char str[]);
int contains_keyword(char *str, char *keyword);
int is_valid_macro_name(MacroNode *head, char name[]);
int isLabelDecl(char str[], Label* head, Error_Location location);
int isLegalLabelName(char name[], Label* head);
int readLabel(char line[], Label** head, Error_Location location, CodeTable* code_table, DataTable* data_table);
int readInstruction(char line[], Error_Location location, DataTable* data_table);
int read_entry_extern(char line[], Label** head, Error_Location location);
int expect(TokenType type, Token token, Error_Location location, int printError);
int readCommand(char line[], Error_Location location, CodeTable* code_table);

#endif
