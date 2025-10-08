#include <assert.h>
#include <string.h>
#include <strings.h>ฃ
#include "patient.h"

#include "main.c"


patients = NULL;
// Counting result
static int tests_run = 0;
static int tests_passed = 0;
static int tests_failed = 0;

#define TEST_CSV "Test-Checkup-Data.csv"

#define test_assert(condition, message) \
    do { \
        tests_run++; \
        if (condition) { \
            printf("  ✓ PASS: %s\n", message); \
            tests_passed++; \
        } else { \
            printf("  ✗ FAIL: %s\n", message); \
            tests_failed++; \
        } \
    } while(0)


void test_addData(){
    int count = 0;
    patients = NULL;

    int read = addDataRecord("Alice", "Inith", "Wonderland", 40, "Healthy", "2025-08-12", &count);
    assert(read == 1);
    assert(count == 1);
    assert(strcmp(patients[0].name, "Alice") == 0);

    // duplicate
    read = addDataRecord("Alice", "Inith", "Wonderland", 40, "Healthy", "2025-08-12", &count);
    assert(read == 0);
    assert(count == 1);

    read = addDataRecord("Wanda", "Kennense", "Marlo", 40, "Unhealthy", "2025-02-", &count);
    assert(read == 1);
    assert(count == 2);

    free(patients);
}



// void newTestcsv(){
    
//     int count = 2;
//     patients = malloc(count * sizeof(Patient));

//     strcpy(patients[0].firstname, "Alice");
//     strcpy(patients[0].middlename, "Inith");
//     strcpy(patients[0].lastname, "Wonderland");
//     strcpy(patients[0].age, "40");
//     strcpy(patients[0].healthStatus, "Healthy");
//     strcpy(patients[0].checkupDate, "2025-08-12");

//     strcpy(patients[1].firstname, "Mercy");
//     strcpy(patients[1].middlename, "Maxus");
//     strcpy(patients[1].lastname, "Duplond");
//     strcpy(patients[1].age, "25");
//     strcpy(patients[1].healthStatus, "Unhealthy");
//     strcpy(patients[1].checkupDate, "2025-02-28");

//     save_csv("test.csv", count);

//     free(patients);

//     patients = NULL;
//     int new_count = 0;
//     load_csv("test.csv", &new_count);

//     assert(new_count == 2);
//     assert(strcmp(patients[0].name, "Alice") == 0);
//     assert(patients[0].age == 30);

//     remove("test.csv");
//     free(patients);



// }


#ifdef unit_test_only
int main() {
    
    test_addData();
    // test_searchData();
    // test_updateData();
    // test_deleteData();
    // test_validatetheDate();
    // test_newTestcsv();

    printf("Congratulations! All the tests passed!\n");
    return (tests_failed == 0) ? 0 : 1;
}
#endif