/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <debug.h>
#include <inttypes.h>
#include <round.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WORDSIZE 4

void insertIntoStack(const char *cmdline UNUSED, void **esp){
    
//printf("Base Address: 0x%08x\n", (unsigned int) *esp);

    // Word align with the stack pointer. DO NOT REMOVE THIS LINE.
    *esp = (void*) ((unsigned int) (*esp) & 0xfffffffc);
    
    //For/while loop through the cmdline plugging in cmdline char into
    //array as seperate words divided by /o
    
    //From the end of the for loop insert these strings into the stack
    //Also store the addresses that they get plugged into the stack into
    //a seperate array.
    //Also when finished inserting single string into array, get the size of 
    //that string and store it into a sepearte array.
    
    //Insert magic code to do word align
    
    
    
    
    int len = strlen(cmdline) + 1;  // get length of cmdline
    
    int argv0adrss = 0;
    
    *esp -= len;    //move the stack pointer 'len' down
    memcpy(*esp, cmdline, len); //cpy data onto stack
    argv0adrss = *esp;
    //printf("After Memcpy im here: 0x%08x\n", (unsigned int) *esp);
    
    // magic code to do word align
    *esp = (void*) ((unsigned int) (*esp) & 0xfffffffc);
    //printf("After word align: 0x%08x\n", (unsigned int) *esp);
    
    //move a word size down and input in for argv[1]
    *esp -= WORDSIZE;
    //printf("After word size move1: 0x%08x\n",(unsigned int) *esp);
    *((char*) *esp) = 0;
    
    //move a word size down and input in for argv[0]
    *esp -= WORDSIZE;
    //printf("After word size move2: 0x%08x\n",(unsigned int) *esp);
    *((void**) *esp) = argv0adrss; //pointer to a pointer
    
    //move a word size down and input in for argv
    *esp -= WORDSIZE;
    //printf("After word size move3: 0x%08x\n",(unsigned int) *esp);
    *((void**) *esp) = *esp + WORDSIZE;
    
    //move a word size down and input in for argc(number of arguements plus command)
    *esp -= WORDSIZE;
    //printf("After word size move4: 0x%08x\n",(unsigned int) *esp);
    *((int*) *esp) = 1;
    
    //move a word size down and input in for fake return address
    *esp -= WORDSIZE;
    //printf("After word size fake return address: 0x%08x\n",(unsigned int) *esp);
    *((void**) *esp) = 0;    
    
    
}

