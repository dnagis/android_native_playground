/*
***
* 
* dumpsys d'un service -> dump() prend un fd; je mets un fichier
* 
* frameworks/base/cmds/
* 
* adb push out/target/product/mido/system/bin/idlevvnx /system/bin
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
	const String16 name("deviceidle");
	//const String16 name("ZoubZoub"); //contrôle neg
	Vector<String16> args;
	FILE * fp;
	int fd;	
	timespec ts;
	
	clock_gettime(CLOCK_REALTIME, &ts);	

    fprintf(stderr, "Début de main, on va se coller au service...\n");
    
    sp<IServiceManager> sm = defaultServiceManager();
	
	sp<IBinder> service = sm->checkService(name);
    
    if (service == nullptr) {
		fprintf(stderr, "Le check du service a foiré\n");
		return 1;
	}
	
	    
    
    fp = fopen("/data/data/idle.txt", "a");
    fd = fileno(fp);
    //fprintf(stderr, "le fd vers fp=%i\n", fd);
    fprintf(fp, "**************************************************************************************************************\n");
    fprintf(fp, "********@ %ld\n", (long)ts.tv_sec);
    
    // frameworks/native/cmds/dumpsys/
	// ./frameworks/native/libs/binder/include/binder/IBinder.h
    int err = service->dump(fd, args); //dump(int fd, const Vector<String16>& args) --> fd=1 = stdout



	
	fprintf(stdout, "fin du programme err=%i\n", err);

	return 0;
}
