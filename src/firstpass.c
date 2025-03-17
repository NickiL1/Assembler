#include "../Header_Files /globals.h"
#include "../Header_Files /lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int firstPass(char* as_file_name){
    FILE* am_file;
    Error_Location location;
    int success_status, pos, ICF, DCF;
    Label *head, *temp;
    Token token;
    CodeTable* code_table;
    DataTable data_table;
    char line[MAX_LINE_LEN], am_file_name[MAX_LINE_LEN];
    success_status = 1;

    code_table = (CodeTable*)malloc(1 * sizeof(CodeTable));
    strcpy(am_file_name,as_file_name);
    am_file_name[strlen(am_file_name) - 1] = 'm';
    strcpy(location.filename,am_file_name);
    location.line = 0;
    code_table->size = 0;
    data_table.size = 0;

    code_table->IC = INIT_IC_VAL;
    data_table.DC = INIT_DC_VAL;
    head = NULL;
    

    am_file = fopen(am_file_name,"r");
    if(am_file == NULL){
        print_file_related_error(FILE_HANDLE,location.filename);
        return 0;
    }

    while(fgets(line,sizeof(line),am_file)){
        if(strlen(line) > 0 && line[strlen(line) - 1] == '\n'){
            line[strlen(line) - 1] = '\0';
        }
        location.line++;
        pos = 0;
        token = get_next_token(line,&pos);
        if(isLabelDecl(token.data,head,location)){
           success_status = readLabel(line,&head,location,code_table,&data_table);
        }
        else{
            success_status = read_entry_extern(line,&head,location);  
            if (success_status == -1) success_status = readInstruction(line,location,&data_table);
            if (success_status == -1) success_status = readCommand(line,location,code_table);
        }
    }
    ICF = code_table->IC;
    DCF = data_table.DC;
    temp = head;
    while(temp != NULL){
        if (strcmp(temp->type, "data") == 0) temp->IC += ICF;
        temp = temp->next;
    }

    // for(int i = 0; i < data_table.size; i++){
    //     printf("%d DC: %d\n",data_table.data[i], data_table.DC);
    // }
    while (head != NULL){
        printf("|%s|, %d %s \n", head->name, head->IC,head->type);
        head = head->next;
    }
    
    // for ( int j = 0; j < code_table->size; j++)
    // {
    //     printf("opcode: %s, arg1: %s, arg2: %s, IC: %d\n",code_table->table[j].opCode, code_table->table[j].arg1, code_table->table[j].arg2, code_table->IC);
    // }
    
    fclose(am_file);
    free(code_table);
    freeLabelList(head);
    return success_status;
}



