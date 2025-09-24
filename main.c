#include <stdio.h>
#include <string.h>

void addData()
{
    char fullname[30];
    int age;
    char healthStatus[50];

    FILE *data = fopen("test.csv", "a");

    printf("Enter Data: ");
    fprintf(data, "%29s, %d, %40s", fullname, &age, healthStatus);
    

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
    printf("Thank you for using the program system.\n");
}

void displayMenu()
{
    char alphabet;
    printf("Welcome to Annual Head Check up Management System!\n Menu Display\n a.Add Data\n b.Search Data\n c.Update Data\n d.Delete data\n e.Exit Menu\n");
    printf("Please Enter an alphabet[a-e] to display a menu: ");
    scanf(" %c", &alphabet); 

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
    // displayMenu();
    
  
    FILE *data = fopen("Checkup-Data.csv","r");

    // check file opened
    if (data == NULL)
    {
        printf("File: %p\n", data);
        printf("cannot open the file");

        return 1;
    }

    char row[100];

    while (fgets(row, sizeof(row), data) ){
        printf("%s", row);
    }

    fclose(data);

    return 0;
}
