#include "database.h"
#include "timer.h"

void AddEntry(Database* self, char * keyc, char * valuec)
{
	DBT key, value;
	memset(&key, 0, sizeof(key));
	key.data = keyc; // a
	key.size = strlen(key.data) + 1;

	memset(&value, 0, sizeof(value));
	value.data = valuec; // b
	value.size = strlen(value.data) + 1;

	int ret;
	if ((ret = (*self->cursor)->c_put(*self->cursor, &key, &value, DB_KEYFIRST)) != 0)
		self->db->err(self->db, ret, "DB->cursor");
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
		_unlink(filepath);
	}
	else
	{
		puts("The database does not exist; please create a database first.");
	}
}
void _populate(Database* self, int amount)
{
	int range;
	int seed = 10000000;
	srand(seed);
	char keybuff[128];
	char valuebuff[128];
	int ret;
	for (int entry = 0; entry < amount; entry++) { // # to populate with
		#pragma region Key Generation
		range = 64 + rand() % (64);
		for (int kbi = 0; kbi < range; kbi++) // keybuffer index
		{
			keybuff[kbi] = (char)(97 + rand() % 26);
		}
		keybuff[range] = 0; // null terminate
		#pragma endregion Key Generation
		#pragma region Value Generation
		range = 64 + rand() % (64);
		for (int vbi = 0; vbi < range; vbi++) // valuebuffer index
		{
			valuebuff[vbi] = (char)(97 + rand() % 26);
		}
		valuebuff[range] = 0; // null terminate
		#pragma endregion Value Generation

		//printf("Key: %s\nData: %s\n", keybuff, valuebuff);
		
		AddEntry(self, keybuff, valuebuff);
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
}

void DBCreate(int dbtype)
{
	Database _D;
	_init(&_D);
	char file[256];
	DBT key, value;
	char keybuff[128];
	char valuebuff[128];
	int ret;
	long start[2], end[2];
	char keytomatch[128], valuetomatch[128];
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

			_D.cursor = malloc(sizeof(_D.cursor));
			_D.db->cursor(_D.db, NULL, _D.cursor, 0);

			_D.populate(&_D, 3);
			break;
		case 2:
			strcpy(keytomatch, "vbcsyoaravmzrqjvdmqjxumrrndrsvorwcbxgtuyltxlsskxierbwjiumczzlrinyhqrhaasnzqmyrkllkzmkzplhvhszangbvfona");
			memset(&key, 0, sizeof(key));
			key.data = keytomatch;
			key.size = strlen(key.data) + 1;
			value.flags = DB_DBT_USERMEM;
			value.data = valuebuff;
			value.ulen = sizeof(valuebuff);

			clock_readtime(&start);
			(*_D.cursor)->c_get(*_D.cursor, &key, &value, DB_SET);
			// Got an entry with correct data
			clock_readtime(&end);

			printf("Elapsed (microseconds): %lu\n", (end[1] - start[1])/1000);
			/*printf("Key: %.*s\nData: %.*s\n",
				(int)key.size, (char *)key.data,
				(int)value.size, (char *)value.data);*/

			break;
		case 3:
			strcpy(valuetomatch, "epcykpslluvcytbfctdnjwopnozekrhhmrmkrjavsmfdjuxqayfopcmekzbbtkmklhrwehfsubezzqxnvkgfosplmesujsqcgwyjpjmdvclsibahnafjx");
			key.flags = DB_DBT_USERMEM;
			key.data = keybuff;
			key.ulen = sizeof(keybuff);
			value.flags = DB_DBT_USERMEM;
			value.data = valuebuff;
			value.ulen = sizeof(valuebuff);

			clock_readtime(&start); // Start query
			(*_D.cursor)->c_get((*_D.cursor), &key, &value, DB_FIRST);
			if (strcmp((char *)value.data, valuetomatch) == 0)
			{
				// Got an entry with correct data
			}
			else
			{
				while ((ret = (*_D.cursor)->c_get((*_D.cursor), &key, &value, DB_NEXT)) == 0)
					if (strcmp((char *)value.data, valuetomatch) == 0)
					{
						// Got an entry with correct data
						break;
					}
			}
			clock_readtime(&end); // End query

			printf("Elapsed (microseconds): %lu\n", (end[1] - start[1]) / 1000);
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