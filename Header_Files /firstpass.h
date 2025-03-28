#ifndef FIRST_PASS_H
#define FIRST_PASS_H

/* 
    the function executes the first pass of the assembly program. 
    the function only checks and stores the commands, instructions, and labels 
    without translating them to machine code. 

    @param as_file_name - the name of the .as file. 
*/
void firstPass(char* as_file_name);

#endif
