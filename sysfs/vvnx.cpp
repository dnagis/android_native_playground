/*
***
* frameworks/base/cmds/
* 
* adb push out/target/product/mido/system/bin/sysfsvvnx /system/bin
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
#include <fstream>

#include <binder/IServiceManager.h>

using namespace android;



int main()
{
	//alias batterie="expr $((`cat /sys/class/power_supply/battery/charge_counter`*100/`cat /sys/class/power_supply/battery/charge_full`))"
	
	std::string charge, temp;

    std::ifstream ifile_c("/sys/class/power_supply/battery/charge_counter");
    ifile_c >> charge; 
    ifile_c.close();
    std::ifstream ifile_t("/sys/class/power_supply/battery/temp");
	ifile_t >> temp;
	ifile_t.close();

    fprintf(stderr, "charge récup=%s, temp récup=%s\n", charge.c_str(), temp.c_str());

    _exit(0);
}
