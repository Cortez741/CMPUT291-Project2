#ifndef HASH_INCLUDED
#define HASH_INCLUDED

#include <stdlib.h>
#include <db.h>

typedef struct HashDatabase HD;
struct HashDatabase {
	DB* db;
	void(*create)(HD*);
	void(*destroy)(HD*);
	void(*populate)(HD*);
	int(*menu)(HD*);
};
void HashD_init(HD* self);
void HashD_create(HD* self);
void HashD_destroy(HD* self);
void HashD_populate(HD* self);
void HashD_
int HashD_menu(HD* self);
void HashDB();

#endif