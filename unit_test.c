#include <stdio.h>
#include <string.h>
#include "patient.h"

// ฟังก์ชันทดสอบ addData
void testAddData()
{
    printf("----- Unit Test: Add Data -----\n");

    // ตัวอย่างข้อมูล
    Patient testPatient = {
        "John",
        "Doe",
        30,
        "Healthy",
        "2025-10-15"
    };

    // เขียนลงไฟล์ CSV
    FILE *data = fopen(csv, "a");
    if (data == NULL)
    {
        printf("Error: Cannot open file for testing addData.\n");
        return;
    }

    fprintf(data, "%s,%s,%d,%s,%s\n",
            testPatient.firstname,
            testPatient.lastname,
            testPatient.age,
            testPatient.healthStatus,
            testPatient.checkupDate);
    fclose(data);

    printf("Test addData: Record added successfully!\n");
}

// ฟังก์ชันทดสอบ updateData
void testUpdateData()
{
    printf("----- Unit Test: Update Data -----\n");

    char keyword[] = "John"; // ค้นหาชื่อ John
    char row[1000];
    int found = 0;

    FILE *data = fopen(csv, "r");
    if (data == NULL)
    {
        printf("Error: Cannot open file for testing updateData.\n");
        return;
    }

    FILE *temp = fopen("temp.csv", "w");
    if (temp == NULL)
    {
        printf("Error: Cannot create temp file for updateData.\n");
        fclose(data);
        return;
    }

    while (fgets(row, sizeof(row), data))
    {
        char rowCopy[1000];
        strcpy(rowCopy, row);

        if (strstr(rowCopy, keyword) != NULL)
        {
            found = 1;
            printf("Updating record: %s", row);

            // เปลี่ยน HealthStatus เป็น "Sick" เป็นตัวอย่าง
            char *token = strtok(row, ","); // firstname
            char firstname[60]; strcpy(firstname, token);

            token = strtok(NULL, ","); // lastname
            char lastname[30]; strcpy(lastname, token);

            token = strtok(NULL, ","); // age
            int age = atoi(token);

            token = strtok(NULL, ","); // healthStatus
            char healthStatus[50]; strcpy(healthStatus, "Sick");

            token = strtok(NULL, ","); // checkupDate
            char checkupDate[11]; strcpy(checkupDate, token);

            fprintf(temp, "%s,%s,%d,%s,%s\n",
                    firstname, lastname, age, healthStatus, checkupDate);
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
        printf("Test updateData: Record updated successfully!\n");
    else
        printf("Test updateData: No matching record found.\n");
}

// ฟังก์ชันเรียกทดสอบ
void runUnitTest()
{
    printf("===== Running Unit Tests =====\n\n");
    testAddData();
    testUpdateData();
    printf("\n===== Unit Tests Completed =====\n");
}
