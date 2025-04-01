#ifndef TABLE_H
#define TABLE_H
#include "../Header_Files /globals.h"

typedef struct Label{
    char name[MAX_LABEL_LEN]; /* label name */
    int IC; /* the labels address */
    char type[MAX_LABEL_LEN]; /* code, data, external or entry */
    struct Label* next; /* next label in the list */
} Label;


/*
    the function adds a label node to the list of labels. 

    @param head - a double pointer to the head of the list. 
    @param label - a pointer to the node to add to the list. 
*/
void addLabelToList(Label** head, Label* label);


/* 
    the function searches the label list for a label node given the labels name. 

    @param head - a pointer to the head of the list.
    @param name - the name of the label. 

    @return a pointer to the node if it was found in the list, NULL if label was not 
    found in the list. 
*/
Label* searchLabel(Label* head, char name[]);


/* 
    the function frees all the dynamically allocated memory of a label list. 

    @param head - a pointer to the head of the list. 
*/
void freeLabelList(Label* head);


/* 
    the function creates, and allocates memory for a new standalone label node. 

    @return a pointer to the created node, NULL if memory could not be allocated for the node. 
*/
Label* createLabel(void);


#endif
