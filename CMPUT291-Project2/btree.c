#include "btree.h"

void BTD_create(BTD* self)
{
	db_create(&self->db, NULL, 0);
	self->db->open(self->db, NULL, "./tmp/ioltuszy/btree.db", NULL, DB_BTREE, DB_CREATE, 0);
}

void BTD_destroy(BTD* self)
{
	self->db->close(self->db, 0);
	_unlink("./tmp/ioltuszy/btree.db");
	//self->db->remove(self->db, "./tmp/ioltuszy/btree.db", NULL, 0);
}

void BTD_populate(BTD* self)
{

}

int BTD_menu(BTD* self)
{
	int invalid = 1;
	int selection;
	while (invalid)
	{
		printf("Please enter the number of the task you wish to preform.\n");
		printf("1 - Create and Populate a database\n");
		printf("2 - Retrieve records with a given key\n");
		printf("3 - Retrieve records with given data\n");
		printf("4 - Retrieve records with a given range of key values\n");
		printf("5 - Destroy the database\n");
		printf("6 - Quit\n");
		printf("Please enter your task: ");

		fflush(stdout);
		scanf("%d", &selection);

		if ((selection < 1) || (selection > 6)) {
			printf("Your mode is not one of the accepted modes, please try again.\n");
		}
		else
		{
			printf("This is your mode: %d\n", selection);
			invalid = 0;
		}
	}
	return selection;
}

void BTD_init(BTD* self)
{
	self->create = BTD_create;
	self->destroy = BTD_destroy;
	self->populate = BTD_populate;
	self->menu = BTD_menu;
}

void BTreeDB()
{
	BTD _BTD;
	BTD_init(&_BTD);

	int task = _BTD.menu(&_BTD);
	switch (task)
	{
		case 1:
			_BTD.create(&_BTD);
			puts("DB Created");
			_BTD.populate(&_BTD);
			puts("DB Populated");
			break;
		case 2:
			break;
		case 3:
			break;
		case 4:
			break;
		case 5:
			break;
		case 6:
			break;
	}
	puts("Press any key to continue...");
	getch();
	_BTD.destroy(&_BTD);
}
