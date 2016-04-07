#include <stdlib.h>
#include <sys/stat.h>
#include <db.h>
#include <string.h>
typedef struct DatabaseStruct Database;
struct DatabaseStruct {
	DB* db;
	DBC** cursor;

	char **keystomatch;
	char **valuestomatch;
};

long diff_time(struct timespec* end, struct timespec* start)
{
	uint64_t diff;
	diff = 1000000000 * (end->tv_sec - start->tv_sec) + end->tv_nsec - start->tv_nsec;
	return diff / 1000;
}

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
	db_create(&(self->db), NULL, 0);
	char filepath[256];
	memset(&filepath, 0, sizeof(filepath));
	for (int i = 0; i<strlen(filepath); i++)
	{
		filepath[0] = malloc(sizeof(char));
	}
	memset(&filepath, 0, sizeof(filepath));
	strncpy(filepath, "./tmp/ioltuszy_db/", 18);
	strncat(filepath, (dbtype == 1) ? "btree" : (dbtype == 2) ? "hash" : "indexfile", (dbtype == 1) ? 5 : (dbtype == 2) ? 4 : 9);
	strncat(filepath, ".db\0", 4);
	(self->db)->open((self->db), NULL, filepath, NULL, dbtype, DB_CREATE, 0755);
	char* solution;
	solution = strdup(filepath);
	return solution;
}
void _destroy(Database* self, char * filepath)
{
	struct stat exists;
	int result = stat(filepath, &exists);
	if (!result)
	{
		unlink(filepath);
	}
}
void _populate(Database* self, int amount)
{
	int range;
	int seed = 10000000;
	srand(seed);
	char keybuff[128];
	char valuebuff[128];

	self->keystomatch = malloc(4 * sizeof(char*));
	for (int i = 0; i < 4; i++)
		self->keystomatch[i] = malloc((128 + 1) * sizeof(char));

	self->valuestomatch = malloc(4 * sizeof(char*));
	for (int i = 0; i < 4; i++)
		self->valuestomatch[i] = malloc((128 + 1) * sizeof(char));

	int random_exists[1];
	for (int i = 0; i <= 1; i++) {
		random_exists[i] = rand() % 100000;
	}

	char * keys[4];
	for (int i = 0; i<3; i++)
	{
		keys[i] = (char *)malloc(sizeof(char) * 129);
	}

	for (int entry = 0; entry < amount; entry++) { // # to populate with
		range = 64 + rand() % (64);
		for (int kbi = 0; kbi < range; kbi++) // keybuffer index
		{
			keybuff[kbi] = (char)(97 + rand() % 26);
		}
		keybuff[range] = 0; // null terminate
		range = 64 + rand() % (64);
		for (int vbi = 0; vbi < range; vbi++) // valuebuffer index
		{
			valuebuff[vbi] = (char)(97 + rand() % 26);
		}
		valuebuff[range] = 0; // null terminate

							  //printf("Key: %s\nData: %s\n", keybuff, valuebuff);

		if (entry == random_exists[0]) {
			self->keystomatch[0] = strdup(keybuff);
			self->valuestomatch[0] = strdup(valuebuff);
		}

		if (entry == random_exists[1]) {
			self->keystomatch[1] = strdup(keybuff);
			self->valuestomatch[1] = strdup(valuebuff);
		}

		if (entry == random_exists[2]) {
			self->keystomatch[2] = strdup(keybuff);
			self->valuestomatch[2] = strdup(valuebuff);
		}

		if (entry == random_exists[3]) {
			self->keystomatch[3] = strdup(keybuff);
			self->valuestomatch[3] = strdup(valuebuff);
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

void DBCreate(int dbtype)
{
	Database * _D;
	_D = (Database *)malloc(sizeof(_D));
	DBT key, value;
	char keybuff[128];
	char valuebuff[128];
	char* filepath;
	char file[256];
	int ret;
	struct timespec start, end;
	char keytomatch[128], valuetomatch[128];
	char mintomatch[128], maxtomatch[128];
	int searchnumber = 0;
	int resultcount = 0;
	long totalTime = 0;
	struct stat exists;
	int result;
	while (1)
	{
		int task = _menu(_D);
		memset(&start, 0, sizeof(start));
		memset(&end, 0, sizeof(end));
		totalTime = 0;
		switch (task)
		{
		default:
			// Nothing
		case 1:
			filepath = _create(_D, dbtype);
			memcpy(file, filepath, sizeof(file));
			_D->cursor = malloc(sizeof(_D->cursor));
			_D->db->cursor(_D->db, NULL, _D->cursor, 0);
			_populate(_D, 100000);
			break;
		case 2:
			result = stat(file, &exists);
			if (!result)
			{
				if (searchnumber == 4) {
					//this program needs to search over different keys four times. After all keys have been used, they keys will repeat with this message.
					printf("The keys for search will now repeat.\n");
					searchnumber = 0;
				}

				if (searchnumber == 3) {
					memset(&keytomatch, 0, sizeof(keytomatch));
					strcpy(keytomatch, _D->keystomatch[3]);
					searchnumber++;
				}

				if (searchnumber == 2) {
					memset(&keytomatch, 0, sizeof(keytomatch));
					strcpy(keytomatch, _D->keystomatch[2]);
					searchnumber++;
				}

				if (searchnumber == 1) {
					memset(&keytomatch, 0, sizeof(keytomatch));
					strcpy(keytomatch, _D->keystomatch[1]);
					searchnumber++;
				}

				if (searchnumber == 0) {
					memset(&keytomatch, 0, sizeof(keytomatch));
					strcpy(keytomatch, _D->keystomatch[0]);
					searchnumber++;
				}

				memset(&key, 0, sizeof(key));
				key.data = keytomatch;
				key.size = strlen(key.data) + 1;
				value.flags = DB_DBT_USERMEM;
				value.data = valuebuff;
				value.ulen = sizeof(valuebuff);

				clock_gettime(CLOCK_MONOTONIC, &start); // Start query
				(*_D->cursor)->c_get((*_D->cursor), &key, &value, DB_SET);
				clock_gettime(CLOCK_MONOTONIC, &end); // End query

				answers(key.data, 0);
				answers(value.data, 1);

				(*_D->cursor)->c_get((*_D->cursor), &key, &value, DB_FIRST);
				printf("Elapsed Time (microseconds): %lu\n", diff_time(&end, &start));
				printf("Total Records Returned: %d\n", 1);

			}
			else
			{
				puts("Please create the database first");
			}
			break;
		case 3:
			result = stat(file, &exists);
			if (!result)
			{
				if (searchnumber == 4) {
					//this program needs to search over different values four times. After all values have been used, the values will repeat with this message.
					printf("The values for search will now repeat.");
					searchnumber = 0;
				}

				if (searchnumber == 3) {
					memset(&valuetomatch, 0, sizeof(valuetomatch));
					strcpy(valuetomatch, _D->valuestomatch[3]);
					searchnumber++;
				}

				if (searchnumber == 2) {
					memset(&valuetomatch, 0, sizeof(valuetomatch));
					strcpy(valuetomatch, _D->valuestomatch[2]);
					searchnumber++;
				}

				if (searchnumber == 1) {
					memset(&valuetomatch, 0, sizeof(valuetomatch));
					strcpy(valuetomatch, _D->valuestomatch[1]);
					searchnumber++;
				}

				if (searchnumber == 0) {
					memset(&valuetomatch, 0, sizeof(valuetomatch));
					strcpy(valuetomatch, _D->valuestomatch[0]);
					searchnumber++;
				}

				key.flags = DB_DBT_USERMEM;
				key.data = keybuff;
				key.ulen = sizeof(keybuff);
				value.flags = DB_DBT_USERMEM;
				value.data = valuebuff;
				value.ulen = sizeof(valuebuff);
				resultcount = 0;

				clock_gettime(CLOCK_MONOTONIC, &start); // Start query
				(*_D->cursor)->c_get((*_D->cursor), &key, &value, DB_FIRST);
				if (strcmp((char *)value.data, valuetomatch) == 0)
				{
					// Got an entry with correct data

					clock_gettime(CLOCK_MONOTONIC, &end);
					totalTime += diff_time(&end, &start);
					answers(key.data, 0);
					answers(value.data, 1);
					clock_gettime(CLOCK_MONOTONIC, &start);
					resultcount++;
				}
				while ((ret = (*_D->cursor)->c_get((*_D->cursor), &key, &value, DB_NEXT)) == 0)
				{
					if (strcmp((char *)value.data, valuetomatch) == 0)
					{
						// Got an entry with correct data
						clock_gettime(CLOCK_MONOTONIC, &end);
						totalTime += diff_time(&end, &start);
						answers(key.data, 0);
						answers(value.data, 1);
						clock_gettime(CLOCK_MONOTONIC, &start);
						resultcount++;
					}
				}
				clock_gettime(CLOCK_MONOTONIC, &end); // End query
				totalTime += diff_time(&end, &start);

				printf("Elapsed Time (microseconds): %lu\n", totalTime);
				printf("Total Records Returned: %d\n", resultcount);
			}
			else
			{
				puts("Please create the database first");
			}
			break;
		case 4:
			result = stat(file, &exists);
			if (!result)
			{
				printf("Please enter your minimum range: ");
				scanf("%s", mintomatch);
				printf("Please enter your maximum range: ");
				scanf("%s", maxtomatch);

				memset(&key, 0, sizeof(key));
				key.data = mintomatch;
				key.size = strlen(key.data) + 1;
				value.flags = DB_DBT_USERMEM;
				value.data = valuebuff;
				value.ulen = sizeof(valuebuff);

				resultcount = 0;

				if (dbtype == 1)
				{
					clock_gettime(CLOCK_MONOTONIC, &start); // Start query
					while ((ret = (*_D->cursor)->c_get((*_D->cursor), &key, &value, DB_SET_RANGE)) == 0)
					{
						if (strcmp((char *)key.data, maxtomatch) > 0)
						{
							// End of range
							break;
						}
						//printf("Key: %s\nData: %s\n", key.data, value.data);
						answers(key.data, 0);
						answers(value.data, 1);
						resultcount++;
						((char*)key.data)[strlen((char *)key.data) - 1]++; // Increment last char before null terminator
					}
					clock_gettime(CLOCK_MONOTONIC, &end); // End query
				}
				else
				{
					key.flags = DB_DBT_USERMEM;
					key.data = keybuff;
					key.ulen = sizeof(keybuff);
					clock_gettime(CLOCK_MONOTONIC, &start); // Start query
					(*_D->cursor)->c_get((*_D->cursor), &key, &value, DB_FIRST);
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
					while ((ret = (*_D->cursor)->c_get((*_D->cursor), &key, &value, DB_NEXT)) == 0)
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
					clock_gettime(CLOCK_MONOTONIC, &end); // End query
				}
				printf("Elapsed Time (microseconds): %lu\n", diff_time(&end, &start));
				printf("Total Records Returned: %d\n", resultcount);
			}
			else
			{
				puts("Please create the database first");
			}
			break;
		case 5:
			_destroy(_D, file);
			break;
		case 6:
			_destroy(_D, file);
			break;
		}
		if (task == 6)
		{
			break;
		}
	}
}

int main(int argc, char ** argv)
{
	mkdir("./tmp", 0755);
	mkdir("./tmp/ioltuszy_db", 0755);

	if (argc == 2)
	{
		if (*argv[1] == '1' || *argv[1] == '2' || *argv[1] == '3')
		{
			int dbtype = (int)(*argv[1] - '0');
			DBCreate(dbtype);
		}
		else
		{
			printf("Invalid DB Type Option");
		}
	}

	rmdir("./tmp/ioltuszy_db");
	rmdir("./tmp");

	return EXIT_SUCCESS;
}
