#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "e2e_header.h"
#include "patient.h" // Include to get the definition of the Patient struct

#define csv "Checkup-Data.csv"

// Helper function to find a specific record in the CSV file for verification.
// Returns 1 if found, 0 if not found.
static int findRecordInFile(const char* firstname, const char* lastname, const char* healthStatus) {
    FILE* data = fopen(csv, "r");
    if (data == NULL) {
        return 0; // File doesn't exist, so record not found
    }

    char row[1000];
    int found = 0;
    while (fgets(row, sizeof(row), data)) {
        // Use strstr for a simple substring check. A more robust check would parse columns.
        if (strstr(row, firstname) != NULL &&
            strstr(row, lastname) != NULL &&
            strstr(row, healthStatus) != NULL) {
            found = 1;
            break;
        }
    }

    fclose(data);
    return found;
}

// Non-interactive version of addData for testing
static void test_addRecord(const Patient* p) {
    FILE* data = fopen(csv, "a");
    if (data == NULL) {
        printf("E2E_ERROR: Could not open file to add test record.\n");
        return;
    }
    fprintf(data, "%s,%s,%s,%d,%s,%s\n",
            p->firstname, p->middlename, p->lastname,
            p->age, p->healthStatus, p->checkupDate);
    fclose(data);
}

// Non-interactive version of updateData for testing
static void test_updateRecord(const char* oldFirstname, const char* oldLastname, const Patient* newPatient) {
    FILE* data = fopen(csv, "r");
    FILE* temp = fopen("temp_e2e.csv", "w");

    if (data == NULL || temp == NULL) {
        printf("E2E_ERROR: Error opening files for update.\n");
        if (data) fclose(data);
        if (temp) fclose(temp);
        return;
    }

    char row[1000];
    while (fgets(row, sizeof(row), data)) {
        char tmpRow[1000];
        strcpy(tmpRow, row);

        char* token = strtok(tmpRow, ",");
        strtok(NULL, ","); // Skip middle name
        char* lastToken = strtok(NULL, ",");

        if (token && lastToken && strcasecmp(token, oldFirstname) == 0 && strcasecmp(lastToken, oldLastname) == 0) {
            // This is the record to update, write the new data instead
            fprintf(temp, "%s,%s,%s,%d,%s,%s\n",
                    newPatient->firstname, newPatient->middlename, newPatient->lastname,
                    newPatient->age, newPatient->healthStatus, newPatient->checkupDate);
        } else {
            // Copy the existing row
            fprintf(temp, "%s", row);
        }
    }

    fclose(data);
    fclose(temp);

    remove(csv);
    rename("temp_e2e.csv", csv);
}

// Non-interactive version of deleteData for testing
static void test_deleteRecord(const char* firstname, const char* lastname) {
    FILE* data = fopen(csv, "r");
    FILE* temp = fopen("temp_e2e.csv", "w");

    if (data == NULL || temp == NULL) {
        printf("E2E_ERROR: Error opening files for delete.\n");
        if (data) fclose(data);
        if (temp) fclose(temp);
        return;
    }

    char row[1000];
    while (fgets(row, sizeof(row), data)) {
        char tmpRow[1000];
        strcpy(tmpRow, row);

        char* token = strtok(tmpRow, ",");
        strtok(NULL, ","); // Skip middle name
        char* lastToken = strtok(NULL, ",");

        if (token && lastToken && strcasecmp(token, firstname) == 0 && strcasecmp(lastToken, lastname) == 0) {
            // This is the record to delete, so we do nothing (skip writing it)
            continue;
        } else {
            // Copy the existing row
            fprintf(temp, "%s", row);
        }
    }

    fclose(data);
    fclose(temp);

    remove(csv);
    rename("temp_e2e.csv", csv);
}

// Main E2E test runner function
void run_end_to_end_test() {
    printf("\n--- Starting End-to-End Test ---\n");

    // 1. Setup: Backup original file and create a clean slate for the test
    printf("SETUP: Backing up current data file...\n");
    rename(csv, "Checkup-Data.csv.bak");
    FILE *testFile = fopen(csv, "w");
    if (testFile == NULL) {
        printf("TEST FAILED: Could not create test file.\n");
        rename("Checkup-Data.csv.bak", csv); // Restore backup
        return;
    }
    fclose(testFile);

    // --- Test Data ---
    Patient testPatient = {"John", "Fitzgerald", "Doe", "45", "Good Condition", "2025-05-10"};
    Patient updatedPatient = {"John", "F", "Doe", "46", "Excellent Condition", "2025-06-11"};
    int test_passed = 1;

    // 2. Test Case 1: Add a new record
    printf("\nTEST 1: Adding a new record for '%s %s'...\n", testPatient.firstname, testPatient.lastname);
    test_addRecord(&testPatient);
    if (findRecordInFile(testPatient.firstname, testPatient.lastname, testPatient.healthStatus)) {
        printf("  [PASS] Record added successfully.\n");
    } else {
        printf("  [FAIL] Record not found after adding.\n");
        test_passed = 0;
    }

    // 3. Test Case 2: Update the existing record
    printf("\nTEST 2: Updating record for '%s %s'...\n", testPatient.firstname, testPatient.lastname);
    test_updateRecord(testPatient.firstname, testPatient.lastname, &updatedPatient);
    if (findRecordInFile(updatedPatient.firstname, updatedPatient.lastname, updatedPatient.healthStatus)) {
        printf("  [PASS] Record updated successfully.\n");
    } else {
        printf("  [FAIL] Updated record not found.\n");
        test_passed = 0;
    }
    if (!findRecordInFile(testPatient.firstname, testPatient.lastname, testPatient.healthStatus)) {
        printf("  [PASS] Old record data is no longer present.\n");
    } else {
        printf("  [FAIL] Old record data still exists after update.\n");
        test_passed = 0;
    }

    // 4. Test Case 3: Delete the record
    printf("\nTEST 3: Deleting record for '%s %s'...\n", updatedPatient.firstname, updatedPatient.lastname);
    test_deleteRecord(updatedPatient.firstname, updatedPatient.lastname);
    if (!findRecordInFile(updatedPatient.firstname, updatedPatient.lastname, updatedPatient.healthStatus)) {
        printf("  [PASS] Record deleted successfully.\n");
    } else {
        printf("  [FAIL] Record still exists after deletion.\n");
        test_passed = 0;
    }

    // 5. Teardown: Clean up and restore backup
    printf("\nTEARDOWN: Cleaning up and restoring original data file...\n");
    remove(csv);
    rename("Checkup-Data.csv.bak", csv);

    printf("\n--- End-to-End Test Finished ---\n");
    if (test_passed) {
        printf("Overall Result: ALL TESTS PASSED\n");
    } else {
        printf("Overall Result: SOME TESTS FAILED\n");
    }
}

int main(){
    run_end_to_end_test();

    return 0;
}