/**
***
* frameworks/base/cmds/
* 
* adb push out/target/product/mido/system/bin/sync_db /system/bin
* 
* envoi de résultats d'une requete sqlite en POST.
* basé sur les fonctionnalités/folders: 
* sqlite
* httpcurl
* epoll_timerfd
* 
* 
* 
 */

#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#include <curl/curl.h>
#include <sqlite3.h>

void send_via_curl(int i, const unsigned char *mac) {
	//printf("resultat: %i, %s \n", i, mac);
	CURL *curl;
	CURLcode res;	

	curl_global_init(CURL_GLOBAL_ALL);	

	curl = curl_easy_init();
	if(curl) {
	curl_easy_setopt(curl, CURLOPT_URL, "192.168.1.7:8000");
	char str[80];
	//char *number = new char;
	char number[40];
	strcpy(str, "id=");	
	sprintf(number, "%i", i);
	strcat(str, number);
	strcat(str, "&mac=");
	strcat(str, (char *)(mac));
	printf("le POST a cette gueule: %s\n", str); //"id=205&mac=30:AE:A4:04:C3:5A"
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, str);

	res = curl_easy_perform(curl);

	if(res != CURLE_OK)
	fprintf(stderr, "curl_easy_perform() failed: %s\n",
	curl_easy_strerror(res));
	curl_easy_cleanup(curl);
	}
	curl_global_cleanup();
				
}

void fetch_db() {
	
	sqlite3 *db;
   	sqlite3_stmt *stmt;
	int rc;

	rc = sqlite3_open("/data/data/com.example.android.bluealrm/databases/temp.db", &db);  
	char const *sql = "select * from temp WHERE ALRMTIME=1549708291;";	
	
	rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);	
	if (rc != SQLITE_OK) {
		printf("error: %s", sqlite3_errmsg(db));
	}
	
	while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
		int id           = sqlite3_column_int (stmt, 0);
		const unsigned char *mac = sqlite3_column_text(stmt, 2);
		send_via_curl(id, mac);
	}
	
	if (rc != SQLITE_DONE) {
	printf("error: %s", sqlite3_errmsg(db));
	}
	
	sqlite3_finalize(stmt);
	sqlite3_close(db);
	
	
	
}





int main()
{
	fetch_db();
	
    _exit(0);
}
