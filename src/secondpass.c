/*  
    @author- Nick Levin 326737046.

    the file implements the second pass of the assembler. the goal of the second pass is to translate 
    all the stored commands to hex base and create the .ob, .ent and .ext files. 
*/


#include "../Header_Files /lexer.h"
#include "../Header_Files /secondpass.h"
#include "../Header_Files /preassem.h"
#include "../Header_Files /util.h"
#include <string.h>

/* Define the opcodes */
OpCode OPCODES_ARRAY_2[] = {
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
char *REGISTERS_ARRAY_2[] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"};

/* Define the instructions */
char *INSTRUCTIONS_ARRAY_2[] = {".data", ".string", ".extern", ".entry"};

void secondPass(char* am_file_name, FILE* am_file, CodeTable* code_table, DataTable* data_table, Label* head, int prevError, MacroNode *macro_head_node){
    Error_Location location;
    int pos, status, error_raised;
    char line[MAX_LINE_LEN];
    char ent_file_name[MAX_LINE_LEN], ext_file_name[MAX_LINE_LEN], ob_file_name[MAX_LINE_LEN];
    Token token;
    strcpy(location.filename,am_file_name);
    location.line = 0;
    status = 1;
    error_raised = prevError;
    /* create the file names */
    create_file_name(am_file_name,".ent",ent_file_name);
    create_file_name(am_file_name,".ob", ob_file_name);
    create_file_name(am_file_name,".ext", ext_file_name);


    rewind(am_file);
    while (fgets(line,sizeof(line),am_file)){
        location.line++;
        pos = 0;
        token = get_next_token(line,&pos);
        while(strcmp(token.data, ".entry") != 0){
            token = get_next_token(line,&pos);
            if (token.type == TOKEN_EOL) break;
        }
        if(strcmp(token.data, ".entry") == 0){
            token = get_next_token(line,&pos);
            status = add_entry(token.data,head,location); /* add entry label to the list */
        }
        if (error_raised == 0) error_raised = !status;
    }
    status = create_ent_file(ent_file_name,head); /* create the .ent file */
    if (status == 0) error_raised = 1;
    status = create_ob_ext_files(am_file_name,ob_file_name,ext_file_name,head,code_table,data_table); /* create .ob file*/
    if (status == 0) error_raised = 1;
    if (error_raised){ 
        remove(ob_file_name);
        remove(ent_file_name);
        remove(ext_file_name);
    }
    free_all_memory(code_table,data_table,head, macro_head_node);
}


int add_entry(char label[], Label* head, Error_Location location){
    Label* temp;
    char entry[] = ", entry";
    temp = searchLabel(head,label);
    if (temp == NULL){
        print_infile_error(UNDEFINED_LABEL,location);
        return 0;
    }
    if (strcmp(temp->type,"external") == 0){ /* same label cant be extern and entry simultaneously */
        print_infile_error(LABEL_CONFLICT,location);
        return 0;
    }
    strcat(temp->type, entry);
    return 1;
}


int create_ent_file(char ent_file_name[], Label* head){
    Label* temp;
    long size;
    FILE* ent_file;
    ent_file = fopen(ent_file_name,"w");
    if(ent_file == NULL){
        print_file_related_error(CREATE_FILE,ent_file_name);
        return 0;
    }
    temp = head;
    while(temp != NULL){
        /* if label is entry, then add it to the .ent file */
        if (contains_keyword(temp->type, "entry")) fprintf(ent_file,"%s %07d\n", temp->name, temp->IC);
        temp = temp->next;
    }
    size = ftell(ent_file);
    fclose(ent_file);
    if (size <= 0){ /* if .ent file is empty, no entry labels were found */
        remove(ent_file_name);
    }
    return 1;
}


int create_ob_ext_files(char* am_file_name, char* ob_file_name, char* ext_file_name, Label* head, 
                        CodeTable* code_table, DataTable* data_table){
    FILE *ob_file, *ext_file;
    long size;
    int j, status, IC, error;
    Command temp_command;
    Error_Location location;
    strcpy(location.filename,am_file_name);
    IC = INIT_IC_VAL;
    status = 1;
    error = 0;

    ob_file = fopen(ob_file_name,"w");
    if (ob_file == NULL){
        print_file_related_error(CREATE_FILE,ob_file_name);
        return 0;
    }

    ext_file = fopen(ext_file_name,"w");
    if (ext_file == NULL){
        print_file_related_error(CREATE_FILE,ext_file_name);
        return 0;
    }
    /* write the size of the code table and data table*/
    fprintf(ob_file,"     %d %d\n",code_table->IC - 100, data_table->DC);
    /* write each command to the .ob file, and if needed also to the .ext file */
    for (j = 0; j < code_table->size; j++){
        temp_command = code_table->table[j];
        location.line = temp_command.line;
        status = write_ob_ext(temp_command,&IC,head,ob_file,ext_file,location);
        if (status == 0) error = 1;
    }
    /* write the data table to the .ob file */
    for(j = 0; j < data_table->size; j++){
        fprintf(ob_file,"%07d %06x\n", IC, data_table->data[j] & 0xFFFFFF);
        IC++;
    }
    size = ftell(ext_file);
    fclose(ob_file);
    fclose(ext_file);
    if (size <= 0) remove(ext_file_name);
    return !error;
}


int write_ob_ext(Command command, int *IC, Label* head, FILE* ob_file, FILE* ext_file, Error_Location location){
    char opcode[5];
    Token arg1, arg2;
    int word, temp, opcode_index, status, error;
    strcpy(opcode, command.opCode);
    arg1 = command.arg1;
    arg2 = command.arg2;
    word = 0;
    status = 1;
    error = 0;

    /*add opcode to first word*/
    opcode_index = what_opcode(opcode);
    temp = OPCODES_ARRAY_2[opcode_index].code;
    word = word | (temp << 18);
    /*add funct to first word*/
    temp = OPCODES_ARRAY_2[opcode_index].funct;
    word = word | (temp << 3);
    /* A = 1, R = E = 0*/
    temp = 1;
    word = word | (temp << 2);

    /*parse destination arguments if they are registers*/
    if (arg1.type == TOKEN_REGISTER){
        temp = what_reg(arg1.data);
        word = word | (temp << 13);
        temp = 3; /* register argument*/
        word = word | (temp << 16);
    } 
    if (arg2.type == TOKEN_REGISTER){
        temp = what_reg(arg2.data);
        word = word | (temp << 8);
        temp = 3; /* register argument*/
        word = word | (temp << 11);
    }
    /* parse destination and source args for the first word if they are not registers*/
    if (arg1.type == TOKEN_LABEL) word = word | (1 << 16);
    else if(arg1.type == TOKEN_AMP_LABEL) word = word | (2 << 16);
    if (arg2.type == TOKEN_LABEL) word = word | (1 << 11);
    else if(arg2.type == TOKEN_AMP_LABEL) word = word | (2 << 11);
    /* write the first word of the command*/
    fprintf(ob_file,"%07d %06x\n", (*IC), word);
    (*IC)++;
    /* write additional words if needed*/
    status = parse_arg1(IC,head,ob_file, ext_file,location,arg1);
    if (status == 0) error = 1;
    status = parse_arg2(IC,head,ob_file,ext_file,location,arg2);
    if (status == 0) error = 1;
    return !error;
}


int parse_arg1(int *IC, Label* head, FILE* ob_file, FILE* ext_file, Error_Location location, Token arg1){
    int word;
    Label* temp_label;
    if (arg1.type == TOKEN_COMMAND_NUMBER){
        word = (atoi(arg1.data)) << 3;
        word = word | (1 << 2); /* A = 1, R = E = 0*/
        fprintf(ob_file,"%07d %06x\n", (*IC), word & 0xFFFFFF);
        (*IC)++;
    }
    else if (arg1.type == TOKEN_LABEL){
        /* make sure the label is in the label list*/
        temp_label = searchLabel(head, arg1.data);
        if (temp_label == NULL){
            print_infile_error(UNDEFINED_LABEL, location);
            return 0;
        }
        word = (temp_label->IC) << 3;
        if (strcmp(temp_label->type,"external") == 0) {
            word = word | 1 ;/* A = 0, R = 0, E = 1*/
            fprintf(ext_file,"%s %07d\n", temp_label->name, (*IC));
        }
        else word = word | (1 << 1); /* A = 0, R = 1, E = 0*/
        fprintf(ob_file,"%07d %06x\n", (*IC), word);
        (*IC)++;
    }
    else if(arg1.type == TOKEN_AMP_LABEL){
        temp_label = searchLabel(head, arg1.data);
        if (temp_label == NULL){
            print_infile_error(UNDEFINED_LABEL, location);
            return 0;
        }
        /* cant use &label if label is of type external*/
        if (strcmp(temp_label->type,"external") == 0){
            print_infile_error(ILLEGAL_LABEL_USE,location);
            return 0;
        }
        word = (temp_label->IC - (*IC) + 1) << 3; /* calculate distance to label */
        word = word | (1 << 2); /* A = 1, R = 0, E = 0*/
        fprintf(ob_file,"%07d %06x\n", (*IC), word & 0xFFFFFF);
        (*IC)++;
    }
    return 1;
}


int parse_arg2(int *IC, Label* head, FILE* ob_file, FILE* ext_file, Error_Location location, Token arg2){
    int word;
    Label* temp_label;
    if (arg2.type == TOKEN_COMMAND_NUMBER){
        word = (atoi(arg2.data)) << 3;
        word = word | (1 << 2); /* A = 1, R = E = 0*/
        fprintf(ob_file,"%07d %06x\n", (*IC), word & 0xFFFFFF);
        (*IC)++;
    }
    else if (arg2.type == TOKEN_LABEL){
        temp_label = searchLabel(head, arg2.data);
        if (temp_label == NULL){
            print_infile_error(UNDEFINED_LABEL, location);
            return 0;
        }
        word = (temp_label->IC) << 3;
        if (strcmp(temp_label->type,"external") == 0){
            word = word | 1 ;/* A = 0, R = 0, E = 1*/
            fprintf(ext_file,"%s %07d\n", temp_label->name, (*IC));
        }
        else word = word | (1 << 1); /* A = 0, R = 1, E = 0*/
        fprintf(ob_file,"%07d %06x\n", (*IC), word);
        (*IC)++;
    }
    else if(arg2.type == TOKEN_AMP_LABEL){
        temp_label = searchLabel(head, arg2.data);
        if (temp_label == NULL){
            print_infile_error(UNDEFINED_LABEL, location);
            return 0;
        }
        if (strcmp(temp_label->type,"external") == 0){
            print_infile_error(ILLEGAL_LABEL_USE,location);
            return 0;
        }
        word = (temp_label->IC - (*IC) + 1) << 3;
        word = word | (1 << 2); /* A = 1, R = 0, E = 0*/
        fprintf(ob_file,"%07d %06x\n", (*IC), word & 0xFFFFFF);
        (*IC)++;
    }
    return 1;
}
