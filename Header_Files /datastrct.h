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
    char name[MAX_LABEL_LEN]; /* The name of the macro*/
    DynamicStringArray macroContent; /* an array of strings containing the content*/
    struct MacroNode* next; /* a pointer to the next MacroNode in the list */
} MacroNode;


/*
  Initializes a dynamically allocated array of strings.
 
  @param array - Pointer to a DynamicStringArray struct that will hold the array of strings.
  @return 1 if initialization is successful, 0 otherwise.
 */
int initArray(DynamicStringArray *array);


/*
    Adds a string to an already initialized DynamicStringArray.addString

    @param array - Pointer to an initialized DynamicStringArray struct
    @param string - the string to add to the array

    @return 1 if succeeded in adding the string, 0 otherwise.
*/
int addString(DynamicStringArray *array, char string[]);


/*
    create a new macro node (MacroNode) based on the macros name.
    the function also initializes all of the nodes attributes.

    @param name - the name of the macro.

    @return a pointer to the created node, NULL if creation was not successful.
*/
MacroNode* createMacro(char name[]);


/*
    add a new macro node to the list of existing macros.

    @param head - a double pointer of type MacroNode - to head of the list.
    @param macroName - a string with the name of the macro.

    @return 1 if succeeded in adding the node, 0 otherwise.
*/
int addMacroToList(MacroNode **head,char macroName[]);


/*
    The function searches the list of macros to find a specific macro, based on it's name.

    @param head - a pointer to the head of the list.
    @param name - the name of the macro to search for.

    @return pointer to the macro node in the  list if found the macro, NUll otherwise. 
*/
MacroNode* searchMacro(MacroNode *head, char name[]);


/*
    free the DynamicStringArray(which was initialized by initArray function)

    @param array - the pointer to the array.
*/
void freeArray(DynamicStringArray *array);


/*
    free a single macro node. 

    @param node - a pointer to the node to free.
*/
void freeMacroNode(MacroNode* node);


/*
    free all the nodes in a macro list.

    @param head - a pointer to the head of the string. 
*/
void freeMacroList(MacroNode *head);

#endif
