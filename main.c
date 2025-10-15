#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "patient.h"


int validatetheDate(const char *date)
{

    if (strlen(date) != 10)
    {
        printf("Please input the correct format.\n");
        return 0;
    }

    for (int i = 0; i < 10; i++)
    {
        if (i == 4 || i == 7)
        {
            if (date[i] != '-')
                return 0;
        }
        else
        {
            if (date[i] < '0' || date[i] > '9')
                return 0;
        }
    }

    int day, month, year;
    sscanf(date, " %4d-%2d-%2d", &year, &month, &day);

    if (month < 1 || month > 12)
        return 0;
    if (day < 1 || day > 31)
        return 0;

    return 1;
}

int validateChar(const char *character_notnumber)
{
    if (character_notnumber[0] == '\0')
    {
        printf("Input cannot be empty.\n");
        return 0;
    }

    for (int i = 0; character_notnumber[i]; i++)
    {
        if (!isalpha(character_notnumber[i]) && character_notnumber[i] != ' ')
        {
            printf("Please enter only letters.\n");
            return 0;
        }
    }

    return 1;
}

int validateNum(int *number_notstr, int min, int max)
{
    int result = scanf("%d", number_notstr);

    while (getchar() != '\n')
        ;

    if (result != 1)
    {
        printf("Error: Invalid input. Please enter a valid number.\n");
        return 0;
    }

    if (*number_notstr < min || *number_notstr > max)
    {
        printf("Error: Number must be between %d and %d.\n", min, max);
        return 0;
    }

    return 1;
}

void displayallData()
{

    char row[100];

    FILE *data = fopen(csv, "r");

    if (data == NULL)
    {
        printf("Cannot open the file for reading.\n");
        return;
    }

    while (fgets(row, sizeof(row), data))
    {
        printf("%s", row);
    }

    fclose(data);
}

Patient inputPatientData()
{
    char middlename[30];
    char optional;

    Patient patientData;

    do
    {
        printf("Enter firstname: ");
        scanf("%59s", patientData.firstname);
    } while (!validateChar(patientData.firstname));

    printf("Do you want to input middlename? (y/n): ");
    scanf(" %c", &optional);
    if (optional == 'y' || optional == 'Y')
    {
        do
        {
            printf("Enter Middlename: ");
            scanf("%29s", middlename);
        } while (!validateChar(middlename));

        strcat(patientData.firstname, " ");
        strcat(patientData.firstname, middlename);
    }

    do
    {
        printf("Enter lastname: ");
        scanf("%29s", patientData.lastname);
    } while (!validateChar(patientData.lastname));

    do
    {
        printf("Enter age (0-150): ");
    } while (!validateNum(&patientData.age, 0, 150));

    do
    {
        printf("Enter health status: ");
        scanf("%49s", patientData.healthStatus);
    } while (!validateChar(patientData.healthStatus));

    do
    {
        printf("Enter Check-up Date (YYYY-MM-DD): ");
        scanf("%10s", patientData.checkupDate);
    } while (!validatetheDate(patientData.checkupDate));

    return patientData;
}

void addData()
{
    char middlename[30];
    char optional;

    printf("\n--- Add New Patient ---\n");

    Patient patientData = inputPatientData();

    FILE *data = fopen(csv, "a");
    if (data == NULL)
    {
        printf("Error opening file.\n");
    }

    fprintf(data, "%s,%s,%d,%s,%s\n",
            patientData.firstname,
            patientData.lastname,
            patientData.age,
            patientData.healthStatus,
            patientData.checkupDate);

    fclose(data);

    printf("\nRecord added successfully!\n");
}

void toLowerCase(char *str)
{
    for (int i = 0; str[i]; i++)
    {
        str[i] = tolower(str[i]);
    }
}

void searchData()
{
    // search from name and health status
    char keyword[50];
    char row[1000];
    int found = 0;

    printf("Enter the name or health status: ");
    scanf(" %[^\n]", keyword);
    toLowerCase(keyword);

    FILE *data = fopen(csv, "r");

    if (data == NULL)
    {
        perror("Cannot open the file.");
        return;
    }

    while (fgets(row, sizeof(row), data))
    {
        char rowLower[1000];
        strcpy(rowLower, row);
        toLowerCase(rowLower);

        if (strstr(rowLower, keyword) != NULL)
        {
            printf("%s", row);
            found = 1;
        }
    }

    if (!found)
    {
        printf("No matching records found.\n");
    }

    fclose(data);
}

void updateData()
{
    char keyword[50];
    char row[1000];
    int found = 0;

    printf("Enter the name to update: ");
    scanf(" %[^\n]", keyword);
    toLowerCase(keyword);

    FILE *data = fopen(csv, "r");
    if (data == NULL)
    {
        printf("Cannot open the file reading.");
        return;
    }

    FILE *temp = fopen("temp.csv", "w");
    if (temp == NULL)
    {
        printf("Cannot create the temporary file.");
        return;
    }

    while (fgets(row, sizeof(row), data))
    {
        char rowLower[1000];
        strcpy(rowLower, row);
        toLowerCase(rowLower);

        if (strstr(rowLower, keyword) != NULL)
        {
            found = 1;
            printf("Record found: %s\n", row);
            printf("--- Update Patient Info ---\n");

            Patient patientData = inputPatientData();

            fprintf(temp, "%s,%s,%d,%s,%s\n",
                    patientData.firstname,
                    patientData.lastname,
                    patientData.age,
                    patientData.healthStatus,
                    patientData.checkupDate);
        }
        else
        {
            fputs(row, temp);
        }
    }

    fclose(data);
    fclose(temp);

    remove(csv);
    rename("temp.csv", csv);

    if (found)
        printf("Record updated successfully!\n");
    else
        printf("No matching record found.\n");
}

void deleteData()
{
    char firstname[60], middlename[30] = "", lastname[30];
    char optional;
    char row[1000];
    int found = 0;

    do
    {
        printf("Enter firstname to delete: ");
        scanf("%59s", firstname);
    } while (!validateChar(firstname));

    printf("Do you want to input middlename? (y/n): ");
    scanf(" %c", &optional);

    if (optional == 'y' || optional == 'Y')
    {
        do
        {
            printf("Enter middlename to delete: ");
            scanf("%29s", middlename);
        } while (!validateChar(middlename));

        strcat(firstname, " ");
        strcat(firstname, middlename);
    }

    do
    {
        printf("Enter lastname to delete: ");
        scanf("%29s", lastname);
    } while (!validateChar(lastname));

    toLowerCase(firstname);
    toLowerCase(lastname);

    FILE *data = fopen(csv, "r");

    if (data == NULL)
    {
        printf("Cannot opening file.\n");
        return;
    }

    FILE *temp = fopen("temp.csv", "w");

    if (temp == NULL)
    {
        printf("Cannot opening the temporary file.\n");
        return;
    }

    char *token;
    char rowFirst[60], rowLast[30];
    char rowLower[1000];

    while (fgets(row, sizeof(row), data))
    {
        strcpy(rowLower, row);
        toLowerCase(rowLower);

        token = strtok(rowLower, ",");
        strcpy(rowFirst, token);

        token = strtok(NULL, ",");
        strcpy(rowLast, token);

        if (strcmp(rowFirst, firstname) == 0 && strcmp(rowLast, lastname) == 0)
        {
            found = 1;
            printf("Delete record: %s", row);
            continue;
        }

        fputs(row, temp);
    }

    fclose(data);
    fclose(temp);

    remove(csv);
    rename("temp.csv", csv);

    if (found)
        printf("Record deleted successfully!\n");
    else
        printf("No matching record found.\n");
}

void clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void exitMenu()
{
    printf("Thank you for using my program system.\n");
}

int main()
{

    char alphabet;
    int running = 1;

    while (running)
    {

        printf("----- Welcome to Annual Head Check up Management System! -----\n \n a. Display all data\n b. Add Data\n c. Search Data\n d. Update Data\n e. Delete data\n f. Unit test\n g. End to end test\n h. Exit Menu\n");
        printf("\nPlease Enter an alphabet[a-h] to display a menu: ");
        scanf(" %c", &alphabet);

        switch (alphabet)
        {
        case 'a':
            displayallData();
            break;
        case 'b':
            addData();
            break;
        case 'c':
            searchData();
            break;
        case 'd':
            updateData();
            break;

        case 'e':
            deleteData();
            break;

        case 'f':
            runUnitTest();
            break;

        case 'g':
            runE2eTest();
            break;

        case 'h':
            exitMenu();
            break;

        default:
            printf("Please input a valid alphabet (a-h).\n");
            break;
        }

        if (running)
        {
            char continuesMenu;
            printf("\nDo you want to continue program? (y/n): ");
            scanf(" %c", &continuesMenu);

            if (continuesMenu == 'n' || continuesMenu == 'N')
            {
                running = 0;
                clearScreen();
                exitMenu();
            }
            else if (continuesMenu == 'y' || continuesMenu == 'Y')
            {
                clearScreen();
                running = 1;
            }
            else
            {
                do
                {
                    printf("Please enter a correct alphabet (only y/n): ");
                    scanf(" %c", &continuesMenu);

                    if (continuesMenu == 'n' || continuesMenu == 'N')
                    {
                        running = 0;
                        clearScreen();
                        exitMenu();
                        break;
                    }
                    else if (continuesMenu == 'y' || continuesMenu == 'Y')
                    {
                        running = 1;
                        clearScreen();
                        break;
                    }
                } while (1);
            }
        }
    }

    return 0;
}