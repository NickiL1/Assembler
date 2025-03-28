/*
    Author - Nick Levin 326737046

    the main purpose of this file is to implement the dynamic list of 
    macros and their content. 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Header_Files /datastrct.h"
#include "../Header_Files /errors.h"


void freeArray(DynamicStringArray *array) {
    int i;
    /* Free each allocated string in the array */
    for (i = 0; i < array->size; i++) {
        if(array->data[i] != NULL) free(array->data[i]);
    }
    /* Free the array of string pointers itself */
    free(array->data);
    /* Reset the size and used counters */
    array->size = 0;
    array->used = 0;
}


int initArray(DynamicStringArray *array){
    int i;
    /* Allocate memory for an array of character pointers */
    array->data = (char**)malloc(DEF_ARR_SIZE * sizeof(char*)); 
    if(array->data == NULL){
        print_sys_error(MEMORY_ALOC);
        return 0;
    }
    array->size = DEF_ARR_SIZE;
    array->used = 0;

    /* Allocate memory for each string in the array */
    for(i = 0; i < DEF_ARR_SIZE; i++){
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
    /* Check if the array needs to be resized */
    if(array->size == array->used){
        newData = (char**)realloc(array->data, array->size * 2 * sizeof(char*));     
        if(newData == NULL){
            print_sys_error(MEMORY_ALOC);
            freeArray(array);
            return 0;
        }
        array->size *= 2;
        array->data = newData;
        /* Allocate memory for the new elements in the resized array */
        for(i = array->used; i < array->size; i++){
            array->data[i] = (char*)malloc(MAX_LINE_LEN * sizeof(char));
            if(array->data[i] == NULL){
                print_sys_error(MEMORY_ALOC);
                freeArray(array);
                return 0;
            }
        }
    }
    /* Copy the string to the array */
    strcpy(array->data[array->used], string);
    array->used++;
    return 1;
}


MacroNode* createMacro(char name[]){
    /* Allocate memory for a new macro node */
    MacroNode* newNode = (MacroNode*)malloc(sizeof(MacroNode));
    if(newNode == NULL){
        print_sys_error(MEMORY_ALOC);
        return NULL;
    } 
    /* Copy the macro name to the node */
    strcpy(newNode->name, name);
    /* Initialize the macro content array */
    if(initArray(&(newNode->macroContent)) == 0){
        freeMacroNode(newNode);
        return NULL;
    }
    newNode->next = NULL;
    return newNode;
}


int addMacroToList(MacroNode **head, char macroName[]){
    /* Create a new macro node */
    MacroNode* newNode = createMacro(macroName);
    if(newNode == NULL) return 0;
    /* If the list is empty, set the new node as the head */
    if(*head == NULL){
        *head = newNode;
        return 1;
    }
    /* Insert the new node at the beginning of the list */
    newNode->next = *head;
    *head = newNode;
    return 1;
}

MacroNode* searchMacro(MacroNode *head, char name[]){
    MacroNode* temp = head;
    /* If the list is empty, return NULL */
    if(head == NULL) return NULL;
    /* Traverse the list to find the macro by name */
    while(temp != NULL){
        if(strcmp(name, temp->name) == 0) return temp;
        temp = temp->next;
    }
    return NULL;
}


void freeMacroNode(MacroNode* node){
    /* If node is NULL, return immediately */
    if(node == NULL) return;
    /* Free the dynamically allocated array inside the macro node */
    freeArray(&(node->macroContent));
    /* Free the macro node itself */
    free(node);
}

void freeMacroList(MacroNode *head){
    MacroNode *current, *next;
    current = head;
    /* Traverse and free each node in the linked list */
    while(current != NULL){
        next = current->next;
        freeMacroNode(current);
        current = next;
    }
}
