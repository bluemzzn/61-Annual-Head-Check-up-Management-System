#include <stdio.h>

int main(){

    FILE *file = fopen("Checkup-Data.csv", "r");

    //check file opened
    if(file == NULL){
        printf("File: %p\n", file);
        printf("cannot open the file");
        
        return 1;
    }

    int fclose(FILE *stream);

    return 0;
}