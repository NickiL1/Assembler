#include "../Header_Files /globals.h"
#include "../Header_Files /lexer.h"
#include "../Header_Files /secondpass.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int firstPass(char* as_file_name){
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

    code_table = (CodeTable*)malloc(1 * sizeof(CodeTable));
    code_table->table = (Command*)malloc(100 * sizeof(Command));
    code_table->capacity = 100;

    data_table = (DataTable*)malloc(1 * sizeof(DataTable));
    data_table->data = (int*)malloc(20 * sizeof(int));
    data_table->capacity = 20;

    strcpy(am_file_name,as_file_name);
    am_file_name[strlen(am_file_name) - 1] = 'm';
    strcpy(location.filename,am_file_name);
    location.line = 0;
    code_table->size = 0;
    data_table->size = 0;

    code_table->IC = INIT_IC_VAL;
    data_table->DC = INIT_DC_VAL;
    head = NULL;
    

    am_file = fopen(am_file_name,"r");
    if(am_file == NULL){
        print_file_related_error(FILE_HANDLE,location.filename);
        return 0;
    }

    while(fgets(line,sizeof(line),am_file)){
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
        success_status = isLabelDecl(token.data,head,location);
        if( success_status == 1){
           success_status = readLabel(line,&head,location,code_table,data_table);
        }
        else if (success_status == -1){
            success_status = read_entry_extern(line,&head,location);  
            if (success_status == -1) success_status = readInstruction(line,location,data_table);
            if (success_status == -1) success_status = readCommand(line,location,code_table);
        }
        if(error_raised == 0) error_raised = !success_status;
    }
    ICF = code_table->IC;
    DCF = data_table->DC;
    temp = head;

    if (ICF - 100 + DCF > MAX_WORDS){
        print_file_related_error(MEM_LIM,am_file_name);
        error_raised = 1;
    }
    while(temp != NULL){
        if (strcmp(temp->type, "data") == 0) temp->IC += ICF;
        temp = temp->next;
    }
        
    return secondPass(am_file_name, am_file,code_table,data_table,head);
}




