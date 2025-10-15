#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "patient.h"

void trimNewline(char *str)
{
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n')
        str[len - 1] = '\0';
}


void runE2eTest()
{
    printf("===== Running E2E Test =====\n\n");

    printf("--- Add Data ---\n");
    Patient testPatient = {
        "Alice",
        "Smith",
        28,
        "Healthy",
        "2025-10-15"
    };

    FILE *data = fopen(csv, "a");
    if (data == NULL)
    {
        printf("Error: Cannot open file for E2E addData.\n");
        return;
    }

    fprintf(data, "%s,%s,%d,%s,%s\n",
            testPatient.firstname,
            testPatient.lastname,
            testPatient.age,
            testPatient.healthStatus,
            testPatient.checkupDate);
    fclose(data);
    printf("Added record: %s %s\n\n", testPatient.firstname, testPatient.lastname);

    printf("--- Search Data ---\n");
    char keyword[50] = "Alice";
    char row[1000];
    int found = 0;

    data = fopen(csv, "r");
    if (data == NULL)
    {
        printf("Error: Cannot open file for E2E searchData.\n");
        return;
    }

    while (fgets(row, sizeof(row), data))
    {
        char rowLower[1000];
        strcpy(rowLower, row);
        toLowerCase(rowLower);

        char keywordLower[50];
        strcpy(keywordLower, keyword);
        toLowerCase(keywordLower);

        if (strstr(rowLower, keywordLower) != NULL)
        {
            printf("Found record: %s", row);
            found = 1;
        }
    }
    if (!found)
        printf(" No matching record found.\n");
    fclose(data);
    printf("\n");

    printf("--- Update Data ---\n");
    char searchKeyword[] = "Alice";
    char tempRow[1000];
    int updated = 0;

    FILE *original = fopen(csv, "r");
    FILE *temp = fopen("temp.csv", "w");
    if (!original || !temp)
    {
        printf("Error: Cannot open files for E2E updateData.\n");
        return;
    }

    while (fgets(tempRow, sizeof(tempRow), original))
    {
        char rowCopy[1000];
        strcpy(rowCopy, tempRow);

        if (strstr(rowCopy, searchKeyword) != NULL)
        {
            updated = 1;
            printf("Updating record: %s", tempRow);

            char *token = strtok(tempRow, ",");
            char firstname[60]; strcpy(firstname, token);

            token = strtok(NULL, ",");
            char lastname[30]; strcpy(lastname, token);

            token = strtok(NULL, ",");
            int age = atoi(token);

            token = strtok(NULL, ",");
            char healthStatus[50]; strcpy(healthStatus, "Unhealthy");

            token = strtok(NULL, ",");
            char checkupDate[11]; trimNewline(token);
            strcpy(checkupDate, token);

            fprintf(temp, "%s,%s,%d,%s,%s\n",
                    firstname, lastname, age, healthStatus, checkupDate);
        }
        else
        {
            fputs(tempRow, temp);
        }
    }

    fclose(original);
    fclose(temp);

    remove(csv);
    rename("temp.csv", csv);

    if (updated)
        printf("Record updated successfully!\n\n");
    else
        printf("No record found to update.\n\n");

    printf("--- Delete Data ---\n");
    char firstname[60], lastname[30];
    strcpy(firstname, "Alice");
    strcpy(lastname, "Smith");
    toLowerCase(firstname);
    toLowerCase(lastname);

    original = fopen(csv, "r");
    temp = fopen("temp.csv", "w");
    if (!original || !temp)
    {
        printf("Error: Cannot open files for E2E deleteData.\n");
        return;
    }

    char rowLower[1000];
    int deleted = 0;

    while (fgets(tempRow, sizeof(tempRow), original))
    {
        char rowCopy[1000];
        strcpy(rowCopy, tempRow);
        trimNewline(rowCopy);

        toLowerCase(rowCopy);

        char *token = strtok(rowCopy, ",");
        char rowFirst[60]; strcpy(rowFirst, token);

        token = strtok(NULL, ",");
        char rowLast[30]; strcpy(rowLast, token);

        if (strcmp(rowFirst, firstname) == 0 && strcmp(rowLast, lastname) == 0)
        {
            printf("Deleting record: %s", tempRow);
            deleted = 1;
            continue; // skip writing this row
        }

        fputs(tempRow, temp);
    }

    fclose(original);
    fclose(temp);

    remove(csv);
    rename("temp.csv", csv);

    if (deleted)
        printf("Record deleted successfully!\n");
    else
        printf("No matching record found to delete.\n");

    printf("\n===== E2E Test Completed =====\n");
}
