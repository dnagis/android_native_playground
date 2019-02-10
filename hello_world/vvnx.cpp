/*
***
* frameworks/base/cmds/
* 
* adb push out/target/product/mido/system/bin/hellovvnx /system/bin
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

#include <binder/IServiceManager.h>

using namespace android;



int main()
{
	const String16 name("alarm");
	//const String16 name("ZoubZoub"); //contrôle neg


    fprintf(stderr, "Début de main de natvvnx, on va se coller au service...\n");
    
    sp<IServiceManager> sm = defaultServiceManager();
    if (sm->checkService(name) == nullptr) {
		fprintf(stderr, "Le check du service a foiré\n");
		_exit(1);
	}
    

    _exit(0);
}
