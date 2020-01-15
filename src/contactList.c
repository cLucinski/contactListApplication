/*****************************
CIS*2500 Assignment 4
Filename: contactList.c
Student: Christopher Lucinski
******************************/

#include "contactList.h"

int main()
{
    FILE *fp;
    int flag;
    flag = 1;

    fp = fopen("myContactList.db", "rb+");  /*If file exits, open it for reading and writing*/
    if (fp == NULL)
    {
        fp = fopen("myContactList.db", "wb+");  /*If file doesn't exist, create new one for reading and writing*/
        if (fp == NULL)
        {
            perror("Error creating file");
        }
    }

    do
    {
        flag = displayList(fp, flag);  /*Function to display full list and perform edits*/
        if (flag == 2)
        {
            enterInformation(fp);  /*Function for adding a contact to the list*/
        }
    } while (flag != 0);  /*Loop until user chooses to exit program*/

    fclose(fp);
    return 0;
}

