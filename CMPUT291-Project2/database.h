#ifndef DATABASE_INCLUDED
#define DATABASE_INCLUDED

#include <stdlib.h>
#include <db.h>

typedef struct searchResult
{
	char key[128];
	char value[128];
	int set;
} searchResult;

typedef struct DatabaseStruct Database;
struct DatabaseStruct {
	DB* db; 
	char*(*create)(Database*);
	void(*destroy)(Database*);
	searchResult(*populate)(Database*, int);
	int(*menu)(Database*);
};
void _init(Database* self);
char * _create(Database* self);
void _destroy(Database* self);
searchResult _populate(Database* self, int amount);
int _menu(Database* self);
void DBCreate(int dbtype);

#endif