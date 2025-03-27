#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../Header_Files /globals.h"
#include "../Header_Files /preassem.h"
#include "../Header_Files /util.h"



int pre_assembler(char* as_file_name){
    FILE *as_file;
    int res;
    as_file = fopen(as_file_name,"r");
    if(as_file == NULL){
        print_file_related_error(FILE_HANDLE,as_file_name);
        fclose(as_file);
        return 0;
    }
    res = create_am_file(as_file,as_file_name);
    fclose(as_file);
    return res;
}



int create_am_file(FILE* as_file, char* as_file_name){
    char am_file_name[MAX_LINE_LEN];
    FILE* am_file;
    MacroNode *head;
    int status;

    head = NULL;
    create_file(as_file_name,".am",am_file_name); 
    
    am_file = fopen(am_file_name,"w+");
    if(am_file == NULL){
        print_file_related_error(CREATE_FILE,am_file_name);
        return 0;
    }

    status = addAllMacros(as_file,as_file_name,&head);
    if(status) status = expandMacros(as_file,as_file_name ,am_file,head);
    if(status) status = removeMacroDecl(am_file,am_file_name);
    freeMacroList(head);
    if(status == 0) remove(am_file_name);
    fclose(am_file);
    return status;
}



int addAllMacros(FILE* as_file, char* as_file_name, MacroNode** head){
    Token token;
    int pos, status, macroFlag;
    char line[MAX_LINE_LEN];
    Error_Location location;
    location.line = 0;
    strcpy(location.filename,as_file_name);
    status = 1;
    macroFlag = 0;

    while(fgets(line,sizeof(line),as_file)){
        location.line++;
        if(strlen(line) > 0 && line[strlen(line) - 1] == '\n'){
            line[strlen(line) - 1] = '\0';
        }
        if(is_comment_line(line) || is_empty_line(line)) continue;
        pos = 0;
        token = get_next_token(line,&pos);
        
        if(token.type == TOKEN_MCRO){
            token = get_next_token(line,&pos);
            if(token.type == TOKEN_EOL){
                print_infile_error(MISSING_ARG,location);
                status = 0;
                continue;
            }
            if(is_valid_macro_name(*head,token.data) == 0){
                print_infile_error(INVALID_MACRO_NAME,location);
                status = 0;
                continue;
            }
            if(addMacroToList(head,token.data) == 0) return 0;
            token = get_next_token(line,&pos);
            if(token.type != TOKEN_EOL){
                print_infile_error(EXTRA_TEXT,location);
                status = 0;
                continue;
            }
            macroFlag = 1;

        }
        else if(token.type == TOKEN_ENDMCRO){
            macroFlag = 0;
            token = get_next_token(line,&pos);
            if(token.type != TOKEN_EOL){
                print_infile_error(EXTRA_TEXT,location);
                status = 0;
                continue;
            }
        }
        else if(macroFlag == 1){
            if(addString(&((*head)->macroContent), line) == 0) return 0;
        }
    }
    return status;
}



int expandMacros(FILE* as_file,char* as_file_name, FILE* am_file, MacroNode* head){
    MacroNode* node;
    Token token;
    int pos, i, status;
    char line[MAX_LINE_LEN];
    Error_Location location;
    strcpy(location.filename,as_file_name);
    location.line = 0;

    rewind(as_file);
    status = 1;
    while(fgets(line,sizeof(line),as_file)){
        location.line++;
        if(strlen(line) > 0 && line[strlen(line) - 1] == '\n'){
            line[strlen(line) - 1] = '\0';
        }

        if(is_comment_line(line) || is_empty_line(line)) continue;

        pos = 0;
        token = get_next_token(line,&pos);
        node = searchMacro(head,token.data);
        if(node == NULL) fprintf(am_file,"%s\n", line);
        else{
            token = get_next_token(line,&pos);
            if(TOKEN_EOL != token.type){ 
                print_infile_error(EXTRA_TEXT, location);
                status = 0;
            }
            for(i = 0; i < node->macroContent.used; i++){
                fprintf(am_file,"%s\n",node->macroContent.data[i]);
            }
        }
    } 
    return status;
}



int removeMacroDecl(FILE* am_file, char* am_file_name){
    FILE* temp_file;
    char line[MAX_LINE_LEN];
    int pos, macroFlag;
    Token token;
    macroFlag = 0;

    temp_file = fopen("tempFile.txt","w+");
    if(temp_file == NULL){
        print_file_related_error(FILE_HANDLE,"tempFile.txt");
        return 0;
    }
    rewind(am_file);
    copyFile(am_file,temp_file);
    am_file = freopen(am_file_name,"w",am_file);
    rewind(temp_file);

    while(fgets(line,sizeof(line),temp_file)){
        if(strlen(line) > 0 && line[strlen(line) - 1] == '\n'){
            line[strlen(line) - 1] = '\0';
        }
        pos = 0;
        token = get_next_token(line,&pos);
        if(token.type == TOKEN_MCRO) macroFlag = 1;
        else if(token.type == TOKEN_ENDMCRO) macroFlag = 0;
        else if(macroFlag == 1) continue;
        else fprintf(am_file,"%s\n",line);
    }
    while(fgets(line,sizeof(line),am_file));
    fclose(temp_file);
    remove("tempFile.txt");
    return 1;
}
