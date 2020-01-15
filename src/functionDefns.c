/*****************************
CIS*2500 Assignment 4
Filename: functionDefns.c
Student: Christopher Lucinski
******************************/

#include "contactList.h"

int displayList(FILE * fp, int flag)
{
    /*Declare all structs, arrays, and variables needed*/
    contact * location;
    contact tempLocation;
    bufferStruct * info;
    bufferStruct tempInfo;
    sortBuffer * sort;
    sortBuffer tempSort;
    int contactNum;
    int i;
    int j;
    char action[64];
    char letterAction;
    char letterActionEdit;
    long int number;
    char phoneBuffer[128];
    unsigned long tempPhoneNumber;
    unsigned long deleteEntry;
    /*Initialize needed values*/
    flag = 1;
    deleteEntry = 1;
    phoneBuffer[0] = '\0';
    contactNum = 0;
    /*Move to start of file, allocate memory for structs*/
    fseek(fp, 0, SEEK_SET);
    location = NULL;
    info = NULL;
    sort = NULL;
    location = malloc(sizeof(contact) + 1);
    info = malloc(sizeof(bufferStruct) + 1);

    while (fread(&location[contactNum], sizeof(contact), 1, fp) == 1)  /*Read location information from file until end of file*/
    {
        if (location[contactNum].phone_number != 1)  /*If contact was not soft deleted, read contactinformation from file*/
        {
            cleanInfoArray(&info[contactNum]);
            fread(&info[contactNum].firstName, sizeof(char), 128, fp);
            fread(&info[contactNum].lastName, sizeof(char), 128, fp);
            fread(&info[contactNum].companyName, sizeof(char), 128, fp);
            fread(&info[contactNum].email, sizeof(char), 128, fp);

            ++contactNum;
            location = realloc(location, sizeof(contact) * (contactNum + 1));
            info = realloc(info, sizeof(bufferStruct) * (contactNum + 1));
            fseek(fp, location[contactNum - 1].next, SEEK_SET);  /*Move to next contact entry*/
        }
        else if (location[contactNum].phone_number == 1)  /*If contact was soft deleted, move to next location information*/
        {
            fseek(fp, location[contactNum].next, SEEK_SET);
        }
    }
    sort = malloc(sizeof(sortBuffer));
    j = 0;
    for (i = 0; i < contactNum; ++i)  /*Copy contact information to a sorting struct*/
    {
        if (location[j].phone_number != 1)
        {
            cleanSortArray(&sort[i]);
            sort[i].phoneNumber = location[j].phone_number;
            strncpy(sort[i].firstName, info[j].firstName, 128);
            strncpy(sort[i].lastName, info[j].lastName, 128);
            strncpy(sort[i].companyName, info[j].companyName, 128);
            strncpy(sort[i].email, info[j].email, 128);
            if (sort[i].lastName[0] == '\0' || sort[i].lastName[0] == '\n')
            {
                strncpy(sort[i].lastName, sort[i].companyName, 128);
            }
            sort = realloc(sort, sizeof(sortBuffer) * (i + 2));
        }
        else
        {
            --i;
        }
        ++j;
    }
    for (i = 0; i < contactNum - 1; ++i)  /*Bubble sort the contacts to be in ascending order*/
    {
        for (j = 0; j < contactNum - 1 - i; ++j)
        {
            if (strcmp(sort[j].lastName, sort[j+1].lastName) > 0)
            {
                tempSort = sort[j];
                sort[j] = sort[j+1];
                sort[j+1] = tempSort;
                tempInfo = info[j];
                info[j] = info[j+1];
                info[j+1] = tempInfo;
            }
        }
    }
    printf("\nNumber of Contacts = %d\n", contactNum);
    for (i = 0; i < contactNum; ++i)  /*Print full list of contacts*/
    {
        printf("%d. ", i+1);
        if (info[i].lastName[0] == '\n' || info[i].lastName[0] == '\0')  /*If last name does not exit in file, print company name*/
        {
            printf("%s", info[i].companyName);
        }
        else  /*Otherwise, print first name (if it exists) followed by last name*/
        {
            for (j = 0; info[i].firstName[j] != '\n'; ++j)
            {
                printf("%c", info[i].firstName[j]);
            }
            if (j == 0)
            {
                printf("%s", info[i].lastName);
            }
            else
            {
                printf(" %s", info[i].lastName);
            }
        }
    }

    do
    {
        number = 0;
        printf("Action: ");  /*Prompt user for next action*/
        fgets(action, 64, stdin);
        number = strtol(action, NULL, 10);

        if (number != 0 && number <= contactNum)  /*Display a chosen contact's information*/
        {
            printf("Contact #%ld\n", number);
            printf("First Name: %s", sort[number-1].firstName);
            printf("Last Name: %s", sort[number-1].lastName);
            printf("Company Name: %s", sort[number-1].companyName);
            printf("Phone Number: %lu\n", sort[number-1].phoneNumber);
            printf("Email: %s", sort[number-1].email);
            do
            {
                printf("Action: ");  /*Prompt user for next action*/
                scanf("%c", &letterAction);
                getchar();
                tempPhoneNumber = sort[number-1].phoneNumber;
                if (letterAction == 'E')  /*Allow user to re-enter the contact's phone number and email to change them*/
                {
                    printf("Contact #%ld\n", number);
                    printf("First Name: %s", sort[number-1].firstName);
                    printf("Last Name: %s", sort[number-1].lastName);
                    printf("Company Name: %s", sort[number-1].companyName);
                    do
                    {
                        printf("Phone Number (enter only numbers): ");
                        fgets(phoneBuffer, 128, stdin);
                        sort[number-1].phoneNumber = strtoul(phoneBuffer, NULL, 10);
                    } while (!(((digitCount(sort[number-1].phoneNumber) == 7) || (digitCount(sort[number-1].phoneNumber) == 10)) && digitCheck(phoneBuffer)));  /*Screen phone number to make sure it meets the criteria*/
                    do
                    {
                        printf("Email: ");
                        fgets(sort[number-1].email, 128, stdin);
                    } while (!(emailCheck(sort[number-1].email)));  /*Screen the email to make sure it meets the criteria*/

                    do
                    {
                        printf("Action: ");
                        scanf("%c", &letterActionEdit);
                        getchar();
                        if (letterActionEdit == 'S')  /*Save the changes made to the contact information*/
                        {
                            fseek(fp, 0, SEEK_SET);
                            while (fread(&tempLocation, sizeof(contact), 1, fp) == 1)
                            {
                                if (tempLocation.phone_number == tempPhoneNumber)  /*Search file for location of contact's information*/
                                {
                                    fseek(fp, sizeof(contact) * (-1), SEEK_CUR);
                                    fwrite(&sort[number-1].phoneNumber, sizeof(unsigned long), 1, fp);  /*Writes new phone number into file*/
                                    fseek(fp, tempLocation.email_posn, SEEK_SET);
                                    fwrite(&sort[number-1].email, sizeof(char), strlen(sort[number-1].email) + 1, fp); /*Writes new email into file*/
                                    break;
                                }
                                else
                                {
                                    fseek(fp, tempLocation.next, SEEK_SET);
                                }
                            }
                            flag = 1;  /*Function will return 1, taking user back to the full list*/
                            break;
                        }
                        else if (letterActionEdit == 'D')  /*Do not save edits; delete the whole contact from the list*/
                        {
                            fseek(fp, 0, SEEK_SET);
                            while (fread(&tempLocation, sizeof(contact), 1, fp) == 1)
                            {
                                if (tempLocation.phone_number == tempPhoneNumber)  /*Search file for location of contact's information*/
                                {
                                    fseek(fp, (sizeof(struct contact) * (-1)), SEEK_CUR);  /*Soft delete contact*/
                                    fwrite(&deleteEntry, sizeof(long), 1, fp);
                                }
                                else
                                {
                                    fseek(fp, tempLocation.next, SEEK_SET);
                                }
                            }
                            flag = 1;  /*Function will return 1, taking user back to the full list*/
                            break;
                        }
                        else if (letterActionEdit == 'R')  /*Do not save edits; return to full list*/
                        {
                            flag = 1;  /*Function will return 1, taking user back to the full list*/
                            break;
                        }
                    } while (1);
                    break;
                }
                else if (letterAction == 'D')  /*Delete the chosen contact from the list*/
                {
                    fseek(fp, 0, SEEK_SET);
                    while (fread(&tempLocation, sizeof(contact), 1, fp) == 1)
                    {
                        if (tempLocation.phone_number == tempPhoneNumber)  /*Search file for location of contact's information*/
                        {
                            fseek(fp, (sizeof(struct contact) * (-1)), SEEK_CUR);  /*Soft delete*/
                            fwrite(&deleteEntry, sizeof(long), 1, fp);
                        }
                        else
                        {
                            fseek(fp, tempLocation.next, SEEK_SET);
                        }
                    }
                    flag = 1;  /*Function will return 1, taking user back to the full list*/
                    break;
                }
                else if (letterAction == 'R')
                {
                    flag = 1;  /*Function will return 1, taking user back to the full list*/
                    break;
                }
            } while (1);
            break;
        }
        else if (strncmp(action, "A", 1) == 0)
        {
            flag = 2;  /*Function will return 2, prompting a new contact to be created*/
            break;
        }
        else if (strncmp(action, "X", 1) == 0)
        {
            flag = 0;  /*Function will return 0, closing the program*/
            break;
        }
    } while (1);
    free(location);
    free(info);
    free(sort);
    return flag;  /*Return value of flag instructing main() what to do next*/
}

void enterInformation (FILE * fp)  /*Functionfor writing a new contact and its information the file*/
{
    contact location;  /*Struct for info locations*/
    bufferStruct buffer;  /*Struct to save input*/
    int start;
    char phoneBuffer[128];
    char letterActionNew;
    phoneBuffer[0] = '\0';

    fseek(fp, 0, SEEK_END);

    cleanInfoArray(&buffer);
    printf("New Contact\n");
    /*Collect information for new contact from user*/
    printf("First Name: ");
    fgets(buffer.firstName, 128, stdin);
    printf("Last Name: ");
    fgets(buffer.lastName, 128, stdin);
    printf("Company Name: ");
    fgets(buffer.companyName, 128, stdin);
    while ((strcmp(buffer.companyName, "\n") == 0) && (strcmp(buffer.lastName, "\n") == 0))  /*Make sure that either the last name or company name spot isn't empty*/
    {
        printf("Last Name: ");
        fgets(buffer.lastName, 128, stdin);
        printf("Company Name: ");
        fgets(buffer.companyName, 128, stdin);
    }
    do
    {
        printf("Phone Number (enter only numbers): ");
        fgets(phoneBuffer, 128, stdin);
        location.phone_number = strtoul(phoneBuffer, NULL, 10);
    } while (!(((digitCount(location.phone_number) == 7) || (digitCount(location.phone_number) == 10)) && digitCheck(phoneBuffer)));  /*Screen phone number to make sure it meets the criteria*/
    do
    {
        printf("Email: ");
        fgets(buffer.email, 128, stdin);
    } while (!(emailCheck(buffer.email)));  /*Screen the email to make sure it meets the criteria*/

    do
    {
        printf("Action: ");
        scanf("%c", &letterActionNew);
        getchar();
        if (letterActionNew == 'S')  /*Save new contact information to file*/
        {
            start = ftell(fp);   /*Save start position of the new contact entry*/
            fwrite(&location.phone_number, sizeof(unsigned long), 1, fp);  /*Write phone number*/

            /*Write position of first name*/
            location.first_name_posn = start + (sizeof(long) * 6);
            fwrite(&location.first_name_posn, sizeof(long), 1, fp);

            /*Write position of last name*/
            location.last_name_posn = start + (sizeof(long) * 6) + sizeof(buffer.firstName);
            fwrite(&location.last_name_posn, sizeof(long), 1, fp);

            /*Write position of company name*/
            location.company_name_posn = start + (sizeof(long) * 6) + (sizeof(buffer.firstName)) + (sizeof(buffer.lastName));
            fwrite(&location.company_name_posn, sizeof(long), 1, fp);

            /*Write postion of email*/
            location.email_posn = start + (sizeof(long) * 6) + (sizeof(buffer.firstName)) + (sizeof(buffer.lastName)) + (sizeof(buffer.companyName));
            fwrite(&location.email_posn, sizeof(long), 1, fp);

            /*Write position of next entry*/
            location.next = start + (sizeof(long) * 6) + (sizeof(buffer.firstName)) + (sizeof(buffer.lastName)) + (sizeof(buffer.companyName)) + (sizeof(buffer.email));   /*Write position of next entry*/
            fwrite(&location.next, sizeof(long), 1, fp);


            /*Writing entered contact information to the file*/
            fwrite(&buffer.firstName, sizeof(char), sizeof(buffer.firstName), fp);
            fwrite(&buffer.lastName, sizeof(char), sizeof(buffer.lastName), fp);
            fwrite(&buffer.companyName, sizeof(char), sizeof(buffer.companyName), fp);
            fwrite(&buffer.email, sizeof(char), sizeof(buffer.email), fp);
            break;
        }
        else if (letterActionNew == 'R')  /*Do not save new contact entry; return to full list*/
        {
            break;
        }
    } while (1);
    return;
}

int digitCount (unsigned long number)  /*Function to count digits in a number. Returns number of digits in number.*/
{
    int count;
    count = 0;
    while (number > 0)
    {
        number = number / 10;
        ++count;
    }
    return count;
}

int digitCheck (char phoneBuffer[128])  /*Function to check phone number entry for non-numbers. Returns 0 if there are non-numbers.*/
{
    int i;
    int flag;
    flag = 1;
    for (i = 0; phoneBuffer[i] != '\n'; ++i)
    {
        if (isdigit(phoneBuffer[i]) == 0)
        {
            flag = 0;
        }
    }
    return flag;
}

int emailCheck (char email[128])  /*Function to check validity of email. Returns 0 if email is invalid.*/
{
    int flag;
    int flagAt;
    int flagPeriod;
    int i;
    int locationAt;
    int locationPeriod;
    locationAt = 0;
    locationPeriod = 0;
    flag = 1;
    flagAt = 0;
    flagPeriod = 0;
    if (email[0] == '@')  /*Email address cannot start with '@'*/
    {
        flag = 0;
    }
    for (i = 0; email[i] != '\n'; ++i)  /*Email must have both '@' and '.'*/
    {
        if (email[i] == '@')
        {
            locationAt = i;
            ++flagAt;
        }
        if (email[i] == '.')
        {
            locationPeriod = i;
            flagPeriod = 1;
        }
    }
    if (email[locationAt + 1] == '.')    /*Email cannot have a '.' immediately after '@'*/
    {
        flag = 0;
    }
    if (email[locationPeriod + 1] == '\n')  /*Email must have content after '.'*/
    {
        flag = 0;
    }
    if (flagAt != 1 || flagPeriod == 0)  /*If email does not meet above criteria, set flag to 0*/
    {
        flag = 0;
    }
    if (email[0] == '\n')  /*If email is not entered, function considers it to be valid*/
    {
        flag = 1;
    }
    return flag;  /*Returns flag, indicating if email is valid*/
}

void cleanInfoArray (struct entryBuffers *info)/*Function to initialize all values of the struct of static info array to NULL character*/
{
    int i;
    for (i = 0; i < 128; ++i)
    {
        info->firstName[i] = '\0';
        info->lastName[i] = '\0';
        info->companyName[i] = '\0';
        info->email[i] = '\0';
    }
    return;
}

void cleanSortArray (struct sortingBuffer *sort)  /*Function to initialize all values of the struct of static sort array to NULL character*/
{
    int i;
    sort->phoneNumber = 0;
    for (i = 0; i < 128; ++i)
    {
        sort->firstName[i] = '\0';
        sort->lastName[i] = '\0';
        sort->companyName[i] = '\0';
        sort->email[i] = '\0';
    }
    return;
}
