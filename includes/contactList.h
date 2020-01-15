#ifndef __CONTACTLISTH__
#define __CONTACTLISTH__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*Struct given in assignment outline*/
struct contact {
    unsigned long phone_number;
    long first_name_posn;
    long last_name_posn;
    long company_name_posn;
    long email_posn;
    long next;
};
typedef struct contact contact;

/*Buffer struct to temporarily hold contact info*/
struct entryBuffers {
    char firstName[128];
    char lastName[128];
    char companyName[128];
    char email[128];
};
typedef struct entryBuffers bufferStruct;

/*Buffer for sorting information before the list of contacts*/
struct sortingBuffer {
    unsigned long phoneNumber;
    char firstName[128];
    char lastName[128];
    char companyName[128];
    char email[128];
};
typedef struct sortingBuffer sortBuffer;

/*Function that displays the full sorted contact list and performs edits*/
int displayList (FILE * fp, int flag);
/** displayList Flag Paths
 * 0 = exit program
 * 1 = return to full list interface
 * 2 = add a new contact
 */

/*Function to write new contacts to the file.*/
void enterInformation (FILE * fp);

/*Function to count digits in a number. Returns number of digits in number.*/
int digitCount (unsigned long number);

/*Function to check phone number entry for non-numbers. Returns 0 if there are non-numbers.*/
int digitCheck (char phoneBuffer[256]);

/*Function to check validity of email. Returns 0 if email is invalid.*/
int emailCheck (char email[128]);

/*Function to initialize all values of the struct of static info array to NULL character*/
void cleanInfoArray (struct entryBuffers *info);

/*Function to initialize all values of the struct of static sort array to NULL character*/
void cleanSortArray (struct sortingBuffer *sort);

#endif
