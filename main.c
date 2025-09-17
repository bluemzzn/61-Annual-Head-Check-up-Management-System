#include <stdio.h>

// void addData()
// {
// }

// void searchData()
// {
//     // from name and health status
// }

// void updateData()
// {
// }

// void deleteData()
// {
// }

int main()
{

    FILE *data = fopen("Checkup-Data.csv", "r");

    // check file opened
    if (data == NULL)
    {
        printf("File: %p\n", data);
        printf("cannot open the file");

        return 1;
    }

    char line[100];
    while (fgets(line, sizeof(line), data) != NULL)
    {
        printf("อ่านบรรทัด: %s", line); // แสดงผลบรรทัดที่อ่านได้
    }

    fclose(data);

    return 0;
}