#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "patient.h"

Patient *patients = NULL;

int validateNum(const char *number_str)
{
    if (number_str == NULL || number_str[0] == '\0') return 0;
    for (int i = 0; number_str[i] != '\0'; i++) {
        if (number_str[i] < '0' || number_str[i] > '9') return 0;
    }
    int num = atoi(number_str);
    if (num < 1 || num > 150) return 0;
    return 1;
}

// add data
int addDataRecord(const char *firstname, const char *middlename, const char *lastname, const char *age, const char *healthStatus, const char *checkupDate, int *count)
{

    // check validateNum
    for (int i = 0; age[i] != '\0'; i++)
    {
        if (age[i] < '0' || age[i] > '9')
        {
            return -1;
        }
    }

    // check duplicate data
    for (int i = 0; i < *count; i++)
    {
        if (strcmp(patients[i].firstname, firstname) == 0 &&
            strcmp(patients[i].middlename, middlename) == 0 &&
            strcmp(patients[i].lastname, lastname) == 0 &&
            strcmp(patients[i].checkupDate, checkupDate) == 0)
        {
            return 0;
        }
    }

    // add new record
    Patient *temp = realloc(patients, (*count + 1) * sizeof(Patient));
    if (!temp)
        return -1;
    patients = temp;

    strcpy(patients[*count].firstname, firstname);
    strcpy(patients[*count].middlename, middlename);
    strcpy(patients[*count].lastname, lastname);
    strcpy(patients[*count].age, age);
    strcpy(patients[*count].healthStatus, healthStatus);
    strcpy(patients[*count].checkupDate, checkupDate);

    (*count)++;
    return 1;
}

// search Data
Patient* searchDataRecord(const char *firstname, const char *middlename, const char *lastname, int count)
{
    for (int i = 0; i < count; i++)
    {   
        if (strcmp(patients[i].firstname, firstname) == 0 &&
            strcmp(patients[i].middlename, middlename) == 0 &&
            strcmp(patients[i].lastname, lastname) == 0)
        {
            return &patients[i];
        }
    }
    return NULL;
}

// update Data
int updateDataRecord(const char *firstname, const char *middlename, const char *lastname, const char *newAge, const char *newhealthStatus, const char *newcheckupDate, int count) {

    if (!validateNum(newAge)) {
        return -1; // Invalid age format or range
    }

    Patient* p = searchDataRecord(firstname, middlename, lastname, count);
    if (p == NULL) {
        return 0; // Record not found
    }

    strcpy(p->age, newAge);
    strcpy(p->healthStatus, newhealthStatus);
    strcpy(p->checkupDate, newcheckupDate);

    return 1; // Success
}


//delete Data
int deleteDataRecord(const char* firstname, const char* lastname, int *count) {
    for (int i = 0; i < *count; i++) {
        if (strcmp(patients[i].firstname, firstname) == 0 &&
            strcmp(patients[i].lastname, lastname) == 0) {

            for (int j = i; j < *count - 1; j++) {
                patients[j] = patients[j + 1];
            }

            (*count)--;

            if (*count == 0) {
                free(patients);
                patients = NULL;
            } else {
                patients = realloc(patients, (*count) * sizeof(Patient));
            }

            return 1;
        }
    }

    return 0;
}