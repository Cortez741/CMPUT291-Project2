#ifndef HASH_INCLUDED
#define HASH_INCLUDED

#include <stdlib.h>
#include <db.h>

void HashDB();
int HashDB_Menu();
DB * HashDB_Init(DB * db);
DB * HashDB_Pop(DB * db);
void HashDB_Destroy(DB * db);

#endif
