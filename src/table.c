#include "../Header_Files /table.h"
#include "../Header_Files /errors.h"
#include <stdlib.h>
#include <string.h>

void addLabelToList(Label** head, Label* label){
    Label* temp = *head;
    if(*head == NULL){
        *head = label;
        label->next = NULL;
        return;
    }
    while(temp->next != NULL) temp = temp->next;
    temp->next = label;
    label->next = NULL;
}


Label* searchLabel(Label* head, char name[]){
    Label* temp = head;
    if(head == NULL) return NULL;
    while(temp != NULL){
        if(strcmp(name,temp->name) == 0) return temp;
        temp = temp->next;
    }
    return NULL;
}


void freeLabelList(Label* head){
    Label *current, *next;
    current = head;
    while(current != NULL){
        next = current->next;
        free(current);
        current = next;
    }
}

Label* createLabel(){
    Label* newLabel = (Label*)malloc(sizeof(Label));
    if(newLabel == NULL){ 
        print_sys_error(MEMORY_ALOC);
        return NULL;
    }
    return newLabel;
}
