#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "patient.h" 

#define TEST_CSV "test.csv"


void test_addData()
{
    printf("Running test_addData...\n");

    remove(TEST_CSV);

    Patient p;
    strcpy(p.firstname, "John");
    strcpy(p.lastname, "Doe");
    p.age = 30;
    strcpy(p.healthStatus, "Unhealthy");
    strcpy(p.checkupDate, "2025-10-15");

    FILE *data = fopen(TEST_CSV, "a");
    if (!data)
    {
        printf("Cannot open test file.\n");
        return;
    }
    fprintf(data, "%s,%s,%d,%s,%s\n", p.firstname, p.lastname, p.age, p.healthStatus, p.checkupDate);
    fclose(data);

    char row[200];
    int found = 0;
    data = fopen(TEST_CSV, "r");
    while (fgets(row, sizeof(row), data))
    {
        if (strstr(row, "John") && strstr(row, "Doe"))
        {
            found = 1;
            break;
        }
    }
    fclose(data);

    if (found)
        printf("test_addData PASSED!\n");
    else
        printf("test_addData FAILED!\n");
}

void test_updateData()
{
    printf("Running test_updateData...\n");

    Patient p;
    strcpy(p.firstname, "Alice");
    strcpy(p.lastname, "Smith");
    p.age = 25;
    strcpy(p.healthStatus, "Healthy");
    strcpy(p.checkupDate, "2025-10-01");

    FILE *data = fopen(TEST_CSV, "w"); 
    fprintf(data, "%s,%s,%d,%s,%s\n", p.firstname, p.lastname, p.age, p.healthStatus, p.checkupDate);
    fclose(data);

    FILE *in = fopen(TEST_CSV, "r");
    FILE *temp = fopen("temp.csv", "w");
    char row[200];
    int found = 0;
    while (fgets(row, sizeof(row), in))
    {
        if (strstr(row, "Alice") && strstr(row, "Smith") && !found)
        {
            found = 1;
            fprintf(temp, "Alice,%s,26,Healthy,2025-10-10\n", "Smith");
        }
        else
        {
            fputs(row, temp);
        }
    }
    fclose(in);
    fclose(temp);
    remove(TEST_CSV);
    rename("temp.csv", TEST_CSV);

    data = fopen(TEST_CSV, "r");
    found = 0;
    while (fgets(row, sizeof(row), data))
    {
        if (strstr(row, "Alice") && strstr(row, "26") && strstr(row, "Healthy"))
        {
            found = 1;
            break;
        }
    }
    fclose(data);

    if (found)
        printf("test_updateData PASSED!\n");
    else
        printf("test_updateData FAILED!\n");
}

int main_program()
{
    test_addData();
    test_updateData();

    printf("All unit tests completed.\n");
    return 0;
}
