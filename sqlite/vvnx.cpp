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





int main()
{
	

    printf("Début de main de natvvnx, on va se coller au service...\n");
    
	sqlite3 *db;
	int rc;
		


	char stmt[70] = "";//attention si taille vide segfault au runtime
	char debut_stmt[] = "insert into temp values(NULL,11111111, 'eee', 123);";
	strcpy(stmt, debut_stmt);
	//strcat(stmt, time_as_string);

	
	printf("commande en string = %s\n", stmt);		
	
	rc = sqlite3_open("/data/data/com.example.android.bluealrm/databases/temp.db", &db);
	rc = sqlite3_exec(db, stmt, NULL, 0, NULL); 		
	
	sqlite3_close(db);
    
    
    


    _exit(0);
}
