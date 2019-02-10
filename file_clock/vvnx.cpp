/*
***
* frameworks/base/cmds/
* 
* adb push out/target/product/mido/system/bin/filevvnx /system/bin
* 
* écrire datetime dans un fichier en utilisant libbase
* 
* 
 */

#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <android-base/file.h> //libbase (LOCAL_SHARED_LIBRARIES)

using namespace android;

static constexpr const char* file_path = "/data/data/essai.txt";

int main()
{

    fprintf(stderr, "Début de main de natvvnx\n");
    	
	std::string in;
	std::string out;
	std::string time_string;
	timespec ts;
	
    if (base::ReadFileToString(file_path, &in)) {
        fprintf(stderr, "Success Read\n");;
    } else {
        fprintf(stderr, "Fail Read\n");;
	}	
	
		
	clock_gettime(CLOCK_REALTIME, &ts);	
	time_string = ctime(&ts.tv_sec); //https://linux.die.net/man/3/ctime
	out = in + time_string;
	
    if (base::WriteStringToFile(out, file_path)) {
        fprintf(stderr, "Success Write\n");;
    } else {
        fprintf(stderr, "Fail Write\n");;
	}
    

    _exit(0);
}
