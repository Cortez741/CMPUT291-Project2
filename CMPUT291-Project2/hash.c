#include "hash.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <db.h>

/*
void HashDB_Destroy() {
	self->db->close(self->db, 0);
	_unlink("./tmp/ioltuszy/btree.db");
	//self->db->remove(self->db, "./tmp/ioltuszy/btree.db", NULL, 0);
}

void HashDB_Init() {

}

void HashDB_Pop() {

}

void HashDB_Search_Key() {

}

void HashDB_Search_Data() {

}

void HashDB_Search_Range() {

}

void HashDB_Quit() {

}
*/

void HashDB() {
	int task = 0;
	//validates user input
	while ((task < 1) || (task > 6)) {
		printf("Please enter the number of the task you wish to preform.\n");
		printf("1 - Create and Populate a database\n");
		printf("2 - Retrieve records with a given key\n");
		printf("3 - Retrieve records with given data\n");
		printf("4 - Retrieve records with a given range of key values\n");
		printf("5 - Destroy the database\n");
		printf("6 - Quit\n");
		printf("Please enter your task: ");
		//flushes stdin and takes user input
		fflush(stdout);
		scanf("%d", &task);

		if ((task < 1) || (task > 6)) {
			printf("Your mode is not one of the accepted modes, please try again.\n");
		}

		else {
			printf("This is your mode: %d\n", task);
		}

		fflush(stdout);
	}

	//calls appropriate functions


}




