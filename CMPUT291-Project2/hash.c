#include "hash.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <db.h>


#define DA_FILE  "/tmp/ioltuszy_db/sample_db"   
#define DB_SIZE   10

/*
void HashDB_Destroy() {
	self->db->close(self->db, 0);
	_unlink("./tmp/ioltuszy/btree.db");
	//self->db->remove(self->db, "./tmp/ioltuszy/btree.db", NULL, 0);
}

void HashDB_Init() {

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



DB * HashDB_Pop(DB * db) {

	printf("Populating...\n");
	int database;
	DBT key, data;
	int range, index, i;
	unsigned seed;
	char keybuff[128];
	char databuff[128];
	
	memset(&key, 0, sizeof(key));
	memset(&data, 0, sizeof(data));
	
	seed = 10000000;
	srand(seed);
	
	
	//populate DB
	for (index = 0; index < DB_SIZE; index++) {
		printf("Populating...\n");
		
		// to generate the key string
		range = 64 + rand() % (64);
		for (i = 0; i<range;i++)
			keybuff[i] = (char)(97 + rand() % 26);
		keybuff[range] = 0;

		key.data = keybuff;
		key.size = range;

		// to generate the data string
		range = 64 + rand() % (64);
		for (i = 0;i<range;i++)
			databuff[i] = (char)(97 + rand() % 26);
		databuff[range] = 0;

		data.data = databuff;
		data.size = range;

		// You may record the key/data string for testing
		printf("%s\n\n", (char *)key.data); 
		printf("%s\n\n", (char *)data.data);

		// to insert the key/data pair into the db
		if (database = db->put(db, NULL, &key, &data, 0))
			printf("DB->put: %s\n", db_strerror(database));
		
	}
	return db;
}

DB * HashDB_Init(DB * db) {

	int database;
	database = db_create(&db, NULL, 0);
	database = db->open(db, NULL, "./tmp/ioltuszy/hash.db", NULL, DB_HASH, DB_CREATE, 0);

	return db;
}

void HashDB_Destroy(DB * db) {
	printf("Deleting...\n");
	int database;
	printf("Deleting...\n");
	database = db->remove(db, "./tmp/ioltuszy/hash.db", NULL, 0);
	database = db->close(db, 0);
}

int HashDB_Menu() {
	int task = 0;
	int task_pass = 1; //means failed to pass

	//validates user input
	while (task_pass == 1) {
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
		//fflush(stdout);



		if ((task < 1) || (task > 6)) {
			printf("Your mode is not one of the accepted modes, please try again.\n");
			task_pass = 1;
		}

		else {
			printf("This is your mode: %d\n", task);
			task_pass = 0;
		}
	}
	return task;
}


void HashDB() {

	int done_quit = 0;//means not quit
	int task;
	DB * db;

	while (done_quit == 0) {

		task = HashDB_Menu();

		printf("DB deleting\n");
		//calls appropriate functions

		if (task == 1) {
			HashDB_Init(&db);
			printf("DB created\n");

			HashDB_Pop(db);
			printf("DB populated\n");
		}

		if (task == 2) {
			printf("Hello");
		}

		if (task == 5) {
			printf("DB deleting\n");
			HashDB_Destroy(&db);
		}

		if (task == 6) {
			printf("DB deleting\n");
			HashDB_Destroy(&db);
			done_quit = 1;
		}
		
	}
}




