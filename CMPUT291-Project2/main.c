#include "btree.h"

int main(int argc, char ** argv)
{
	_wmkdir(u"./tmp");
	_wmkdir(u"./tmp/ioltuszy");

	btree(); // Comment out if using from command prompt (Option 1)
	if (argc == 2)
	{
		switch (*argv[1])
		{
			default:
				printf("Invalid DB Type Option");
				break;
			case '1': // DB_BTREE
				btree();
				break;
			case '2': // DB_HASH

				break;
			case '3': // indexfile

				break;
		}
	}

	_wrmdir(u"./tmp/ioltuszy");
	_wrmdir(u"./tmp");

	return EXIT_SUCCESS;
}