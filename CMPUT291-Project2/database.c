#include "database.h"
#include <time.h>
#include <sys/stat.h>

void _search_key(Database* self)
{
	printf("Searching..\n");
	DBC * cursorp;
	DBT key, data;

	memset(&key, 0, sizeof(DBT));
	memset(&data, 0, sizeof(DBT));

	if (self->db->cursor(self->db, NULL, &cursorp, 0) != 0) {
		printf("Curser Creation Failed\n");
	}
	else {
		char * search_key = "xuihibbhelcweqpeqajbapkvtewyoitruikompuhndafmrlqahizwbypdkkhrlesgnzjrd";
		char * search_data = "";

		//set up keys
		key.data = search_key;
		key.size = strlen(search_key) + 1;
		data.data = search_data;
		data.size = strlen(search_data) + 1;


		if (cursorp->get(cursorp, &key, &data, DB_SET) != 0) {
			printf("Begin Test...\n");
			time_t total_t;
			printf("Begin Test...\n");
			int test;
			int i;

			for (i = 0; i < 5; i++) {
				time_t start_t, end_t;
				printf("Begin Test...\n");
				start_t = time(NULL);
				test = cursorp->get(cursorp, &key, &data, DB_SET);
				end_t = time(NULL);
				total_t = end_t - start_t;
				printf("The time elapsed was: %ld\n", total_t);
				
			}
			
			// Do something with the data
			printf("%s\n", key.data);
		}
	}

	/* Close the cursor */
	if (cursorp != NULL) {
		cursorp->close(cursorp);
	}
}
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
		puts("The database does not exist; please create one first.");
	}
}
void _populate(Database* self, int amount)
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
		//printf("%s\n\n", (char *)value.data);
		
		if (failure = self->db->put(self->db, NULL, &key, &value, 0))
		{
			//printf("DB->put: %s\n", db_strerror(failure));
		}
	}
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
void _init(Database* self)
{
	self->create = _create;
	self->destroy = _destroy;
	self->populate = _populate;
	self->menu = _menu;
	self->search_key = _search_key;
}

void DBCreate(int dbtype)
{
	Database _D;
	_init(&_D);
	char file[256];
	while (1)
	{
		int task = _D.menu(&_D);
		switch (task)
		{
		default:
			// Nothing
		case 1:
			strcpy(file, _D.create(&_D, dbtype));
			puts("DB Created");
			_D.populate(&_D, 100);
			puts("DB Populated");
			break;
		case 2:
			_D.search_key(&_D);
			break;
		case 3:
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