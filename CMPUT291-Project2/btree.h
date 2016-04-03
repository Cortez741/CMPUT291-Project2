#ifndef BTREE_INCLUDED
#define BTREE_INCLUDED

#include <stdlib.h>
#include <db.h>

typedef struct BTreeDatabase BTD;
struct BTreeDatabase {
	DB* db; 
	void(*destroy)(BTD*);
};
void BTD_init(BTD* self);
void BTD_destroy(BTD* self);
void BTreeDB();

#endif