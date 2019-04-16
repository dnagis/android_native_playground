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

/** pour transformer un fd en un stream file-like qui me permet de récupérer ligne par ligne
 * copié depuis ailleurs dans les sources (frameworks/native/service/vr/performanced/, juste enlevé les namespaces.**/
#include "stdio_filebuf.h" 



#include <binder/IServiceManager.h>

using namespace android;



int main()
{
	const String16 name("deviceidle");
	Vector<String16> args;
	int pipefd[2];
	std::string line, ma_variable;


    /**Bind au service via string "name"**/
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

	
    /**dumpsys ->dump(int fd, const Vector<String16>& args) frameworks/native/cmds/dumpsys/ frameworks/native/libs/binder/include/binder/IBinder.h**/
    service->dump(pipefd[1], args); //fd à 1 <=> stdout, pipefd[1] <=> write end
    
    /**transformation du fd en stream (FILE object), espoir d'avoir plus de fonctions de manipulation de fichier pour parser**/
    /**le stdio_filebuf.h me permet pas comme dans l'ordi de passer directement d'un fd à un istream (pour avoir getline) du coup obligé de passer par ça**/
    FILE* pFile = fdopen(pipefd[0], "r");
    if (pFile == NULL) printf("Error opening pipe");
    
    /**transformation du FILE stream en istream pour avoir getline**/    
    stdio_filebuf<char> filebuf(pFile);
    std::istream is(&filebuf);
    
    while(std::getline(is, line)) 
    {
		//printf("%lu -- %s\n", line.length(), line.c_str());
		if (line.find("mState") != std::string::npos) 
		{
		std::size_t start = 2;
		size_t ws = line.find(" ", start);
		//printf("ws @ %lu", ws);
		ma_variable = line.substr(9, ws-9);
		break;
		}
	}
	
	
	printf("ma variable extraite = %s\n", ma_variable.c_str());
    
    
	
	return 0;
}
