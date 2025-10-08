#ifndef PATIENT_H
#define PATIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct
{
    char firstname[30];
    char middlename[30];
    char lastname[30];
    char age[4];
    char healthStatus[50];
    char checkupDate[11];
} Patient;

extern Patient* patients;

int addDataRecord(const char *firstname, const char *lastname, const char *middlename,
const char *age, const char *healthStatus, const char *checkupDate, int *count);

#endif 