/*
***
* frameworks/base/cmds/
* 
* adb push out/target/product/mido/system/bin/battvvnx /system/bin
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
#include <binder/IBatteryStats.h> //frameworks/native/libs/binder/include/binder/IBatteryStats.h

using namespace android;



int main()
{
	const String16 name("batterystats");
	//const String16 name("ZoubZoub"); //contrôle neg


    fprintf(stderr, "Début de main de natvvnx, on va se coller au service\n");
    
    sp<IServiceManager> sm = defaultServiceManager();
    sp<IBatteryStats> mBatteryStatService;
    
    if (sm->checkService(name) == nullptr) {
		fprintf(stderr, "Le checkService a foiré\n");
		_exit(1);
	}
	
	mBatteryStatService = interface_cast<IBatteryStats>(sm->getService(name));
    
	if (mBatteryStatService == nullptr) {
		fprintf(stderr, "mBatteryStatService null\n");
		_exit(1);
	}

    _exit(0);
}
