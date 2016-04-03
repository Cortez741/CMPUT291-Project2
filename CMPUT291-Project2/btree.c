#include "btree.h"

void BTD_destroy(BTD* self)
{
	self->db->close(self->db, 0);
	_unlink("./tmp/ioltuszy/btree.db");
	//self->db->remove(self->db, "./tmp/ioltuszy/btree.db", NULL, 0);
}

void BTD_init(BTD* self)
{
	db_create(&self->db, NULL, 0);
	self->db->open(self->db, NULL, "./tmp/ioltuszy/btree.db", NULL, DB_BTREE, DB_CREATE, 0);
	self->destroy = BTD_destroy;
}

void BTreeDB()
{
	BTD _BTD;
	BTD_init(&_BTD);

	puts("Press any key to continue...");
	getch();

	_BTD.destroy(&_BTD);
}
