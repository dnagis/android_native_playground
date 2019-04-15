/*
***
* 
* dumpsys d'un service -> dump() prend un fd; on y mettre 1=stdout, ou un fichier avec fopen()
* mais pour récupérer des variables de l'output de dumpsys c'est une autre histoire: faut pouvoir
* accéder à ce qui sort de ce fd. Solution que j'ai trouvé: pipe().
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
	const String16 name("power");
	Vector<String16> args;
	char buffer[1024];
	int pipefd[2], size;


    fprintf(stderr, "Début de main, on va se coller au service...\n");
    
    sp<IServiceManager> sm = defaultServiceManager();
	
	sp<IBinder> service = sm->checkService(name);
    
    if (service == nullptr) {
		fprintf(stderr, "Le check du service a foiré\n");
		return 1;
	}
    
	if (pipe(pipefd) == -1) {
        printf("error pipe \n");
        return 1;
    }

    // frameworks/native/cmds/dumpsys/
	// ./frameworks/native/libs/binder/include/binder/IBinder.h
    int err = service->dump(pipefd[1], args); //dump(int fd, const Vector<String16>& args) --> fd=1 = stdout
    
    
    size = read(pipefd[0], buffer, 1024);    
    printf("%i\n", size);
	printf("*****%s*****\n", buffer); 

    size = read(pipefd[0], buffer, 1024);    
    printf("%i\n", size);
	printf("*****%s*****\n", buffer); 
	
	size = read(pipefd[0], buffer, 1024);    
    printf("%i\n", size);
	printf("*****%s*****\n", buffer); 		 
		 
	size = read(pipefd[0], buffer, 1024);    
    printf("%i\n", size);
	printf("*****%s*****\n", buffer); 
	
	size = read(pipefd[0], buffer, 1024);    
    printf("%i\n", size);
	printf("*****%s*****\n", buffer); 
	
	size = read(pipefd[0], buffer, 1024);    
    printf("%i\n", size);
	printf("*****%s*****\n", buffer); 
	
	

	fprintf(stdout, "fin du programme err=%i\n", err);

	return 0;
}
