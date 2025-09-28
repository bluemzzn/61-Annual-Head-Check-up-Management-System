#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define csv "Checkup-Data.csv"

typedef struct
{
    char firstname[30];
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

void addData()
{

    Patient newPatient;

    printf("Enter Firstname: ");
    scanf("%29s", newPatient.firstname);

    printf("Enter Lastname: ");
    scanf("%29s", newPatient.lastname);

    printf("Enter Age: ");
    scanf("%d", &newPatient.age);

    printf("Enter Health Status: ");
    scanf(" %49[^\n]", newPatient.healthStatus);


    do{
        printf("Enter Checkup Date (YYYY-MM-DD): ");
        scanf("%10s", newPatient.checkupDate);

    }while (!validatetheDate(newPatient.checkupDate));
    

    FILE *data = fopen(csv, "a");

    if (data == NULL)
    {
        perror("Error opening file.");
        return;
    }

    fprintf(data, "%s %s,%d,%s,%s\n",
            newPatient.firstname,
            newPatient.lastname,
            newPatient.age,
            newPatient.healthStatus,
            newPatient.checkupDate);

    fclose(data);
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
    char row[200];
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
        char rowLower[200];
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
    char firstname[30], lastname[30];
    char row[200];
    int found = 0;
    Patient updatedPatient;

    FILE *data = fopen(csv, "r");
    FILE *temp = fopen("temp.csv", "w");

    if (data == NULL || temp == NULL)
    {
        printf("Error opening files.\n");
        return;
    }

    // search name to update => age, healthStatus and checkupDate
    printf("Enter First Name to update: ");
    scanf("%29s", firstname);
    printf("Enter Last Name to update: ");
    scanf("%29s", lastname);

    char searchName[50];
    sprintf(searchName, "%s %s", firstname, lastname);

    while (fgets(row, sizeof(row), data))
    {
        if (strncmp(row, searchName, strlen(searchName)) == 0)
        {
            printf("Enter new age: ");
            scanf("%d", &updatedPatient.age);

            printf("Enter new health status: ");
            scanf(" %49[^\n]", updatedPatient.healthStatus);

            printf("Enter new checkup date (YYYY-MM-DD): ");
            scanf("%10s", updatedPatient.checkupDate);

            fprintf(temp, "%s %s,%d,%s,%s\n",
                    firstname, lastname,
                    updatedPatient.age,
                    updatedPatient.healthStatus,
                    updatedPatient.checkupDate);
            found = 1;
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
    char row[200];
    int found = 0;

    FILE *data = fopen(csv, "r");
    FILE *temp = fopen("temp.csv", "w");

    if (data == NULL || temp == NULL)
    {
        printf("Error opening files.\n");
    }

    printf("Enter First Name to update: ");
    scanf("%29s", firstname);
    printf("Enter Last Name to update: ");
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
            char continues;
            printf("\nDo you want to continue program? (y/n): ");
            scanf(" %c", &continues);

            if (continues == 'n' || continues == 'N')
            {
                running = 0;
                exitMenu();
            }
            else if (continues == 'y' || continues == 'Y')
            {
                running = 1;
            }
        }
    }

    return 0;
}
