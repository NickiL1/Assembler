/*  
    @author- Nick Levin 326737046.

    the file implements a lexer and parser for the users instructions and commands, and handles 
    the  decomposition of the commands into its parts, while filling the data and code tables of the assembler.
    the program also handles errors if there were any. 
*/


#include <ctype.h> 
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "../Header_Files /lexer.h"
#include "../Header_Files /preassem.h"



/* Define the opcodes */
OpCode OPCODES_ARRAY[] = {
    {"mov",  0, 0},
    {"cmp",  1, 0},
    {"add",  2, 1},
    {"sub",  2, 2},
    {"lea",  4, 0},
    {"clr",  5, 1},
    {"not",  5, 2},
    {"inc",  5, 3},
    {"dec",  5, 4},
    {"jmp",  9, 1},
    {"bne",  9, 2},
    {"jsr",  9, 3},
    {"red",  12, 0},
    {"prn",  13, 0},
    {"rts",  14, 0},
    {"stop", 15, 0}
};
/* Define the registers */
char *REGISTERS_ARRAY[] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"};

/* Define the instructions */
char *INSTRUCTIONS_ARRAY[] = {".data", ".string", ".extern", ".entry"};

void skip_whitespace(char *input, int* pos) {
    while (isspace(input[(*pos)])) (*pos)++;
}

int is_empty_line(char line[]) { 
    int pos = 0;
    while (line[pos] != '\0') { 
        if (!isspace(line[pos])) {  /* found non space char */
            return 0;  
        }
        pos++;  
    }
    return 1; 
}

int is_comment_line(char line[]){
    int pos = 0;
    while (isspace(line[pos])) (pos)++;
    if(line[pos] == ';'){
        return 1;
    }
    return 0;    
}


int is_instr(char str[]) {
    int i;
    if (str == NULL) {
        return 0;
    }
    /* traverse all possible instructions */
    for (i = 0; i < INSTRUCTIONS_COUNT; i++) {
        if (strcmp(str, INSTRUCTIONS_ARRAY[i]) == 0) {
            return 1; 
        }
    }
    return 0; 
}

int isNum(char *str) {
    char *ptr;
    
    if (str != NULL) {
        strtol(str, &ptr, 10);
        if (*ptr == '\0' || *ptr == ' ') { /* no remainder after strtol*/
            return 1;
        }
    }
    return 0;
}


int is_double(char str[]) {
    char *endptr;
    if (str == NULL || str[0] == '\0') {
        return 0;
    }
    while (isspace((char)*str)) str++;
    if (*str == '\0') {
        return 0;
    }
    strtod(str, &endptr);
    /* checking if there are non whitespace chars that remain after strtod*/
    while (isspace((char)*endptr)) endptr++;
    return *endptr == '\0'; 
}


int what_opcode(char str[]) {
    int i;
    if (str == NULL) {
        return -1;
    }
    /* traverse all possible opcodes  */
    for (i = 0; i < OPCODES_COUNT; i++) {
        if (strcmp(str, OPCODES_ARRAY[i].opcode) == 0) {
            return i; /* return index of the opcode in the array */
        }
    }
    return -1; 
}


int what_reg(char str[]) {
    int i;
    if (str == NULL) {
        return -1;
    }
    /* traverse all possible registers  */
    for (i = 0; i < REGISTER_COUNT; i++) {
        if (strcmp(str, REGISTERS_ARRAY[i]) == 0) {
            return i; /* conveniently return the number of the register  */
        }
    }
    return -1; 
}


int is_valid_macro_name(MacroNode *head ,char name[]){
    int i = 0;
    /* checking all the conditions for a valid macro name */
    if(is_instr(name) || what_opcode(name) >= 0 || what_reg(name) >= 0 ||
    !strcmp(name,"mcro") || !strcmp(name,"mcroend") || searchMacro(head,name) != NULL ||
    strlen(name) > MAX_LABEL_LEN){ 
        return 0;
    }
    if(!isalpha(name[i])){
        return 0;
    }
    while(name[i] != '\0'){
        if(!isalnum(name[i]) && name[i] != '_'){
            return 0;
        }
        i++;
    }
    return 1;
}



Token get_next_token(char *input, int* pos){
    Token token = {TOKEN_LABEL, ""};
    int token_pos = 0;

    skip_whitespace(input, pos);

    if (input[(*pos)] == '\0' ) {
        token.type = TOKEN_EOL; /* end of line*/
        return token;
    }
    if(input[(*pos)] == ','){
        token.type = TOKEN_COMMA;
        token.data[0] = input[(*pos)];
        token.data[1] = '\0';
        (*pos)++;
        return token;
    }
    if (input[(*pos)] == '"'){
        token.data[0] = '"';
        token_pos = 1;
        (*pos)++;
        while(input[(*pos)] != '\0' && input[(*pos)] != '"'){
            token.data[token_pos] = input[(*pos)];
            token_pos++;
            (*pos)++;
        }
        token.data[token_pos] = input[(*pos)];
        if(input[(*pos)] == '"') (*pos)++;
        return token;
    }
    /* copying into token.data its content, until a whitespace or '\0' or ',' */ 
    while(input[(*pos)] != '\0' && !isspace(input[(*pos)])  && input[(*pos)] != ','){
        token.data[token_pos] = input[(*pos)];
        token_pos++;
        (*pos)++;
    }
    token.data[token_pos] = '\0';

    /*classifying the token type */
    if(what_opcode(token.data) != -1){
        token.type = TOKEN_OPCODE;
        return token;
    }
    else if(what_reg(token.data) != -1){
        token.type = TOKEN_REGISTER;
        return token;
    }
    else if(strcmp(token.data, "#") == 0 || strcmp(token.data,"&") == 0){
        token.type = TOKEN_ERROR; 
        return token;
    }
    else if (token.data[0] == '#'){
        char copy[MAX_LINE_LEN];
        int i = 1;
        /* copy the number without the '#'*/
        while (token.data[i] != '\0'){
            copy[i - 1] = token.data[i];
            i++;
        }
        copy[i-1] = '\0';
        strcpy(token.data,copy);
        if(isNum(copy)){
            token.type = TOKEN_COMMAND_NUMBER;
            return token;
        }
        else if(is_double(copy)){
            token.type = TOKEN_DOUBLE;
            return token;
        }
        token.type = TOKEN_ERROR;
        return token;
    }
    else if(token.data[0] == '&'){
        char copy[MAX_LINE_LEN];
        int i = 1;
        /* copy the label without '&'*/
        while (token.data[i] != '\0'){
            copy[i - 1] = token.data[i];
            i++;
        }
        copy[i-1] = '\0';
        strcpy(token.data,copy);
        token.type = TOKEN_AMP_LABEL;
        return token;
    }
    else if(isNum(token.data)){
        token.type = TOKEN_NUMBER;
        return token;
    }
    else if(is_instr(token.data)){
        token.type = TOKEN_INSTRUCTION;
    }
    else if(!strcmp(token.data,"mcro")){
        token.type = TOKEN_MCRO;
    }
    else if(!strcmp(token.data,"mcroend")){
        token.type = TOKEN_MCROEND;
    }
    else{
        token.type = TOKEN_LABEL;
        return token;
    }
    return token;
}



int contains_keyword(char *str,char *keyword) {
    const char *pos = strstr(str, keyword);
    int len = strlen(keyword);

    while (pos) {
        /* 
         * Check if the keyword is a standalone word:
         * - Either at the start of the string or preceded by a space.
         * - Either at the end of the string or followed by a space.
         */
        if ((pos == str || isspace(*(pos - 1))) &&
            (*(pos + len) == '\0' || isspace(*(pos + len)))) {
            return 1;  /* Found valid keyword */
        }
        pos = strstr(pos + 1, keyword);  /* Continue searching */
    }
    return 0;  /* No valid keyword found */
}



int isLabelDecl(char str[], Label* head, Error_Location location, MacroNode *macro_head_node){
    int i, status;
    char str_copy[MAX_LINE_LEN];
    i = 0;
    if(str[strlen(str) - 1] != ':') return -1; /* not a label declaration */
    while(str[i] != ':'){
        str_copy[i] = str[i];
        i++;
    }
    str_copy[i] = '\0';
    status = isLegalLabelName(str_copy,head, macro_head_node);
    if(searchLabel(head,str_copy) != NULL){
        print_infile_error(LABEL_ALREADY_DEFINED,location);
        return 0;
    }
    if (searchMacro(macro_head_node, str_copy) != NULL){
        print_infile_error(MACRO_LABEL_COLLISION,location);
        return 0;
    }
    if(!status) print_infile_error(INVALID_LABEL_NAME,location);
    return status;
}


int isLegalLabelName(char name[], Label* head, MacroNode *macro_head_node){
    int i = 0;
    /* check all the conditions */
    if(is_instr(name) || what_opcode(name) >= 0 || what_reg(name) >= 0 ||
    !strcmp(name,"mcro") || !strcmp(name,"mcroend") || searchLabel(head,name) != NULL ||
    strlen(name) > MAX_LABEL_LEN || searchMacro(macro_head_node, name) != NULL){ 
        return 0;
    }
    if(!isalpha(name[i])){ /* first char must be alphabetic */
        return 0;
    }
    while(name[i] != '\0'){
        if(!isalnum(name[i])){
            return 0;
        }
        i++;
    }
    return 1;
}


int readLabel(char line[], Label** head, Error_Location location, CodeTable* code_table, DataTable* data_table){
    Label* temp;
    int pos, status;
    Token token;
    pos = 0;
    status = 1;
    token = get_next_token(line,&pos);
    temp = createLabel();
    if(temp == NULL) return 0;
    token.data[strlen(token.data) - 1] = '\0'; /* get rid of ':' */
    strcpy(temp->name,token.data);
    token = get_next_token(line,&pos);

    if(token.type == TOKEN_INSTRUCTION){
        if (strcmp(token.data,".entry") == 0 || strcmp(token.data,".extern") == 0){
            free(temp);
            return -1;
        }
        strcpy(temp->type,"data"); /* label of type "data"*/
        temp->IC = (data_table->DC);
        status =  readInstruction(line,location,data_table); /* analyze instruction  */
    }
    else if(token.type == TOKEN_OPCODE){
        strcpy(temp->type,"code"); /* label of type "code"*/
        temp->IC = (code_table->IC);
        status = readCommand(line,location,code_table); /* analyze command  */
    }
    else if(token.type == TOKEN_EOL){
        print_infile_error(MISSING_INFO,location);
        status = 0;
    }
    else{
        print_infile_error(UNRECOGNIZED_COMMAND,location);
        status = 0;
    }
    addLabelToList(head,temp); /* add the label to the label list */
    return status;
}



int readInstruction(char line[], Error_Location location, DataTable* data_table){
    Token token;
    char c;
    int i,pos;
    pos = 0;
    i = 1;
    token = get_next_token(line,&pos);
    while(token.type != TOKEN_INSTRUCTION){
        token = get_next_token(line,&pos);
        if(token.type == TOKEN_EOL) return -1;
    }
    if (strcmp(token.data,".entry") == 0 || strcmp(token.data,".extern") == 0){
        return -1;
    }
    if(strcmp(".data", token.data) == 0){
        token = get_next_token(line,&pos);
        if(token.type == TOKEN_EOL){
            print_infile_error(MISSING_DATA,location); /* .data with no integers to read*/
            return 0;
        }
        while(1){
            if(token.type == TOKEN_COMMA || token.type == TOKEN_EOL){
                print_infile_error(MISSING_DATA,location);
                return 0;
            }
            /* handle errors if not numbers */
            if(!isNum(token.data)) {
                if (is_double(token.data)) print_infile_error(ILLEGAL_DOUBLE,location);
                else print_infile_error(NOT_A_NUMBER,location);
                return 0;
            }
            /* increase data table capacity if needed*/
            if(data_table->size == data_table->capacity){
                data_table->capacity *= 2;
                data_table->data = (int*)realloc(data_table->data, data_table->capacity * sizeof(int));
            }
            /* add number to data table */
            data_table->data[data_table->size] = atoi(token.data);
            data_table->DC++;
            data_table->size++;
            token = get_next_token(line,&pos);
            if(token.type == TOKEN_EOL) break;
            else if(token.type != TOKEN_COMMA){
                print_infile_error(MISSING_COMMA,location);
                return 0;
            }
            token = get_next_token(line,&pos);
        }
    }
    else if(strcmp(".string", token.data) == 0){
        token = get_next_token(line,&pos);
        if(token.type == TOKEN_EOL){
            print_infile_error(MISSING_STRING,location);
            return 0;
        }
        /* string has to be enclosed in "" */
        if(token.data[0] != '"' || token.data[strlen(token.data) - 1] != '"'){
            print_infile_error(STRING_ENCLOSURE,location);
            return 0;
        }
        c = token.data[i];
        while(c != '"'){
            /* increase data table capacity if needed*/
            if(data_table->size == data_table->capacity){
                data_table->capacity *= 2;
                data_table->data = (int*)realloc(data_table->data, data_table->capacity * sizeof(int));
            }
            /* add char to data table */
            data_table->data[data_table->size] = (int)c;
            data_table->DC++;
            data_table->size++;
            i++;
            c = token.data[i];
        }
        /* add '\0' char to data table at the end of the string */
        if(data_table->size == data_table->capacity){
            data_table->capacity *= 2;
            data_table->data = (int*)realloc(data_table->data, data_table->capacity * sizeof(int));
        }
        data_table->data[data_table->size] = 0;
        data_table->DC++;
        data_table->size++;
        token = get_next_token(line,&pos);
        if(token.type != TOKEN_EOL){
            print_infile_error(EXTRA_TEXT,location);
            return 0;
        }
    }
    return 1;
}


int read_entry_extern(char line[], Label** head, Error_Location location){
    Label *temporary;
    Token token, temp;
    int pos = 0;
    temporary = createLabel();
    if(temporary == NULL) return 0;
    token = get_next_token(line,&pos);
    /* look for an instruction */
    while(token.type != TOKEN_INSTRUCTION){
        token = get_next_token(line,&pos);
        if(token.type == TOKEN_EOL){ 
            free (temporary);
            return -1;
        }
    }
    temp = token;
    if(strcmp(token.data, ".entry") != 0 && strcmp(token.data, ".extern") != 0) return -1; /* instruction is not .entry or .extern*/
    token = get_next_token(line,&pos);
    if(expect(TOKEN_LABEL,token,location,0) == 0) return 0;
    /* add extern label to label list */
    if(strcmp(temp.data, ".extern") == 0){
        temporary->IC = 0;
        strcpy(temporary->name,token.data);
        strcpy(temporary->type, "external");
        addLabelToList(head,temporary);
    }
    else free (temporary);
    token = get_next_token(line,&pos);
    if(expect(TOKEN_EOL,token,location,0) == 0) return 0;
    return 1;
}


int readCommand(char line[], Error_Location location, CodeTable* code_table){
    Token token, token_null;
    Command command;
    int pos;
    pos = 0;
    token_null.type = TOKEN_NULL;
    strcpy(token_null.data,"");
    command.totalWords = 1;
    command.line = location.line;
    token = get_next_token(line,&pos);
    while(token.type != TOKEN_OPCODE){
        token = get_next_token(line,&pos);
        if(token.type == TOKEN_EOL){
            print_infile_error(UNRECOGNIZED_COMMAND,location); /* did not find an appropriate opcode*/
            return 0;
        } 
    }
    strcpy(command.opCode,token.data);
    /* parse the arguments according to each opcode */
    if (!strcmp("add",token.data) || !strcmp("mov",token.data) || !strcmp("sub",token.data)){
        token = get_next_token(line,&pos);
        if(expect(TOKEN_COMMAND_NUMBER,token,location,0) == 0 || expect(TOKEN_LABEL,token,location,0) == 0 || expect(TOKEN_REGISTER,token,location,0) == 0) return 0;
        if(expect(TOKEN_COMMAND_NUMBER,token,location,0) == 2 && expect(TOKEN_LABEL,token,location,0) == 2 && expect(TOKEN_REGISTER,token,location,1) == 2) return 0;
        if (token.type == TOKEN_COMMAND_NUMBER || token.type == TOKEN_LABEL) command.totalWords++;
        command.arg1 = token;
        token = get_next_token(line,&pos);
        if (!expect(TOKEN_COMMA,token,location,1)) return 0;
        token = get_next_token(line,&pos);
        if(expect(TOKEN_LABEL,token,location,0) == 0 || expect(TOKEN_REGISTER,token,location,0) == 0) return 0;
        if(expect(TOKEN_LABEL,token,location,0) == 2 && expect(TOKEN_REGISTER,token,location,1) == 2) return 0;
        if (token.type == TOKEN_LABEL) command.totalWords++;
        command.arg2 = token;
        token = get_next_token(line,&pos);
        if (!expect(TOKEN_EOL,token,location,1)) return 0;
        code_table->table[code_table->size] = command;
        code_table->IC+= command.totalWords;
        code_table->size++;
    }
    else if (!strcmp("cmp",token.data)){
        token = get_next_token(line,&pos);
        if(expect(TOKEN_COMMAND_NUMBER,token,location,0) == 0 || expect(TOKEN_LABEL,token,location,0) == 0 || expect(TOKEN_REGISTER,token,location,0) == 0) return 0;
        if(expect(TOKEN_COMMAND_NUMBER,token,location,0) == 2 && expect(TOKEN_LABEL,token,location,0) == 2 && expect(TOKEN_REGISTER,token,location,1) == 2) return 0;
        if (token.type == TOKEN_COMMAND_NUMBER || token.type == TOKEN_LABEL) command.totalWords++;
        command.arg1 = token;
        token = get_next_token(line,&pos);
        if (!expect(TOKEN_COMMA,token,location,1)) return 0;
        token = get_next_token(line,&pos);
        if(expect(TOKEN_COMMAND_NUMBER,token,location,0) == 0 || expect(TOKEN_LABEL,token,location,0) == 0 || expect(TOKEN_REGISTER,token,location,0) == 0) return 0;
        if(expect(TOKEN_COMMAND_NUMBER,token,location,0) == 2 && expect(TOKEN_LABEL,token,location,0) == 2 && expect(TOKEN_REGISTER,token,location,1) == 2) return 0;
        if (token.type == TOKEN_COMMAND_NUMBER || token.type == TOKEN_LABEL) command.totalWords++;
        command.arg2 = token;
        token = get_next_token(line,&pos);
        if (!expect(TOKEN_EOL,token,location,1)) return 0;
        code_table->table[code_table->size] = command;
        code_table->IC+= command.totalWords;
        code_table->size++;
    }
    else if (!strcmp("lea",token.data)){
        token = get_next_token(line,&pos);
        if(expect(TOKEN_LABEL,token,location,0) == 0 ) return 0;
        if(expect(TOKEN_LABEL,token,location,1) == 2) return 0;
        command.totalWords++;
        command.arg1 = token;
        token = get_next_token(line,&pos);
        if (!expect(TOKEN_COMMA,token,location,1)) return 0;
        token = get_next_token(line,&pos);
        if(expect(TOKEN_LABEL,token,location,0) == 0 || expect(TOKEN_REGISTER,token,location,0) == 0) return 0;
        if(expect(TOKEN_LABEL,token,location,0) == 2 && expect(TOKEN_REGISTER,token,location,1) == 2) return 0;
        if (token.type == TOKEN_LABEL) command.totalWords++;
        command.arg2 = token;
        token = get_next_token(line,&pos);
        if (!expect(TOKEN_EOL,token,location,1)) return 0;
        code_table->table[code_table->size] = command;
        code_table->IC+= command.totalWords;
        code_table->size++;
    }
    else if (!strcmp("clr",token.data) || !strcmp("not",token.data) || !strcmp("inc",token.data) || !strcmp("dec",token.data) || !strcmp("red",token.data)){
        token = get_next_token(line,&pos);
        if(expect(TOKEN_LABEL,token,location,0) == 0 || expect(TOKEN_REGISTER,token,location,0) == 0) return 0;
        if(expect(TOKEN_LABEL,token,location,0) == 2 && expect(TOKEN_REGISTER,token,location,1) == 2) return 0;
        if (token.type == TOKEN_LABEL) command.totalWords++;
        command.arg2 = token;
        token = get_next_token(line,&pos);
        if (!expect(TOKEN_EOL,token,location,1)) return 0;
        command.arg1 = token_null;
        code_table->table[code_table->size] = command;
        code_table->IC+= command.totalWords;
        code_table->size++;
    }
    else if (!strcmp("jmp",token.data) || !strcmp("bne",token.data) || !strcmp("jsr",token.data)){
        token = get_next_token(line,&pos);
        if(expect(TOKEN_LABEL,token,location,0) == 0 || expect(TOKEN_AMP_LABEL,token,location,0) == 0) return 0;
        if(expect(TOKEN_LABEL,token,location,0) == 2 && expect(TOKEN_AMP_LABEL,token,location,1) == 2) return 0;
        command.totalWords++;
        command.arg2 = token;
        token = get_next_token(line,&pos);
        if (!expect(TOKEN_EOL,token,location,1)) return 0;
        command.arg1 = token_null;
        code_table->table[code_table->size] = command;
        code_table->IC+= command.totalWords;
        code_table->size++;
    }
    else if (!strcmp("prn",token.data)){
        token = get_next_token(line,&pos);
        if(expect(TOKEN_COMMAND_NUMBER,token,location,0) == 0 || expect(TOKEN_LABEL,token,location,0) == 0 || expect(TOKEN_REGISTER,token,location,0) == 0) return 0;
        if(expect(TOKEN_COMMAND_NUMBER,token,location,0) == 2 && expect(TOKEN_LABEL,token,location,0) == 2 && expect(TOKEN_REGISTER,token,location,1) == 2) return 0;
        if (token.type == TOKEN_COMMAND_NUMBER || token.type == TOKEN_LABEL) command.totalWords++;
        command.arg2 = token;
        token = get_next_token(line,&pos);
        if (!expect(TOKEN_EOL,token,location,1)) return 0;
        command.arg1 = token_null;
        code_table->table[code_table->size] = command;
        code_table->IC+= command.totalWords;
        code_table->size++;
    }
    else if(!strcmp("rts",token.data) || !strcmp("stop",token.data)){
        command.arg1 = token_null;
        command.arg2 = token_null;
        token = get_next_token(line,&pos);
        if (!expect(TOKEN_EOL,token,location,1)) return 0;
        code_table->table[code_table->size] = command;
        code_table->IC+= command.totalWords;
        code_table->size++;
    }
    return 1;
}


int expect(TokenType type, Token token, Error_Location location, int printError){
    if(token.type != type){
        /* handling different error messages based on the expected token vs the actual token */
        if(type == TOKEN_EOL){
            print_infile_error(EXTRA_TEXT,location);
        }
        else if((type == TOKEN_REGISTER || type == TOKEN_COMMAND_NUMBER || type == TOKEN_COMMA || type == TOKEN_AMP_LABEL || type == TOKEN_LABEL) && token.type == TOKEN_EOL){
            print_infile_error(MISSING_ARG,location);
        }
        else if((type == TOKEN_REGISTER || type == TOKEN_COMMAND_NUMBER || type == TOKEN_COMMA || type == TOKEN_AMP_LABEL || type == TOKEN_LABEL) && token.type == TOKEN_COMMA){
            print_infile_error(ILLEGAL_COMMA,location);
        }
        else if(type == TOKEN_OPCODE && token.type == TOKEN_COMMA){
            print_infile_error(ILLEGAL_COMMA,location);
        }
        else if(type == TOKEN_OPCODE){
            print_infile_error(UNRECOGNIZED_COMMAND, location);
        }
        else if(type == TOKEN_COMMAND_NUMBER && token.type == TOKEN_NUMBER){
            print_infile_error(MISSING_NET,location);
        }
        else if(type == TOKEN_COMMA){
            print_infile_error(MISSING_COMMA,location);
        }
        else if(type == TOKEN_COMMAND_NUMBER && token.type == TOKEN_DOUBLE){
            print_infile_error(ILLEGAL_DOUBLE,location);
        }
        else{
            /* at this point the only possible mismatch is between the types: TOKEN_REGISTER, TOKEN_COMMAND_NUMBER, 
            TOKEN_AMP_LABEL,  TOKEN_LABEL. for some opcodes there are several possible argument types */
            if (printError) print_infile_error(ILLEGAL_ARG,location);
            return 2;
        }
        return 0;
    }

    return 1;
}

