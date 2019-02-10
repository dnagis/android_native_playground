/*
***
* frameworks/base/cmds/
* 
* adb push out/target/product/mido/system/bin/battpropvvnx /system/bin
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
#include <batteryservice/BatteryServiceConstants.h>
#include <batteryservice/IBatteryPropertiesRegistrar.h>

using namespace android;



int main()
{
	const String16 name("batteryproperties");
	//const String16 name("ZoubZoub"); //contrôle neg
		
	/**frameworks/native/services/batteryservice/include/batteryservice/BatteryService.h**/
	struct BatteryProperty val; 

    fprintf(stderr, "Début de main de natvvnx, on va se coller au service...\n");
    
    sp<IServiceManager> sm = defaultServiceManager();
    if (sm->checkService(name) == nullptr) {
		fprintf(stderr, "Le check du service a foiré\n");
		_exit(1);
	}
	
	sp<IBinder> binder = sm->getService(name);
    if (binder == NULL) return 1;
    
    sp<IBatteryPropertiesRegistrar> battery_properties =
        interface_cast<IBatteryPropertiesRegistrar>(binder);
    if (battery_properties == NULL) return 1;

	battery_properties->getProperty(BATTERY_PROP_CHARGE_COUNTER, &val); //BATTERY_PROP_CHARGE_COUNTER <-> "charge counter" dans dumpsys batteryproperties
	
	fprintf(stderr, "recup property = %i\n", (int)val.valueInt64);

	return 0;
    //_exit(0);
}
