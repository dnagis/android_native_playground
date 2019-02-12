/*
***
* frameworks/base/cmds/
* 
* adb push out/target/product/mido/system/bin/sqlvvnx /system/bin
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

//sans libsqlite à LOCAL_SHARED_LIBRARIES du Android.mk les includes sqlite3 plantent!
#include <sqlite3.h>





int main()
{
	

    printf("Début de main de natvvnx, on va se coller au service...\n");
    


    _exit(0);
}
