#ifndef PATIENT_H
#define PATIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define csv "Checkup-Data.csv"

typedef struct
{
    char firstname[60];
    char lastname[30];
    int age;
    char healthStatus[50];
    char checkupDate[11];
} Patient;

// Function prototypes

// Validation
int validatetheDate(const char *date);
int validateChar(const char *character_notnumber);
int validateNum(int *number_notstr, int min, int max);

// core functions
void displayallData();
Patient inputPatientData();
void addData();
void updateData();
void deleteData();
void searchData();

// helper function
void toLowerCase(char *str);
void clearScreen();
void exitMenu();

// test function
void runUnitTest();
void runE2eTest();


#endif 