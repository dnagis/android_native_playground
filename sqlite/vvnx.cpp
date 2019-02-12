/*
***
* frameworks/base/cmds/
* 
* adb push out/target/product/mido/system/bin/sqlvvnx /system/bin
* 
* 
* https://stackoverflow.com/questions/31146713/sqlite3-exec-callback-function-clarification
* 
* CREATE TABLE temp (ID INTEGER PRIMARY KEY AUTOINCREMENT, ALRMTIME INTEGER NOT NULL, MAC TEXT NOT NULL, TEMP REAL NOT NULL);
* 
* 
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



int main()
{
	sqlite3 *db;
   	sqlite3_stmt *stmt;
	int rc;

	rc = sqlite3_open("/data/data/com.example.android.bluealrm/databases/temp.db", &db);  
	char const *sql = "select * from temp WHERE ALRMTIME=1549708291;";
	
	
	rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);	
	if (rc != SQLITE_OK) {
		printf("error: %s", sqlite3_errmsg(db));
		return 1;
	}
	
	while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
		int id           = sqlite3_column_int (stmt, 0);
		const unsigned char *mac = sqlite3_column_text(stmt, 2);
		printf("resultat: %i, %s \n", id, mac);
	}
	
	if (rc != SQLITE_DONE) {
	printf("error: %s", sqlite3_errmsg(db));
	}
	
	sqlite3_finalize(stmt);

	
	/**lire une base de donnees technique avec callback d'un tuto, super lourd
	char *zErrMsg = 0; 
   char sql[50] = "select * from temp where ID=234;";
   rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);**/		

	/**écriture dans bdd
	char stmt[70] = "";
	char debut_stmt[] = "insert into temp values(NULL,11111111, 'eee', 123);";
	strcpy(stmt, debut_stmt);	
	printf("commande en string = %s\n", stmt);		
	rc = sqlite3_open("/data/data/com.example.android.bluealrm/databases/temp.db", &db);
	rc = sqlite3_exec(db, stmt, NULL, 0, NULL); 			
	**/
    sqlite3_close(db);


    _exit(0);
}
