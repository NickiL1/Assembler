#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Header_Files /datastrct.h"
#include "../Header_Files /errors.h"


void freeArray(DynamicStringArray *array) {
    int i;
    for (i = 0; i < array->size; i++) {
        if(array->data[i] != NULL) free(array->data[i]);
    }
    free(array->data);
    array->size = 0;
    array->used = 0;
}



int initArray(DynamicStringArray *array){
    int i;
    array->data = (char**)malloc(DEF_ARR_SIZE * sizeof(char*));
    if(array->data == NULL){
        print_sys_error(MEMORY_ALOC);
        return 0;
    }
    array->size = DEF_ARR_SIZE;
    array->used = 0;

    for(i = 0; i < DEF_ARR_SIZE;i++){
        array->data[i] = (char*)malloc(MAX_LINE_LEN * sizeof(char));
        if(array->data[i] == NULL){
            print_sys_error(MEMORY_ALOC);
            freeArray(array);
            return 0;
        }
    }
    return 1;
}



int addString(DynamicStringArray* array, char string[]){
    int i;
    char** newData;
    if(array->size == array->used){
        newData = (char**)realloc(array->data, array->size * 2 * sizeof(char*));     
        if(newData == NULL){
        print_sys_error(MEMORY_ALOC);
        freeArray(array);
        return 0;
        }
        array->size *= 2;
        array->data = newData;
        for(i = array->used; i < array->size;i++){
            array->data[i] = (char*)malloc(MAX_LINE_LEN * sizeof(char));
            if(array->data[i] == NULL){
            print_sys_error(MEMORY_ALOC);
            freeArray(array);
            return 0;
            }
        }
    }

    strcpy(array->data[array->used], string);
    array->used++;
    return 1;
    
}


void printArray(DynamicStringArray* array){
    int i;
    if(array == NULL){
        printf("null array\n");
        return;
    }
    for(i = 0; i < array->used; i++){
        printf("%s\n", array->data[i]);
    }
}




MacroNode* createMacro(char name[]){
    MacroNode* newNode = (MacroNode*)malloc(sizeof(MacroNode));
    if(newNode == NULL){
        print_sys_error(MEMORY_ALOC);
        return NULL;
    } 
    strcpy(newNode->name,name);
    if(initArray(&(newNode->macroContent)) == 0){
        freeMacroNode(newNode);
        return NULL;
    }
    newNode->next = NULL;
    return newNode;
}


int addMacroToList(MacroNode **head,char macroName[]){
    MacroNode* newNode = createMacro(macroName);
    if(newNode == NULL) return 0;
    if(*head == NULL){
        *head = newNode;
        return 1;
    }
    newNode->next = *head;
    *head = newNode;
    return 1;
}

MacroNode* searchMacro(MacroNode *head, char name[]){
    MacroNode* temp = head;
    if(head == NULL) return NULL;
    while(temp!= NULL){
        if(strcmp(name,temp->name) == 0) return temp;
        temp = temp->next;
    }
    return NULL;
}


void freeMacroNode(MacroNode* node){
    if(node == NULL) return;
    freeArray(&(node->macroContent));
    free(node);
}

void freeMacroList(MacroNode *head){
    MacroNode *current, *next;
    current = head;
    while(current != NULL){
        next = current->next;
        freeMacroNode(current);
        current = next;
    }
}

