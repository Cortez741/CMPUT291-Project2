#include "btree.h"

void BTD_create(BTD* self)
{
	db_create(&self->db, NULL, 0);
	self->db->open(self->db, NULL, "./tmp/ioltuszy/btree.db", NULL, DB_BTREE, DB_CREATE, 0);
}
void BTD_destroy(BTD* self)
{
	self->db->remove(self->db, "./tmp/ioltuszy/btree.db", NULL, 0);
	self->db->close(self->db, 0);
	_unlink("./tmp/ioltuszy/btree.db");
}
void BTD_populate(BTD* self, int amount)
{
	DBT key, value;
	memset(&key, 0, sizeof(key));
	memset(&value, 0, sizeof(value));
	int range;
	int seed = 10000000;
	srand(seed);
	char keybuff[128];
	char valuebuff[128];
	int failure;
	for (int entry = 0; entry < amount; entry++) { // # to populate with
		#pragma region Key Generation
		range = 64 + rand() % (64);
		for (int kbi = 0; kbi < range; kbi++) // keybuffer index
		{
			keybuff[kbi] = (char)(97 + rand() % 26);
		}
		keybuff[range] = 0; // null terminate
		key.data = keybuff;
		key.size = range;
		#pragma endregion Key Generation
		#pragma region Value Generation
		range = 64 + rand() % (64);
		for (int vbi = 0; vbi < range; vbi++) // valuebuffer index
		{
			valuebuff[vbi] = (char)(97 + rand() % 26);
		}
		valuebuff[range] = 0; // null terminate
		value.data = valuebuff;
		value.size = range;
		#pragma endregion Value Generation
		printf("%s\n\n", (char *)key.data);
		printf("%s\n\n", (char *)value.data);
		
		if (failure = self->db->put(self->db, NULL, &key, &value, 0))
		{
			printf("DB->put: %s\n", db_strerror(failure));
		}
	}
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
			_BTD.populate(&_BTD, 10);
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

