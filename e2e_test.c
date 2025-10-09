#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "patient.h"

#define e2e_csv "e2e_data.csv"


//for set up memory 
void setup_memory() {
    if (patients != NULL) {
        free(patients);
        patients = NULL;
    }
}

void save_data_to_csv(int count) {
    FILE *file = fopen(e2e_csv, "w");
    if (file == NULL) {
        printf("Error: Cannot open file for writing.\n");
        exit(1);
    }
    for (int i = 0; i < count; i++) {
        fprintf(file, "%s,%s,%s,%s,%s,%s\n",
                patients[i].firstname,
                patients[i].middlename,
                patients[i].lastname,
                patients[i].age,
                patients[i].healthStatus,
                patients[i].checkupDate);
    }
    fclose(file);
}

void load_data_from_csv(int *count) {
    FILE *file = fopen(e2e_csv, "r");
    if (file == NULL) {
        return;
    }
    
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        char firstname[30], middlename[30], lastname[30], age[4], status[50], date[11];
        
        line[strcspn(line, "\n")] = 0;

        // แยกข้อมูลด้วย sscanf (อาจต้องปรับ format string หากข้อมูลซับซ้อน)
        if (sscanf(line, "%[^,],%[^,],%[^,],%[^,],%[^,],%s",
                   firstname, middlename, lastname, age, status, date) == 6) {
            addDataRecord(firstname, middlename, lastname, age, status, date, count);
        }
    }
    fclose(file);
}


void run_e2e_workflow() {
    printf("--- Starting E2E Test Workflow ---\n\n");
    
    int count = 0;
    setup_memory();

    // --== 1. ADD & SAVE ==--
    printf("Step 1: Adding new record 'Diana Prince'...\n");
    addDataRecord("Diana", "", "Prince", "30", "Healthy", "2025-10-09", &count);
    assert(count == 1);
    printf("Step 2: Saving to %s...\n", e2e_csv);
    save_data_to_csv(count);

    // --== 2. RELOAD & VERIFY ==--
    printf("Step 3: Clearing memory and reloading from file...\n");
    setup_memory();
    count = 0;
    load_data_from_csv(&count);
    assert(count == 1 && "Data should be reloaded");
    assert(strcmp(patients[0].firstname, "Diana") == 0 && "Firstname should match");
    printf("   ✓ Verification successful!\n\n");

    // --== 3. UPDATE & SAVE ==--
    printf("Step 4: Updating record's health status...\n");
    updateDataRecord("Diana", "", "Prince", "31", "Amazonian", "2025-10-10", count);
    printf("Step 5: Saving updated data to %s...\n", e2e_csv);
    save_data_to_csv(count);

    // --== 4. RELOAD & VERIFY UPDATE ==--
    printf("Step 6: Clearing memory and reloading updated data...\n");
    setup_memory();
    count = 0;
    load_data_from_csv(&count);
    assert(count == 1 && "Updated data should be reloaded");
    assert(strcmp(patients[0].healthStatus, "Amazonian") == 0 && "Health status should be updated");
    assert(strcmp(patients[0].age, "31") == 0 && "Age should be updated");
    printf("   ✓ Verification successful!\n\n");

    // --== 5. SEARCH ==--
    printf("Step 7: Searching for the updated record...\n");
    Patient* found = searchDataRecord("Diana", "", "Prince", count);
    assert(found != NULL && "Should find the record after reloading");
    printf("   ✓ Record found!\n\n");

    // --== 6. DELETE & SAVE ==--
    printf("Step 8: Deleting the record...\n");
    deleteDataRecord("Diana", "Prince", &count);
    assert(count == 0 && "Count should be 0 after deletion");
    printf("Step 9: Saving the empty list to %s...\n", e2e_csv);
    save_data_to_csv(count);
    
    // --== 7. FINAL RELOAD & VERIFY DELETION ==--
    printf("Step 10: Clearing memory and reloading to verify deletion...\n");
    setup_memory();
    count = 0;
    load_data_from_csv(&count);
    assert(count == 0 && "Data should be empty after deletion is saved and reloaded");
    printf("   ✓ Verification successful! Record is permanently deleted.\n\n");

    printf("--- E2E Test Workflow Passed! ---\n");
}


int main() {
    remove(e2e_csv);
    
    run_e2e_workflow();

    remove(e2e_csv);

    return 0;
}