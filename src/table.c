/*
    @author - Nick Levin 326737046 

    this file implements the label list.
*/


#include "../Header_Files /table.h"
#include "../Header_Files /errors.h"
#include <stdlib.h>
#include <string.h>

void addLabelToList(Label** head, Label* label){
    Label* temp = *head;
    if(*head == NULL){ /* if list is empty make the label node the head of the list */
        *head = label;
        label->next = NULL;
        return;
    }
    while(temp->next != NULL) temp = temp->next; /* go over the list until found last node */
    temp->next = label;
    label->next = NULL;
}


Label* searchLabel(Label* head, char name[]){
    Label* temp = head;
    if(head == NULL) return NULL; /* list is empty */
    /* check each node in the list */
    while(temp != NULL){
        if(strcmp(name,temp->name) == 0) return temp;
        temp = temp->next;
    }
    return NULL;
}


void freeLabelList(Label* head){
    Label *current, *next;
    current = head;
    /* free each node in the list */
    while(current != NULL){
        next = current->next;
        free(current);
        current = next;
    }
}

Label* createLabel(void){
    Label* newLabel = (Label*)malloc(sizeof(Label));
    if(newLabel == NULL){ 
        print_sys_error(MEMORY_ALOC);
        return NULL;
    }
    return newLabel;
}
