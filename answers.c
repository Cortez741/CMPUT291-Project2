//---answers.c----------------------------------------------------------------------------------------------------------------------------
// Authors: Victor Frunza & Ian Oltuszyk
// Instructor: Li-Yan Yuan
// TA: Kriti Khare

#include <stdio.h>
#include <stdlib.h>
#include "BTree\btree.h"
#include <db.h>

//when using this function, make sure to append "" to both ends of the string before passing the string into this function.
int main(int argc, char * argv[]){
    //opens the file
    FILE * answers = fopen("answers.txt", "a+");
    //prints the input to the file
    fprintf(answers, "%s\n", argv[1]);
    //closes the file
    fclose(answers);
    return 0;
}
