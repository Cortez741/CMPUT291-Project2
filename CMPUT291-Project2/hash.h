#ifndef HASH_INCLUDED
#define HASH_INCLUDED

#include <stdlib.h>
#include <db.h>

void HashDB();
int HashDB_Menu();
void HashDB_Init(DB * db);
void HashDB_Pop(DB * db);
void HashDB_Destroy(DB * db);

#endif
