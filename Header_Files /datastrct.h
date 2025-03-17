#ifndef DATASTRCT_H
#define DATASTRCT_H

#include "../Header_Files /globals.h"


/* Define a structure for a dynamic array of strings */
typedef struct {
    char **data;    /* Pointer to the array of strings */
    int used;       /* Current number of elements */
    int size;   /* Maximum capacity of the array */
} DynamicStringArray;

typedef struct MacroNode{
    char name[MAX_LABEL_LEN];
    DynamicStringArray macroContent;
    struct MacroNode* next;
} MacroNode;


int initArray(DynamicStringArray *array);
int addString(DynamicStringArray *array, char string[]);

void printArray(DynamicStringArray* array);
MacroNode* createMacro(char name[]);
int addMacroToList(MacroNode **head,char macroName[]);
MacroNode* searchMacro(MacroNode *head, char name[]);
void freeArray(DynamicStringArray *array);
void freeMacroNode(MacroNode* node);
void freeMacroList(MacroNode *head);

#endif
