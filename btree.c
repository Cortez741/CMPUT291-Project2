/*
 *  A sample program to create a Berkeley db, and then
 *  populate the db with 1000 records.
 *
 *  Author: Prof. Li-Yan Yuan, University of Alberta
 *
 *  A directory named "/tmp/my_db" must be created before testing this program.
 *  You may replace my_db with user_db, where user is your user name,
 *  as required.
 *
 *  Use the following to compile this sample program
 *     gcc -o mydbtest sample.c -ldb -pthread -lm
 *
 */
# include <stdlib.h>
# include <string.h>
# include "db.h"
# include "CMPUT291-Project2\

#define DA_FILE  "/tmp/vfrunza/sample_db"
#define DB_SIZE   1000

int main(int argc, char *argv[]) {
  DB *db;
  int ret,range,index,i;
  DBT key, data;
  unsigned seed;

  char keybuff[128];
  char databuff[128];

  /*
   *  to create a db handle
   */
  if ( (ret = db_create(&db, NULL, 0)) != 0 ) {
    printf("db_create: %s\n", db_strerror(ret));
    exit(1);
  }

  /*
   *  to open the db
   */
  ret = db->open(db,NULL,DA_FILE,NULL,DB_BTREE,DB_CREATE,0);
  if (ret != 0) {
    printf("DB doesn't exist, creating a new one: %s\n", db_strerror(ret));
    exit(1);
  }

  memset(&key, 0, sizeof(key));
  memset(&data, 0, sizeof(data));

  /*
   *  to seed the random number after db openning, and see it once.
   */
  seed = 10000000;
  srand(seed);

  /*
   *  to populate the database
   */
  for (index=0;index<DB_SIZE;index++)  {

    // to generate the key string
    range=64+random()%(64);
    for (i=0; i<range;i++)
      keybuff[i]= (char)(97+random()%26);
    keybuff[range]=0;

    key.data = keybuff;
    key.size = range;

    // to generate the data string
    range=64+random()%(64);
    for (i=0;i<range;i++)
      databuff[i]= (char) (97+random()%26);
    databuff[range]=0;

    data.data=databuff;
    data.size=range;

    // You may record the key/data string for testing
    printf("%s\n",(char *)key.data); printf("%s\n\n",(char *)data.data);

    // to insert the key/data pair into the db
    if (ret=db->put(db, NULL, &key, &data, 0))
      printf("DB->put: %s\n", db_strerror(ret));
  }

  /*
   *  to close the database
   */
  if (ret = db->close(db,0))
    printf("DB->close: %s\n", db_strerror(ret));

  return 0;
}
