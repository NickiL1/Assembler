#ifndef LEXER_H
#define LEXER_H

#include  "../Header_Files /errors.h"
#include "../Header_Files /datastrct.h"
#include "../Header_Files /globals.h"
#include "../Header_Files /table.h"




typedef struct op_code {
    char *opcode;    /* The name of the opcode corresponding to the operation */
    int arg_num;     /* The number of arguments for the operation */
} op_code;

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
    TOKEN_ERROR
} TokenType;


typedef struct {
    TokenType type;
    char data[MAX_LINE_LEN];
} Token;

typedef struct Command{
    char opCode[MAX_LABEL_LEN];
    char arg1[MAX_LABEL_LEN];
    char arg2[MAX_LABEL_LEN];
    int totalWords;
}Command;


typedef struct CodeTable{
    int IC;
    int size;
    Command table[MAX_IC_VAL];
}CodeTable;

typedef struct DataTable{
    int DC;
    int size;
    int data[MAX_IC_VAL];
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
