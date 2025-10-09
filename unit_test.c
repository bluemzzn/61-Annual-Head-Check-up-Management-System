#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "patient.h" // ใช้ header ที่แก้ไขแล้ว

// --- Test Helper ---
void setup() {
    if (patients != NULL) {
        free(patients);
        patients = NULL;
    }
}

// --- Test Cases ---

void test_validateNum() {
    printf("Testing validateNum...\n");
    // Normal Case
    assert(validateNum("30") == 1);
    assert(validateNum("1") == 1);
    assert(validateNum("150") == 1);

    // Boundary Case
    assert(validateNum("0") == 0); // Below lower bound
    assert(validateNum("151") == 0); // Above upper bound

    // Extreme/Error Case
    assert(validateNum("-5") == 0); // Negative number
    assert(validateNum("abc") == 0); // Non-numeric
    assert(validateNum("25a") == 0); // Mixed characters
    assert(validateNum("") == 0);   // Empty string
    assert(validateNum(NULL) == 0); // Null input
    printf("All validateNum tests passed!\n\n");
}


void test_addDataRecord() {
    printf("Testing addDataRecord...\n");
    setup();
    int count = 0;

    // Normal Case: Add a valid record
    int result = addDataRecord("John", "F", "Doe", "35", "Good", "2025-10-09", &count);
    assert(result == 1 && "Should add new record successfully");
    assert(count == 1 && "Count should be 1");
    assert(strcmp(patients[0].firstname, "John") == 0);

    // Normal Case: Add another record
    result = addDataRecord("Jane", "", "Smith", "28", "Excellent", "2025-11-15", &count);
    assert(result == 1 && "Should add a second record");
    assert(count == 2 && "Count should be 2");
    assert(strcmp(patients[1].lastname, "Smith") == 0);

    // Boundary/Error Case: Add a duplicate record
    result = addDataRecord("John", "F", "Doe", "35", "Good", "2025-10-09", &count);
    assert(result == 0 && "Should detect duplicate record");
    assert(count == 2 && "Count should not change after duplicate");

    // Extreme/Error Case: Add record with invalid age (non-numeric)
    result = addDataRecord("Test", "", "Invalid", "abc", "N/A", "2025-01-01", &count);
    assert(result == -1 && "Should return error for invalid age string");
    assert(count == 2 && "Count should not change after failed add");

    printf("All addDataRecord tests passed!\n\n");
}


void test_searchDataRecord() {
    printf("Testing searchDataRecord...\n");
    setup();
    int count = 0;

    // Setup data
    addDataRecord("Clark", "J", "Kent", "33", "Super", "1938-04-18", &count);
    addDataRecord("Bruce", "", "Wayne", "40", "Rich", "1939-03-30", &count);

    // Normal Case: Find an existing record
    Patient* found = searchDataRecord("Bruce", "", "Wayne", count);
    assert(found != NULL && "Should find existing record");
    assert(strcmp(found->healthStatus, "Rich") == 0);
    
    // Normal Case: Find another existing record with middle name
    found = searchDataRecord("Clark", "J", "Kent", count);
    assert(found != NULL && "Should find record with middle name");
    assert(strcmp(found->age, "33") == 0);

    // Boundary/Error Case: Search for a non-existent record
    found = searchDataRecord("Peter", "", "Parker", count);
    assert(found == NULL && "Should not find non-existent record");

    // Extreme/Error Case: Search in an empty list
    setup();
    count = 0;
    found = searchDataRecord("Nobody", "", "Here", count);
    assert(found == NULL && "Should return NULL when searching in empty list");
    
    printf("All searchDataRecord tests passed!\n\n");
}

void test_updateDataRecord() {
    printf("Testing updateDataRecord...\n");
    setup();
    int count = 0;
    addDataRecord("Tony", "E", "Stark", "45", "Genius", "2008-05-02", &count);

    // Normal Case: Update an existing record
    int result = updateDataRecord("Tony", "E", "Stark", "50", "Iron Man", "2024-01-01", count);
    assert(result == 1 && "Should successfully update");
    assert(strcmp(patients[0].age, "50") == 0);
    assert(strcmp(patients[0].healthStatus, "Iron Man") == 0);

    // Boundary/Error Case: Try to update a non-existent record
    result = updateDataRecord("Steve", "", "Rogers", "105", "Captain", "1941-03-01", count);
    assert(result == 0 && "Should return 0 for non-existent record");

    // Extreme/Error Case: Update with invalid data
    result = updateDataRecord("Tony", "E", "Stark", "999", "Too Old", "2030-01-01", count);
    assert(result == -1 && "Should return -1 for invalid new age");
    assert(strcmp(patients[0].age, "50") == 0 && "Data should not change after failed update");

    printf("All updateDataRecord tests passed!\n\n");
}

void test_deleteDataRecord() {
    printf("Testing deleteDataRecord...\n");
    setup();
    int count = 0;
    addDataRecord("A", "", "1", "10", "A1", "2025-01-01", &count);
    addDataRecord("B", "", "2", "20", "B2", "2025-02-02", &count);
    addDataRecord("C", "", "3", "30", "C3", "2025-03-03", &count);
    
    // Normal Case: Delete from the middle
    int result = deleteDataRecord("B", "2", &count);
    assert(result == 1 && "Should delete record successfully");
    assert(count == 2 && "Count should be 2 after delete");
    assert(strcmp(patients[0].firstname, "A") == 0);
    assert(strcmp(patients[1].firstname, "C") == 0);

    // Boundary/Error Case: Delete a non-existent record
    result = deleteDataRecord("D", "4", &count);
    assert(result == 0 && "Should return 0 for non-existent record");
    assert(count == 2 && "Count should not change");
    
    // Normal Case: Delete the last element
    result = deleteDataRecord("C", "3", &count);
    assert(result == 1 && "Should delete last record");
    assert(count == 1 && "Count should be 1");
    
    // Boundary Case: Delete the only remaining element
    result = deleteDataRecord("A", "1", &count);
    assert(result == 1 && "Should delete the only record");
    assert(count == 0 && "Count should be 0");
    assert(patients == NULL && "Patients pointer should be NULL after last delete");
    
    printf("All deleteDataRecord tests passed!\n\n");
}

void run_unit(){
    test_validateNum();
    test_addDataRecord();
    test_searchDataRecord();
    test_updateDataRecord();
    test_deleteDataRecord();

    printf("=======================================\n");
    printf("  All test suites passed successfully! \n");
    printf("=======================================\n");

}


int main_unit() {
    run_unit();
    return 0;
}