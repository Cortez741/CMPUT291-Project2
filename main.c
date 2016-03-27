#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char * argv[]){

    /*prints an error message if the function doesn't get one argument*/
    if (argc != 2){
        printf("Argument Error: This program only accepts one argument, where the argument is either:\n");
        printf("\t(1) <btree> for a btree database\n");
        printf("\t(2) <hash> for a hash database\n");
        printf("\t(3) <indexfile> for a database with an indexfile\n");
        printf("indicating the database type to be tested.\n");
    }

    else{
        return 0;
    }
}
