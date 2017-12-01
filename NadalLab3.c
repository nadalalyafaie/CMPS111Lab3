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
#include "NadalLab3.h"
#include "threads/vaddr.h"
#include "threads/palloc.h"

#define WORDSIZE 4

void insertIntoStack(const char *cmdline UNUSED, void **esp){
    
//printf("Base Address: 0x%08x\n", (unsigned int) *esp);

    // Word align with the stack pointer. DO NOT REMOVE THIS LINE.
    *esp = (void*) ((unsigned int) (*esp) & 0xfffffffc);
    char *cmdlineCopy = (char *) palloc_get_page(0);
    char **cmdarg = (char **) palloc_get_page(0); //Array to hold cmdline arguements & exec file
    char *dummyarg;
    void **argAddresses = (void **) palloc_get_page(0);
    int argCounter = 1;
    int len = 0;
    
    //args-single: copy whats in the command line pointer to another variable
    strlcpy(cmdlineCopy, (char *) cmdline, PGSIZE); 
    
    //Grabs the first line then stores the second in a dummy variable
    //Then the dummy arg is one step ahead so that it can check if its NULL
    //in the for loop. Cmdarg should store all the command line arguements
    cmdarg[0] = strtok_r(cmdlineCopy, " ", &cmdlineCopy);
    dummyarg = strtok_r(NULL, " ", &cmdlineCopy);
    for(int i = 1; dummyarg != NULL; i++){
        cmdarg[i] = dummyarg;
        dummyarg = strtok_r(NULL, " ", &cmdlineCopy);
        argCounter++;
    }

    for(int i = argCounter - 1; i >= 0; i--){
        len = strlen(cmdarg[i]) + 1;  // get length of cmdarg
        *esp -= len;    //move the stack pointer 'len' down
        argAddresses[i] = *esp; //store address into array
        memcpy(*esp, cmdarg[i], len); //cpy data onto stack
    }
    
    // magic code to do word align
    *esp = (void*) ((unsigned int) (*esp) & 0xfffffffc);
    
    //move a word size down and input in for argv[1]
    *esp -= WORDSIZE;
    //printf("After word size move1: 0x%08x\n",(unsigned int) *esp);
    *((char*) *esp) = 0;
    
    for(int i = argCounter - 1; i >= 0; i--){
        *esp -= WORDSIZE; //decrement stack pointer
        *((void**) *esp) = argAddresses[i]; //put addresses into stack
    }
    
    //move a word size down and input in for argv
    *esp -= WORDSIZE;
    //printf("After word size move3: 0x%08x\n",(unsigned int) *esp);
    *((void**) *esp) = *esp + WORDSIZE;
    
    //move a word size down and input in for argc(number of arguements plus command)
    *esp -= WORDSIZE;
    //printf("After word size move4: 0x%08x\n",(unsigned int) *esp);
    *((int*) *esp) = argCounter;
    
    //move a word size down and input in for fake return address
    *esp -= WORDSIZE;
    //printf("After word size fake return address: 0x%08x\n",(unsigned int) *esp);
    *((void**) *esp) = 0;  
    
    
    //For/while loop through the cmdline plugging in cmdline char into
    //array as seperate words divided by /o
    
    //From the end of the for loop insert these strings into the stack
    //Also store the addresses that they get plugged into the stack into
    //a seperate array.
    //Also when finished inserting single string into array, get the size of 
    //that string and store it into a sepearte array.
    
    //Insert magic code to do word align
    
    
    
//    
//    int len = strlen(cmdline) + 1;  // get length of cmdline
//    
//    void *argv0adrss = 0;
//    
//    *esp -= len;    //move the stack pointer 'len' down
//    memcpy(*esp, cmdline, len); //cpy data onto stack
//    argv0adrss = *esp;
//    //printf("After Memcpy im here: 0x%08x\n", (unsigned int) *esp);
//    
//    // magic code to do word align
//    *esp = (void*) ((unsigned int) (*esp) & 0xfffffffc);
//    //printf("After word align: 0x%08x\n", (unsigned int) *esp);
//    
//    //move a word size down and input in for argv[1]
//    *esp -= WORDSIZE;
//    //printf("After word size move1: 0x%08x\n",(unsigned int) *esp);
//    *((char*) *esp) = 0;
//    
//    //move a word size down and input in for argv[0]
//    *esp -= WORDSIZE;
//    //printf("After word size move2: 0x%08x\n",(unsigned int) *esp);
//    *((void**) *esp) = argv0adrss; //pointer to a pointer
//    
//    //move a word size down and input in for argv
//    *esp -= WORDSIZE;
//    //printf("After word size move3: 0x%08x\n",(unsigned int) *esp);
//    *((void**) *esp) = *esp + WORDSIZE;
//    
//    //move a word size down and input in for argc(number of arguements plus command)
//    *esp -= WORDSIZE;
//    //printf("After word size move4: 0x%08x\n",(unsigned int) *esp);
//    *((int*) *esp) = 1;
//    
//    //move a word size down and input in for fake return address
//    *esp -= WORDSIZE;
//    //printf("After word size fake return address: 0x%08x\n",(unsigned int) *esp);
//    *((void**) *esp) = 0;    
    
    
}

