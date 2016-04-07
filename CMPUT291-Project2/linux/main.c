#include "database.h"

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
