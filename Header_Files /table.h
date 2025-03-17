#ifndef TABLE_H
#define TABLE_H
#include "../Header_Files /globals.h"

typedef struct Label{
    char name[MAX_LABEL_LEN];
    int IC;
    char type[MAX_LABEL_LEN];
    struct Label* next;
} Label;


void addLabelToList(Label** head, Label* label);
Label* searchLabel(Label* head, char name[]);
void freeLabelList(Label* head);
Label* createLabel();


#endif
