/*
***
* 
* dumpsys d'un service -> dump() prend un fd; je mets un fichier
* 
* frameworks/base/cmds/
* 
* adb push out/target/product/mido/system/bin/dumpvvnx /system/bin
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
	FILE *stream;
	//int new_fd;	
	char buffer[255];
	


    fprintf(stderr, "Début de main, on va se coller au service...\n");
    
    sp<IServiceManager> sm = defaultServiceManager();
	
	sp<IBinder> service = sm->checkService(name);
    
    if (service == nullptr) {
		fprintf(stderr, "Le check du service a foiré\n");
		return 1;
	}
    
    //fp = fopen("/data/data/idle.txt", "a");
    //fd = fileno(fp);
    //fprintf(stderr, "le fd vers fp=%i\n", fd);
    //new_fd = dup(1);
    fprintf(stderr, "li 55\n");    
	stream = fdopen(0, "r");   
	
	if (stream == nullptr) {
		fprintf(stderr, "Le fdopen a foiré\n");
		return 1;
	} 

    // frameworks/native/cmds/dumpsys/
	// ./frameworks/native/libs/binder/include/binder/IBinder.h
	fprintf(stderr, "li 60\n");
    int err = service->dump((int)&stream, args); //dump(int fd, const Vector<String16>& args) --> fd=1 = stdout
    
    
    


	fprintf(stderr, "li 64\n");
	
	while(fgets(buffer, 255, stream)) {
    printf("%s *************\n", buffer);
	}
    
	fprintf(stderr, "li 69\n");
    fclose(stream);


	fprintf(stderr, "li 73\n");	
	fprintf(stdout, "fin du programme err=%i\n", err);

	return 0;
}
