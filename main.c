#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define csv "Checkup-Data.csv"

// ใส่วิธีcompile + run program

typedef struct
{
    char firstname[30];
    char middlename[30];
    char lastname[30];
    int age;
    char healthStatus[50];
    char checkupDate[11]; // format YYYY-MM-DD + \0

} Patient;

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

int validateChar(const char *character_notnum)
{
    for (int i = 0; character_notnum[i] != '\0'; i++)
    {
        if (!((character_notnum[i] >= 'A' && character_notnum[i] <= 'Z') ||
              (character_notnum[i] >= 'a' && character_notnum[i] <= 'z')))
        {
            printf("Please enter only letters.\n");
            return 0;
        }
    }
    return 1;
}

int validateNum(const char *number_notchar)
{
    for (int i = 0; number_notchar[i] != '\0'; i++)
    {
        if (number_notchar[i] < '0' || number_notchar[i] > '9')
        {
            printf("Please enter only numbers.\n");
            return 0;
        }
    }
    return 1;
}

void addData()
{

    Patient newPatient;
    char middlename;

    do
    {
        printf("Enter Firstname: ");
        scanf("%29s", newPatient.firstname);
    } while (!validateChar(newPatient.firstname));

    do
    {
        printf("\nDo you want to input middlename? (y/n): ");
        scanf(" %c", &middlename);

        if (middlename == 'n' || middlename == 'N')
        {
            strcpy(newPatient.middlename, "");
            break;
        }
        else if (middlename == 'y' || middlename == 'Y')
        {
            printf("Enter Middlename: ");
            scanf("%29s", newPatient.middlename);
            break;
        }
        printf("Please enter a correct alphabet (only y/n)\n");
    } while (1);

    do
    {
        printf("Enter Lastname: ");
        scanf("%29s", newPatient.lastname);
    } while (!validateChar(newPatient.lastname));

    do
    {
        printf("Enter Age: ");
        scanf("%d", &newPatient.age);
    } while (!validateNum(newPatient.age));

    do
    {
        printf("Enter Health Status: ");
        scanf(" %49[^\n]", newPatient.healthStatus);
    } while (!validateChar(newPatient.healthStatus));

    do
    {
        printf("Enter Checkup Date (YYYY-MM-DD): ");
        scanf("%10s", newPatient.checkupDate);

    } while (!validatetheDate(newPatient.checkupDate));

    FILE *data = fopen(csv, "a");

    if (data == NULL)
    {
        printf("Error opening file.");
        return;
    }

    fprintf(data, "%s,%s,%s,%d,%s,%s\n",
            newPatient.firstname,
            newPatient.middlename,
            newPatient.lastname,
            newPatient.age,
            newPatient.healthStatus,
            newPatient.checkupDate);

    fclose(data);
}

void displayallData()
{

    char row[1000];

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

void toLowerCase(char *str)
{
    // make string to lowercase
    for (; *str; str++)
    {
        if (*str >= 'A' && *str <= 'Z')
        {
            *str = *str - 'A' + 'a';
        }
    }
}

void searchData()
{
    // search from name and health status
    char keyword[50];
    char row[1000];
    int found = 0;

    printf("Search or Enter data you want : ");
    scanf(" %[^\n]", keyword);

    toLowerCase(keyword);

    FILE *data = fopen(csv, "r");

    if (data == NULL)
    {
        printf("Cannot open the file.");
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
    char firstname[30], middlename[30], lastname[30];
    char row[1000];
    int found = 0;
    Patient updatedPatient;

    FILE *data = fopen(csv, "r");
    FILE *temp = fopen("temp.csv", "w");

    if (data == NULL || temp == NULL)
    {
        printf("Error opening files.\n");
        return;
    }

    char oldFirstname[30], oldLastname[30];
    printf("Enter First Name to update: ");
    scanf("%29s", oldFirstname);
    printf("Enter Last Name to update: ");
    scanf("%29s", oldLastname);

    char searchName[60];
    sprintf(searchName, "%s,%s", oldFirstname, oldLastname);

    while (fgets(row, sizeof(row), data))
    {
        char tmpRow[1000];
        strcpy(tmpRow, row);


        char *token = strtok(tmpRow, ",");
        char *midToken = strtok(NULL, ","); 
        char *lastToken = strtok(NULL, ",");

        if (token && lastToken && strcmp(token, oldFirstname) == 0 && strcmp(lastToken, oldLastname) == 0)
        {
            found = 1;

            do {
                printf("Enter new Firstname: ");
                scanf("%29s", updatedPatient.firstname);
            } while (!validateChar(updatedPatient.firstname));

            do {
                printf("Do you want to input Middlename? (y/n): ");
                char choice;
                scanf(" %c", &choice);
                if (choice == 'n' || choice == 'N') {
                    strcpy(updatedPatient.middlename, "");
                    break;
                } else if (choice == 'y' || choice == 'Y') {
                    do {
                        printf("Enter Middlename: ");
                        scanf("%29s", updatedPatient.middlename);
                    } while (!validateChar(updatedPatient.middlename));
                    break;
                }
            } while (1);

            do {
                printf("Enter new Lastname: ");
                scanf("%29s", updatedPatient.lastname);
            } while (!validateChar(updatedPatient.lastname));

            char ageStr[10];
            do {
                printf("Enter new Age: ");
                scanf("%s", ageStr);
            } while (!validateNum(ageStr));
            updatedPatient.age = atoi(ageStr);

            do {
                printf("Enter new Health Status: ");
                scanf(" %49[^\n]", updatedPatient.healthStatus);
            } while (!validateChar(updatedPatient.healthStatus));

            do {
                printf("Enter Checkup Date (YYYY-MM-DD): ");
                scanf("%10s", updatedPatient.checkupDate);
            } while (!validatetheDate(updatedPatient.checkupDate));

            fprintf(temp, "%s,%s,%s,%d,%s,%s\n",
                    updatedPatient.firstname,
                    updatedPatient.middlename,
                    updatedPatient.lastname,
                    updatedPatient.age,
                    updatedPatient.healthStatus,
                    updatedPatient.checkupDate);
        }
        else
        {
            fprintf(temp, "%s", row);
        }
    }

    fclose(data);
    fclose(temp);

    if (found)
    {
        remove(csv);
        rename("temp.csv", csv);
        printf("Record updated successfully.\n");
    }
    else
    {
        remove("temp.csv");
        printf("Record not found.\n");
    }
}

void deleteData()
{
    char firstname[30], lastname[30];
    char row[1000];
    int found = 0;

    FILE *data = fopen(csv, "r");
    FILE *temp = fopen("temp.csv", "w");

    if (data == NULL || temp == NULL)
    {
        printf("Error opening files.\n");
    }

    printf("Enter First Name to delete: ");
    scanf("%29s", firstname);
    printf("Enter Last Name to delete: ");
    scanf("%29s", lastname);

    char searchName[50];
    sprintf(searchName, "%s %s", firstname, lastname);

    while (fgets(row, sizeof(row), data))
    {

        if (strncmp(row, searchName, strlen(searchName)) == 0)
        {
            found = 1;
            continue;
        }
        fprintf(temp, "%s", row);
    }

    fclose(data);
    fclose(temp);

    if (found)
    {
        remove(csv);
        rename("temp.csv", csv);
        printf("Record deleted successfully.\n");
    }
    else
    {
        remove("temp.csv");
        printf("Record not found.\n");
    }
}

void unitTest()
{
}

void endtoendTest()
{
}

void exitMenu()
{
    printf("Thank you for using the program system.\n");
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
            unitTest();
            break;

        case 'g':
            endtoendTest();
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
                exitMenu();
                break;
            }
            else if (continuesMenu == 'y' || continuesMenu == 'Y')
            {
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
                        exitMenu();
                        break;
                    }
                    else if (continuesMenu == 'y' || continuesMenu == 'Y')
                    {
                        running = 1;
                        break;
                    }
                } while (1);
            }
        }
    }

    return 0;
}
