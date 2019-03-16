#include "header.h"
#include <cutils/klog.h>
#include <android-base/file.h> //libbase (LOCAL_SHARED_LIBRARIES)

#define LOG_TAG "alrmvvnx"
#define KLOG_LEVEL 6

static constexpr const char* file_path = "/data/data/essai.txt";

using namespace android;

void action() {
	
	KLOG_WARNING(LOG_TAG, "********Timer Triggered******\n");
		
	std::string in;
	std::string out;
	std::string time_string;
	timespec ts;
	
    base::ReadFileToString(file_path, &in);			
	clock_gettime(CLOCK_REALTIME, &ts);	
	time_string = ctime(&ts.tv_sec); //https://linux.die.net/man/3/ctime
	out = in + time_string;
	
    base::WriteStringToFile(out, file_path);
	
}
