#ifndef DATABASE_INCLUDED
#define DATABASE_INCLUDED

#include <stdlib.h>
#include <sys/stat.h>
#include <db.h>

typedef struct DatabaseStruct Database;
struct DatabaseStruct {
	DB* db; 
	DBC** cursor;
	char*(*create)(Database*);
	void(*destroy)(Database*);
	void(*populate)(Database*, int);
	int(*menu)(Database*);

	char keytomatch0[128];
	char keytomatch1[128];
	char keytomatch2[128];
	char keytomatch3[128];

	char valuetomatch0[128];
	char valuetomatch1[128];
	char valuetomatch2[128];
	char valuetomatch3[128];
};

void AddEntry(Database* self, char * keyc, char * valuec);
void _init(Database* self);
char * _create(Database* self);
void _destroy(Database* self);
void _populate(Database* self, int amount);
int _menu(Database* self);
void DBCreate(int dbtype);

#endif