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

#include <hidl/Status.h>

#include <android/hardware/gnss/1.0/IGnss.h>

using android::hardware::Return;
using android::hardware::Void;

using android::hardware::gnss::V1_0::IGnss;
using android::hardware::gnss::V1_0::IGnssCallback;
using android::hardware::gnss::V1_0::GnssLocation;
using android::sp;


//Si la totalité des mthdes ne sont pas implémentées erreur peu informative:
//error: allocating an object of abstract class type 'GnssCallback'
class GnssCallback : public IGnssCallback {
	public:	
	
	virtual ~GnssCallback() = default;
	
    Return<void> gnssLocationCb(const GnssLocation& location) override {
      fprintf(stdout, "Location received... %f %f\n", location.longitudeDegrees, location.latitudeDegrees); //gnss/1.0/types.hal
      return Void();
    }

    Return<void> gnssStatusCb(
        const IGnssCallback::GnssStatusValue /* status */) override {
      return Void();
    }
    
    Return<void> gnssSvStatusCb(
        const IGnssCallback::GnssSvStatus& /* svStatus */) override {
      return Void();
    }
    
    Return<void> gnssNmeaCb(
        int64_t /* timestamp */,
        const android::hardware::hidl_string& /* nmea */) override {
      return Void();
    }
    
    Return<void> gnssSetCapabilitesCb(uint32_t capabilities) override {
      fprintf(stdout,"Capabilities received %d", capabilities);
      return Void();
    }
    
    Return<void> gnssAcquireWakelockCb() override { return Void(); }
    Return<void> gnssReleaseWakelockCb() override { return Void(); }
    Return<void> gnssRequestTimeCb() override { return Void(); }
    
    Return<void> gnssSetSystemInfoCb(
        const IGnssCallback::GnssSystemInfo& info) override {
      fprintf(stdout,"Info received, year %d", info.yearOfHw);
      return Void();
    }


};



int main()
{
	bool result;

    fprintf(stderr, "Début de main de hidlvvnx...\n");
    sp<IGnss> gnss_hal = IGnss::getService();
	if (gnss_hal == nullptr) fprintf(stderr, "null_ptr hal...\n");
	
	sp<IGnssCallback> gnss_cb = new GnssCallback();
    if (gnss_cb == nullptr) fprintf(stderr, "null_ptr cb...\n");
	
	result = gnss_hal->setCallback(gnss_cb);
	if (!result) fprintf(stderr, "erreur setcb...\n");
	
	result = gnss_hal->setPositionMode(
      IGnss::GnssPositionMode::MS_BASED,
      IGnss::GnssPositionRecurrence::RECURRENCE_PERIODIC, 500,
      0, 0);      
    if (!result) fprintf(stderr, "erreur setPM...\n");
    
    result = gnss_hal->start();
    if (!result) fprintf(stderr, "erreur start...\n");
    
    sleep(200);
    
    result = gnss_hal->stop();
    if (!result) fprintf(stderr, "erreur stop...\n");
	
    _exit(0);
}
