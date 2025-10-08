#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "patient.h"

Patient *patients = NULL;

// add data
int addDataRecord(const char *firstname, const char *lastname, const char *middlename,
                  const char *age, const char *healthStatus, const char *checkupDate, int *count)
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
Patient *searchDataRecord(const char *firstname, const char *lastname, const char *middlename, int *count)
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
int updateDataRecord(const char* firstname, const char* middlename,
                     const char* lastname, const char* newAge,
                     const char* newhealthStatus, const char* newcheckupDate, int count) {

    if (!validateNum(newAge)) {
        return -1; 
    }

    Patient* p = searchDataRecord(firstname, lastname, count);
    if (p == NULL) {
        return 0; 
    }

    strcpy(p->age, newAge);
    strcpy(p->healthStatus, newhealthStatus);
    strcpy(p->checkupDate, newcheckupDate);

    return 1; 
}

//delete Data
int deleteDataRecord(const char* firstname, const char* lastname, int* count) {
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