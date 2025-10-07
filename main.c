#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define csv "Checkup-Data.csv"

// วิธี compile และ run:
// compile: gcc -o checkup.c
// run: ./checkup

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
        printf("Please input the correct format (YYYY-MM-DD).\n");
        return 0;
    }

    for (int i = 0; i < 10; i++)
    {
        if (i == 4 || i == 7)
        {
            if (date[i] != '-')
            {
                printf("Invalid format. Use YYYY-MM-DD.\n");
                return 0;
            }
        }
        else
        {
            if (date[i] < '0' || date[i] > '9')
            {
                printf("Invalid character in date.\n");
                return 0;
            }
        }
    }

    int year, month, day;
    if (sscanf(date, "%4d-%2d-%2d", &year, &month, &day) != 3)
        return 0;

    if (year < 1900 || year > 2100)
    {
        printf("Year must be between 1900 and 2100.\n");
        return 0;
    }

    if (month < 1 || month > 12)
    {
        printf("Invalid month (1-12 only).\n");
        return 0;
    }

    int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
    {
        daysInMonth[1] = 29;
    }

    if (day < 1 || day > daysInMonth[month - 1])
    {
        printf("Invalid day for the given month (%d/%d).\n", day, month);
        return 0;
    }

    return 1;
}

int validateChar(const char *character_notnum)
{
    if (character_notnum[0] == '\0')
    {
        printf("Input cannot be empty.\n");
        return 0;
    }

    for (int i = 0; character_notnum[i] != '\0'; i++)
    {
        if (!isalpha(character_notnum[i]) && character_notnum[i] != ' ')
        {
            printf("Please enter only letters.\n");
            return 0;
        }
    }
    return 1;
}

int validateNum(const char *number_str)
{
    if (number_str[0] == '\0')
    {
        printf("Input cannot be empty.\n");
        return 0;
    }

    for (int i = 0; number_str[i] != '\0'; i++)
    {
        if (number_str[i] < '0' || number_str[i] > '9')
        {
            printf("Please enter only numbers.\n");
            return 0;
        }
    }

    int num = atoi(number_str);
    if (num < 1 || num > 150)
    {
        printf("Age must be between 1 and 150.\n");
        return 0;
    }

    return 1;
}

int checkDuplicate(const Patient *p)
{
    FILE *data = fopen(csv, "r");
    if (data == NULL)
        return 0;

    char row[1000];
    char f[30], m[30], l[30];
    int age;
    char status[50], date[11];

    while (fgets(row, sizeof(row), data))
    {
        if (sscanf(row, "%29[^,],%29[^,],%29[^,],%d,%49[^,],%10[^\n]",
                   f, m, l, &age, status, date) >= 5)
        {
            if (strcasecmp(f, p->firstname) == 0 &&
                strcasecmp(m, p->middlename) == 0 &&
                strcasecmp(l, p->lastname) == 0 &&
                strcmp(date, p->checkupDate) == 0)
            {
                fclose(data);
                return 1;
            }
        }
    }

    fclose(data);
    return 0;
}

void addData()
{
    Patient newPatient;
    char middlename;
    char ageStr[10];

    printf("\n--- Add New Patient ---\n");

    do
    {
        printf("Enter Firstname: ");
        scanf("%29s", newPatient.firstname);
    } while (!validateChar(newPatient.firstname));

    do
    {
        printf("Do you want to input middlename? (y/n): ");
        scanf(" %c", &middlename);

        if (middlename == 'n' || middlename == 'N')
        {
            strcpy(newPatient.middlename, "");
            break;
        }
        else if (middlename == 'y' || middlename == 'Y')
        {
            do
            {
                printf("Enter Middlename: ");
                scanf("%29s", newPatient.middlename);
            } while (!validateChar(newPatient.middlename));
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
        scanf("%9s", ageStr);
    } while (!validateNum(ageStr));
    newPatient.age = atoi(ageStr);

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

    if (checkDuplicate(&newPatient))
    {
        printf("\nThis record already exists. Please enter a different person/date.\n");
        return;
    }

    FILE *data = fopen(csv, "a");
    if (data == NULL)
    {
        printf("Error opening file.\n");
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
    printf("\nRecord added successfully!\n");
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

    printf("\n--- All Patient Records ---\n");
    printf("%-15s %-15s %-15s %-5s %-20s %-12s\n",
           "Firstname", "Middlename", "Lastname", "Age", "Health Status", "Checkup Date");
    printf("--------------------------------------------------------------------------------\n");

    int count = 0;
    while (fgets(row, sizeof(row), data))
    {
        printf("%s", row);
        count++;
    }

    if (count == 0)
    {
        printf("No records found.\n");
    }

    fclose(data);
}

void toLowerCase(char *str)
{
    for (; *str; str++)
    {
        *str = tolower(*str);
    }
}

void searchData()
{
    char keyword[50];
    char row[1000];
    int found = 0;

    printf("\n--- Search Records ---\n");
    printf("Enter search keyword: ");
    scanf(" %[^\n]", keyword);

    toLowerCase(keyword);

    FILE *data = fopen(csv, "r");

    if (data == NULL)
    {
        printf("Cannot open the file.\n");
        return;
    }

    printf("\n--- Search Results ---\n");

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
    char row[1000];
    int found = 0;
    Patient updatedPatient;

    FILE *data = fopen(csv, "r");
    FILE *temp = fopen("temp.csv", "w");

    if (data == NULL || temp == NULL)
    {
        printf("Error opening files.\n");
        if (data)
            fclose(data);
        if (temp)
            fclose(temp);
        return;
    }

    char oldFirstname[30], oldLastname[30];
    printf("\n--- Update Record ---\n");
    printf("Enter First Name to update: ");
    scanf("%29s", oldFirstname);
    printf("Enter Last Name to update: ");
    scanf("%29s", oldLastname);

    while (fgets(row, sizeof(row), data))
    {
        char tmpRow[1000];
        strcpy(tmpRow, row);

        char *token = strtok(tmpRow, ",");
        char *midToken = strtok(NULL, ",");
        char *lastToken = strtok(NULL, ",");

        if (token && lastToken &&
            strcasecmp(token, oldFirstname) == 0 &&
            strcasecmp(lastToken, oldLastname) == 0)
        {
            found = 1;

            printf("\nRecord found. Enter new information:\n");

            do
            {
                printf("Enter new Firstname: ");
                scanf("%29s", updatedPatient.firstname);
            } while (!validateChar(updatedPatient.firstname));

            do
            {
                printf("Do you want to input Middlename? (y/n): ");
                char choice;
                scanf(" %c", &choice);
                if (choice == 'n' || choice == 'N')
                {
                    strcpy(updatedPatient.middlename, "");
                    break;
                }
                else if (choice == 'y' || choice == 'Y')
                {
                    do
                    {
                        printf("Enter Middlename: ");
                        scanf("%29s", updatedPatient.middlename);
                    } while (!validateChar(updatedPatient.middlename));
                    break;
                }
                else
                {
                    printf("Please enter y or n only.\n");
                }
            } while (1);

            do
            {
                printf("Enter new Lastname: ");
                scanf("%29s", updatedPatient.lastname);
            } while (!validateChar(updatedPatient.lastname));

            char ageStr[10];
            do
            {
                printf("Enter new Age: ");
                scanf("%9s", ageStr);
            } while (!validateNum(ageStr));
            updatedPatient.age = atoi(ageStr);

            do
            {
                printf("Enter new Health Status: ");
                scanf(" %49[^\n]", updatedPatient.healthStatus);
            } while (!validateChar(updatedPatient.healthStatus));

            do
            {
                printf("Enter Checkup Date (YYYY-MM-DD): ");
                scanf("%10s", updatedPatient.checkupDate);
            } while (!validatetheDate(updatedPatient.checkupDate));

            if (checkDuplicate(&updatedPatient))
            {
                printf("\nThis updated record would create a duplicate. Update cancelled.\n");
                fprintf(temp, "%s", row);
            }
            else
            {
                fprintf(temp, "%s,%s,%s,%d,%s,%s\n",
                        updatedPatient.firstname,
                        updatedPatient.middlename,
                        updatedPatient.lastname,
                        updatedPatient.age,
                        updatedPatient.healthStatus,
                        updatedPatient.checkupDate);
            }
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
        printf("\nRecord updated successfully.\n");
    }
    else
    {
        remove("temp.csv");
        printf("\nRecord not found.\n");
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
        if (data)
            fclose(data);
        if (temp)
            fclose(temp);
        return;
    }

    printf("\n--- Delete Record ---\n");
    printf("Enter First Name to delete: ");
    scanf("%29s", firstname);
    printf("Enter Last Name to delete: ");
    scanf("%29s", lastname);

    while (fgets(row, sizeof(row), data))
    {
        char tmpRow[1000];
        strcpy(tmpRow, row);

        char *token = strtok(tmpRow, ",");
        char *midToken = strtok(NULL, ",");
        char *lastToken = strtok(NULL, ",");

        if (token && lastToken &&
            strcasecmp(token, firstname) == 0 &&
            strcasecmp(lastToken, lastname) == 0)
        {
            found = 1;
            printf("Record deleted: %s %s %s\n", token, midToken ? midToken : "", lastToken);
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
        printf("\nRecord deleted successfully.\n");
    }
    else
    {
        remove("temp.csv");
        printf("\nRecord not found.\n");
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
    printf("\nThank you for using the Annual Health Checkup Management System.\n");
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