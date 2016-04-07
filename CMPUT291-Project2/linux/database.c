#include "database.h"
#include "timer.h"

#define db_create(a,b,c);

void answers(char * string, int newline) {
	//opens the file
	FILE * answers = fopen("answers.txt", "a+");
	//prints the input to the file
	if (newline) {
		fprintf(answers, "%s\n", string);
		fprintf(answers, "\n");
	}
	else {
		fprintf(answers, "%s\n", string);
	}
	//closes the file
	fclose(answers);
}

void AddEntry(Database* self, char * keyc, char * valuec)
{
	DBT key, value;
	memset(&key, 0, sizeof(key));
	key.data = keyc;
	key.size = strlen(key.data) + 1;

	memset(&value, 0, sizeof(value));
	value.data = valuec;
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
	printf("test3\n");
	int ret;
	//self->db = malloc(sizeof(self->db));
	printf("test3\n");
	if (ret = self->db->open(self->db, NULL, filepath, NULL, dbtype, DB_CREATE, 0755) != 0){
		self->db->err(self->db, ret, "DB->cursor");
	}
	printf("test3\n");
	return filepath;
}
void _destroy(Database* self, char * filepath)
{
	struct stat exists;
	int result = stat(filepath, &exists);
	if (!result)
	{
		self->db->close(self->db, 0);
		unlink(filepath);
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

	memset(&self->keytomatch1, 0, sizeof(self->keytomatch1));
	memset(&self->valuetomatch1, 0, sizeof(self->valuetomatch1));

	int random_exists[8];

	for (int i = 0; i <= 7; i++) {
		random_exists[i] = rand();
	}

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

		if (entry == random_exists[0]) {
			strcpy(self->keytomatch0, keybuff);
			strcpy(self->valuetomatch0, valuebuff);

			//printf("Key: %s\n", self->keytomatch0);
			//printf("Value: %s\n", self->valuetomatch0);
		}

		if (entry == random_exists[1]) {
			strcpy(self->keytomatch1, keybuff);
			strcpy(self->valuetomatch1, valuebuff);

			//printf("Key: %s\n", self->keytomatch1);
			//printf("Value: %s\n", self->valuetomatch1);
		}

		if (entry == random_exists[2]) {
			strcpy(self->keytomatch2, keybuff);
			strcpy(self->valuetomatch2, valuebuff);

			//printf("Key: %s\n", self->keytomatch2);
			//printf("Value: %s\n", self->valuetomatch2);
		}

		if (entry == random_exists[3]) {
			strcpy(self->keytomatch3, keybuff);
			strcpy(self->valuetomatch3, valuebuff);

			//printf("Key: %s\n", self->keytomatch3);
			//printf("Value: %s\n", self->valuetomatch3);
		}
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
			invalid = 0;
		}
	}
	return selection;
}
void _initDatabase(Database* self)
{
	self->create = _create;
	self->destroy = _destroy;
	self->populate = _populate;
	self->menu = _menu;


}

void DBCreate(int dbtype)
{
	Database _D;
	//_D = (Database *)malloc(sizeof(_D));
	_initDatabase(&_D);
	char file[256];
	DBT key, value;
	char keybuff[128];
	char valuebuff[128];
	int ret;
	long start, end;
	char keytomatch[128], valuetomatch[128];
	char mintomatch[128], maxtomatch[128];
	int searchnumber = 0;
	int resultcount = 0;
	long totalTime = 0;
	while (1)
	{
		int task = _D.menu(&_D);
		switch (task)
		{
		default:
			// Nothing
		case 1:
			printf("test1\n");
			memset(&file, 0, sizeof(file));
			printf("test2\n");
			strcpy(file, _D.create((dbtype)));
			printf("test2\n");
			puts("DB: Created");

			_D.cursor = malloc(sizeof(_D.cursor));
			_D.db->cursor(_D.db, NULL, _D.cursor, 0);

			_D.populate(&_D, 100000);
			break;
		case 2:
			if (searchnumber == 4) {
				//this program needs to search over different keys four times. After all keys have been used, they keys will repeat with this message.
				printf("The keys for search will now repeat.");
				searchnumber = 0;
			}

			if (searchnumber == 3) {
				memset(&keytomatch, 0, sizeof(keytomatch));
				strcpy(keytomatch, _D.keytomatch3);
				searchnumber++;
			}

			if (searchnumber == 2) {
				memset(&keytomatch, 0, sizeof(keytomatch));
				strcpy(keytomatch, _D.keytomatch2);
				searchnumber++;
			}

			if (searchnumber == 1) {
				memset(&keytomatch, 0, sizeof(keytomatch));
				strcpy(keytomatch, _D.keytomatch1);
				searchnumber++;
			}

			if (searchnumber == 0) {
				memset(&keytomatch, 0, sizeof(keytomatch));
				strcpy(keytomatch, _D.keytomatch0);
				searchnumber++;
			}

			memset(&key, 0, sizeof(key));
			key.data = keytomatch;
			key.size = strlen(key.data) + 1;
			value.flags = DB_DBT_USERMEM;
			value.data = valuebuff;
			value.ulen = sizeof(valuebuff);

			clock_readtime(&start); // Start query
			(*_D.cursor)->c_get(*_D.cursor, &key, &value, DB_SET);
			// Got an entry with correct data
			clock_readtime(&end); // End query

			answers(key.data, 0);
			answers(value.data, 1);

			(*_D.cursor)->c_get(*_D.cursor, &key, &value, DB_FIRST);
			printf("Elapsed Time (microseconds): %lu\n", diff_time(end, start));
			printf("Total Records Returned: %d\n", 1);

			/*printf("Key: %.*s\nData: %.*s\n",
			(int)key.size, (char *)key.data,
			(int)value.size, (char *)value.data);*/

			break;
		case 3:
			if (searchnumber == 4) {
				//this program needs to search over different values four times. After all values have been used, the values will repeat with this message.
				printf("The values for search will now repeat.");
				searchnumber = 0;
			}

			if (searchnumber == 3) {
				memset(&valuetomatch, 0, sizeof(valuetomatch));
				strcpy(valuetomatch, _D.valuetomatch3);
				searchnumber++;
			}

			if (searchnumber == 2) {
				memset(&valuetomatch, 0, sizeof(valuetomatch));
				strcpy(valuetomatch, _D.valuetomatch2);
				searchnumber++;
			}

			if (searchnumber == 1) {
				memset(&valuetomatch, 0, sizeof(valuetomatch));
				strcpy(valuetomatch, _D.valuetomatch1);
				searchnumber++;
			}

			if (searchnumber == 0) {
				memset(&valuetomatch, 0, sizeof(valuetomatch));
				strcpy(valuetomatch, _D.valuetomatch0);
				searchnumber++;
			}

			key.flags = DB_DBT_USERMEM;
			key.data = keybuff;
			key.ulen = sizeof(keybuff);
			value.flags = DB_DBT_USERMEM;
			value.data = valuebuff;
			value.ulen = sizeof(valuebuff);
			resultcount = 0;

			clock_readtime(&start); // Start query
			(*_D.cursor)->c_get((*_D.cursor), &key, &value, DB_FIRST);
			if (strcmp((char *)value.data, valuetomatch) == 0)
			{
				// Got an entry with correct data
				
				clock_readtime(&end);
				totalTime += diff_time(end, start);
				answers(key.data, 0);
				answers(value.data, 1);
				clock_readtime(&start);
				resultcount++;
			}
			while ((ret = (*_D.cursor)->c_get((*_D.cursor), &key, &value, DB_NEXT)) == 0)
			{
				if (strcmp((char *)value.data, valuetomatch) == 0)
				{
					// Got an entry with correct data
					clock_readtime(&end);
					totalTime += diff_time(end, start);
					answers(key.data, 0);
					answers(value.data, 1);
					clock_readtime(&start);
					resultcount++;
				}
			}
			clock_readtime(&end); // End query
			totalTime += diff_time(end, start);

			printf("Elapsed Time (microseconds): %lu\n", totalTime);
			printf("Total Records Returned: %d\n", resultcount);
			break;
		case 4:
			printf("Please enter your minimum range: ");
			scanf("%s", &mintomatch);
			printf("Please enter your maximum range: ");
			scanf("%s", &maxtomatch);

			memset(&key, 0, sizeof(key));
			key.data = mintomatch;
			key.size = strlen(key.data) + 1;
			value.flags = DB_DBT_USERMEM;
			value.data = valuebuff;
			value.ulen = sizeof(valuebuff);

			resultcount = 0;

			if (dbtype == 1)
			{
				clock_readtime(&start); // Start query
				while ((ret = (*_D.cursor)->c_get((*_D.cursor), &key, &value, DB_SET_RANGE)) == 0)
				{
					if (strcmp((char *)key.data, maxtomatch) > 0)
					{
						// End of range
						break;
					}
					answers(key.data, 0);
					answers(value.data, 1);
					resultcount++;
					((char*)key.data)[strlen((char *)key.data) - 1]++; // Increment last char before null terminator
				}
				clock_readtime(&end); // End query
			}
			else
			{
				key.flags = DB_DBT_USERMEM;
				key.data = keybuff;
				key.ulen = sizeof(keybuff);
				clock_readtime(&start); // Start query
				(*_D.cursor)->c_get((*_D.cursor), &key, &value, DB_FIRST);
				if (strcmp((char *)key.data, mintomatch) < 0)
				{
					// Bad key
				}
				else if (strcmp((char *)key.data, maxtomatch) > 0)
				{
					// Bad key
				}
				else
				{
					answers(key.data, 0);
					answers(value.data, 1);
					resultcount++;
				}
				while ((ret = (*_D.cursor)->c_get((*_D.cursor), &key, &value, DB_NEXT)) == 0)
				{
					if (strcmp((char *)key.data, mintomatch) < 0)
					{
						// Bad key
					}
					else if (strcmp((char *)key.data, maxtomatch) > 0)
					{
						// Bad key
					}
					else
					{
						answers(key.data, 0);
						answers(value.data, 1);
						resultcount++;
					}
				}
				clock_readtime(&end); // End query
			}
			printf("Elapsed Time (microseconds): %lu\n", diff_time(end, start));
			printf("Total Records Returned: %d\n", resultcount);
			break;
		case 5:
			_D.destroy(file);
			break;
		case 6:
			_D.destroy(file);
			break;
		}
		if (task == 6)
		{
			break;
		}
	}
}

