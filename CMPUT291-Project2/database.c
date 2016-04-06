#include "database.h"
#include <sys/stat.h>
#include "timer.h"

char * _create(Database* self, int dbtype) // btree = 1, hash = 2
{
	struct stat exists;
	db_create(&self->db, NULL, 0);
	char filepath[256];
	strcpy(filepath, "./tmp/ioltuszy_db/");
	strcat(filepath, (dbtype == 1) ? "btree" : (dbtype == 2) ? "hash" : "indexfile");
	strcat(filepath, ".db\0");
	self->db->open(self->db, NULL, filepath, NULL, dbtype, DB_CREATE, 0);
	return filepath;
}
void _destroy(Database* self, char * filepath)
{
	struct stat exists;
	int result = stat(filepath, &exists);
	if (!result)
	{
		self->db->close(self->db, 0);
		//self->db->remove(self->db, "./tmp/ioltuszy_db/btree.db", NULL, 0);
		_unlink(filepath);
	}
	else
	{
		puts("The database does not exist; please create a database first.");
	}
}
searchResult _populate(Database* self, int amount)
{
	searchResult sr;
	DBT key, value;

	char * search_keys[4];

	for (int i = 0; i < 4; i++) {

		if ((search_keys[i] = malloc(sizeof(char) * 128)) == NULL) {
			printf("problem allocating memory\n");
		}
	}

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

		if (entry == 24) {
			strcpy(search_keys[0], (char *)key.data);
			printf("The key to search is: %s\n", search_keys[0]);
		}

		if (entry == 49) {
			strcpy(search_keys[1], (char *)key.data);
			printf("The key to search is: %s\n", search_keys[1]);
		}

		if (entry == 74) {
			strcpy(search_keys[2], (char *)key.data);
			printf("The key to search is: %s\n", search_keys[2]);
		}

		if (entry == 99) {
			strcpy(search_keys[3], (char *)key.data);
			printf("The key to search is: %s\n", search_keys[3]);
		}

		if (entry == 5)
		{
			strcpy(sr.key, (char *)key.data);
			strcpy(sr.value, (char *)value.data);
		}
		
		if (failure = self->db->put(self->db, NULL, &key, &value, 0))
		{
			//printf("DB->put: %s\n", db_strerror(failure));
		}
	}
	sr.set = 1;
	return sr;
}
int _menu(Database* self)
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
			char c;
			scanf("%c", &c);
		}
		else
		{
			printf("This is your mode: %d\n", selection);
			invalid = 0;
		}
	}
	return selection;
}
void _retrieve(Database* self)
{

}
void _init(Database* self)
{
	self->create = _create;
	self->destroy = _destroy;
	self->populate = _populate;
	self->menu = _menu;
}

void DBCreate(int dbtype)
{
	Database _D;
	_init(&_D);
	char file[256];
	searchResult sr;
	memset(&sr, 0, sizeof(sr));
	DBT key, value;
	DBC * cursor;
	memset(&cursor, 0, sizeof(cursor));
	int ret;
	while (1)
	{
		int task = _D.menu(&_D);
		switch (task)
		{
		default:
			// Nothing
		case 1:
			strcpy(file, _D.create(&_D, dbtype));
			puts("DB: Created");
			sr = _D.populate(&_D, 100);
			_D.db->cursor(_D.db, NULL, &cursor, 0);
			break;
		case 2:
			if (sr.set)
			{
				memset(&key, 0, sizeof(key));
				memset(&value, 0, sizeof(value));
				key.data = sr.key;
				key.size = sizeof(sr.key);
				cursor->get(cursor, &key, &value, DB_NEXT);
				printf("DB: %s\n", value.data);
			}
			else
			{
				puts("The database has not yet been created");
			}
			break;
		case 3:
			if (sr.set)
			{
				memset(&key, 0, sizeof(key));
				memset(&value, 0, sizeof(value));
				value.data = sr.value;
				value.size = sizeof(sr.value);
				while ((ret = cursor->get(cursor, &key, &value, DB_NEXT)) == 0)
					printf("%.*s : %.*s\n",
						(int)key.size, (char *)key.data,
						(int)value.size, (char *)value.data);
			}
			else
			{
				puts("The database has not yet been created");
			}
			break;
		case 4:
			break;
		case 5:
			_D.destroy(&_D, file);
			break;
		case 6:
			_D.destroy(&_D, file);
			break;
		}
		if (task == 6)
		{
			break;
		}
	}
}