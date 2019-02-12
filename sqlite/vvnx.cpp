/*
***
* frameworks/base/cmds/
* 
* adb push out/target/product/mido/system/bin/sqlvvnx /system/bin
* 
* CREATE TABLE temp (ID INTEGER PRIMARY KEY AUTOINCREMENT, ALRMTIME INTEGER NOT NULL, MAC TEXT NOT NULL, TEMP REAL NOT NULL);
* insert into temp values(
* 
* insert into temp values(NULL,11111111, 'eee', 234);
* 
* sqlite3 /data/data/com.example.android.bluealrm/databases/temp.db "select * from temp;"
* 
 */

#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>


//sans libsqlite à LOCAL_SHARED_LIBRARIES du Android.mk les includes sqlite3 plantent!
#include <sqlite3.h>

static int callback(void *data, int argc, char **argv, char **azColName){
   int i;
   fprintf(stderr, "%s: ", (const char*)data);
   
   for(i = 0; i<argc; i++){
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   
   printf("\n");
   return 0;
}




int main()
{
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	const char* data = "Callback function called";
	
	/**lire une base de donnees **/




   /* Open database */
   rc = sqlite3_open("/data/data/com.example.android.bluealrm/databases/temp.db", &db);
   
   if( rc ) {
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      return(0);
   } else {
      fprintf(stderr, "Opened database successfully\n");
   }

   /* Create SQL statement -- gaffe à la taille du char[] */
   char sql[50] = "select * from temp where ID=234;";

   /* Execute SQL statement */
   rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
   
   if( rc != SQLITE_OK ) {
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   } else {
      fprintf(stdout, "Operation done successfully\n");
   }
   sqlite3_close(db);


    

		

	/**écriture dans bdd
	 * 	sqlite3 *db;
	int rc;
	char stmt[70] = "";
	char debut_stmt[] = "insert into temp values(NULL,11111111, 'eee', 123);";
	strcpy(stmt, debut_stmt);	
	printf("commande en string = %s\n", stmt);		
	rc = sqlite3_open("/data/data/com.example.android.bluealrm/databases/temp.db", &db);
	rc = sqlite3_exec(db, stmt, NULL, 0, NULL); 			
	sqlite3_close(db);**/
    

    _exit(0);
}
