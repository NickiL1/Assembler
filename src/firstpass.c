/* 
    @author - Nick Levin 326737046

    this file implements the first pass of the assembler. 
    note - in this implementation of the first pass, we only check and store 
    the commands, instructions, and labels, without translating them to machine code - this task is managed by 
    the second pass. 
*/

#include "../Header_Files /globals.h"
#include "../Header_Files /lexer.h"
#include "../Header_Files /secondpass.h"
#include "../Header_Files /preassem.h"
#include "../Header_Files /util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void firstPass(char* as_file_name, MacroNode *macro_head_node){
    /* declaring and initializing some variables which will be used in the first and second pass.*/
    FILE* am_file;
    Error_Location location;
    int success_status, pos, ICF, DCF, error_raised;
    Label *head, *temp;
    Token token;
    CodeTable* code_table;
    DataTable* data_table;
    char line[MAX_LINE_LEN], am_file_name[MAX_LINE_LEN];
    success_status = 1;
    error_raised = 0;

    /* this is the code table that will store all the commands that are not instructions*/
    code_table = (CodeTable*)malloc(1 * sizeof(CodeTable));
    code_table->table = (Command*)malloc(100 * sizeof(Command));
    code_table->capacity = 100;

    /* this is the data table that will store all the data image*/
    data_table = (DataTable*)malloc(1 * sizeof(DataTable));
    data_table->data = (int*)malloc(20 * sizeof(int));
    data_table->capacity = 20;

    create_file_name(as_file_name,".am",am_file_name); 
    strcpy(location.filename,am_file_name);
    location.line = 0;
    code_table->size = 0;
    data_table->size = 0;

    code_table->IC = INIT_IC_VAL;
    data_table->DC = INIT_DC_VAL;
    head = NULL;
    

    am_file = fopen(am_file_name,"r");
    if(am_file == NULL){
        /* if file could not be opened then quit first pass and free all memory.*/
        print_file_related_error(FILE_HANDLE,location.filename);
        free_all_memory(code_table,data_table,head, macro_head_node);
        return;
    }

    while(fgets(line,sizeof(line),am_file)){
        /* basic size reallocation in case of code table being possibly too small.*/
        if((code_table->size >= code_table->capacity - 3 )){
            code_table->capacity *= 2;
            code_table->table = (Command*)realloc(code_table->table,code_table->capacity * sizeof(Command));
        }
        if(strlen(line) > 0 && line[strlen(line) - 1] == '\n'){
            line[strlen(line) - 1] = '\0';
        }
        location.line++;
        pos = 0;
        token = get_next_token(line,&pos);
        success_status = isLabelDecl(token.data,head,location, macro_head_node);
        if( success_status == 1){ /* success status 1 means successful label declaration */
            /* store the label and read the command or instruction that are after the label */
           success_status = readLabel(line,&head,location,code_table,data_table); 
        }
        if (success_status == -1){ /* success status -1 means no label was found or label was found with .entry or .extern after */
            /* try to find entry or external */
            success_status = read_entry_extern(line,&head,location);  
            /* if sis not find entry or external, try finding a .data or .string instruction */
            if (success_status == -1) success_status = readInstruction(line,location,data_table);
            /* if did not find instruction, try finding a command */
            if (success_status == -1) success_status = readCommand(line,location,code_table);
        }
        /* success status 0 means the current line had an error(that was found by read_entry_extern or 
        readInstruction or readCommand)*/
        if(success_status == 0) error_raised = 1; 
    }
    ICF = code_table->IC;
    DCF = data_table->DC;

    /* raise an error if the total amount of words in the program exceed the amount of allocated memory by the 
    assembler */
    if (ICF - 100 + DCF > MAX_WORDS){
        print_file_related_error(MEM_LIM,am_file_name);
        error_raised = 1;
    }

    /* for each label with type "data" add ICF to its addrress */
    temp = head;
    while(temp != NULL){
        if (strcmp(temp->type, "data") == 0) temp->IC += ICF;
        temp = temp->next;
    }
    /* start second pass */
    secondPass(am_file_name, am_file,code_table,data_table,head, error_raised, macro_head_node);
}




