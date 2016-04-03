#include "database.h"

int main(int argc, char ** argv)
{
	_wmkdir(u"./tmp");
	_wmkdir(u"./tmp/ioltuszy_db");

	if (argc == 2)
	{
		if (*argv[1] == '1' || *argv[1] == '2' || *argv[1] == '3')
		{
			int dbtype = *argv[1] - '0';
			DBCreate(dbtype);
		}
		else
		{
			printf("Invalid DB Type Option");
		}
	}

	_wrmdir(u"./tmp/ioltuszy_db");
	_wrmdir(u"./tmp");

	return EXIT_SUCCESS;
}