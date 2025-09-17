#include <stdio.h>

void addData()
{
    char fullname[30];
    scanf("%s", fullname);
    

}

void searchData()
{
    // from name and health status
}

void updateData()
{

}

void deleteData()
{

}

void exitMenu()
{
    printf("Thank you for using the program.\n");
}

void displayMenu()
{
    char alphabet;
    scanf("%c", &alphabet);
    printf("Menu Diaplay\n a.Add Data\n b.Search Data\n c.Update Data\n d.Delete data\n e.Exit Menu\n ");
    printf("Please Enter an alphabet[a-e] to display a menu : %c");

    switch (alphabet)
    {
    case 'a':
        addData();
        break;
    case 'b':
        searchData();
        break;
    case 'c':
        updateData();
        break;

    case 'd':
        deleteData();
        break;

    default:
        exitMenu();
        break;
    }
}

int main(){
    displayMenu();

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