#ifndef BTREE_INCLUDED
#define BTREE_INCLUDED

#include <stdlib.h>
#include <db.h>

typedef struct BTreeDatabase BTD;
struct BTreeDatabase {
	DB* db; 
	void(*create)(BTD*);
	void(*destroy)(BTD*);
	void(*populate)(BTD*);
	int(*menu)(BTD*);
};
void BTD_init(BTD* self);
void BTD_create(BTD* self);
void BTD_destroy(BTD* self);
void BTD_populate(BTD* self);
int BTD_menu(BTD* self);
void BTreeDB();

#endif