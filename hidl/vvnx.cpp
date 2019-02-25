/*
***
* But:
* accéder au GPS directement en natif
*  
* 
* inspiration:
* hardware/interfaces/gnss/1.0/vts/functional/VtsHalGnssV1_0TargetTest.cpp
* https://source.android.com/devices/architecture/hidl-cpp
* 
* adb push out/target/product/mido/system/bin/hidlvvnx /system/bin
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

#include <android/hardware/gnss/1.0/IGnss.h>


using android::hardware::gnss::V1_0::IGnss;
using android::sp;





int main()
{


    fprintf(stderr, "Début de main de hidlvvnx...\n");
    sp<IGnss> gnss_hal = IGnss::getService();
	if (gnss_hal == nullptr) fprintf(stderr, "null_ptr...\n");
    

    _exit(0);
}
