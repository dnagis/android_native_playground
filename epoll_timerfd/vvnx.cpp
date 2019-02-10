/**
* frameworks/base/cmds/
* 
adb push out/target/product/mido/system/bin/alrmvvnx /system/bin
* 
   /etc/init/alrmvvnx.rc (chmod 755)
service alrmvvnx /system/bin/alrmvvnx
    class main
    oneshot
 
chmod 755 /system/bin/alrmvvnx
chcon u:object_r:healthd_exec:s0 /system/bin/alrmvvnx
* 
* 
rw fichier dans data/data selinux:
sepolicy/public/healthd.te ajouter -> allow healthd system_data_file:file { getattr read write open };
commenter les lignes de neverallow system_data_file:file no_w_file_perms; ligne 875 system/sepolicy/public/domain.te
pushd out/target/product/mido/system/etc/selinux/; adb push plat_sepolicy.cil plat_and_mapping_sepolicy.cil.sha256 plat_file_contexts /etc/selinux/; popd

* 
 */

#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <cutils/klog.h>
#include <time.h>

#include <sys/epoll.h> 
#include <sys/timerfd.h>

#include <android-base/file.h> //libbase (LOCAL_SHARED_LIBRARIES)



#define LOG_TAG "alrmvvnx"
#define KLOG_LEVEL 6

using namespace android;

static constexpr const char* file_path = "/data/data/essai.txt";

static int eventct = 10;
static int epollfd;
static int wakealarm_fd;


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

int main()
{
	int nevents = 0;

	struct epoll_event ev;
	struct epoll_event events[eventct];
	struct itimerspec itval;
	



	epollfd = epoll_create(eventct);
	wakealarm_fd = timerfd_create(CLOCK_BOOTTIME_ALARM, TFD_NONBLOCK);
	
	KLOG_WARNING(LOG_TAG, "Creation du fd epoll=%i et du fd timerfd=%i\n", epollfd, wakealarm_fd);
	//printf("Création du fd epoll=%i et du fd timerfd=%i\n", epollfd, wakealarm_fd);  
	

	//int timerfd_settime(int fd, int flags, const struct itimerspec *new_value, struct itimerspec *old_value);
	//remplir les 4 sinon settime renvoie -1
	itval.it_value.tv_sec = 20;
	itval.it_value.tv_nsec = 0;
	itval.it_interval.tv_sec = 600; //repeating
	itval.it_interval.tv_nsec = 0;
	
	ev.events = EPOLLIN | EPOLLWAKEUP;	
	ev.data.fd = wakealarm_fd;
	
	if (epoll_ctl(epollfd, EPOLL_CTL_ADD, wakealarm_fd, &ev) == -1) {
	//printf("Plantade epollfd\n");
	KLOG_WARNING(LOG_TAG, "Plantade epollfd\n");
	_exit(1);
	}	
	
	if (timerfd_settime(wakealarm_fd, 0, &itval, NULL) != 0){
	//printf("timerfd_settime() error\n");
	KLOG_WARNING(LOG_TAG, "timerfd_settime() error\n");
	_exit(1);
	}
	

	
	
	//loop epoll_wait()/read()
	while (1) {
		
		
	nevents = epoll_wait(epollfd, events, eventct, -1);  //arg4 = to en ms, -1=infini
	//KLOG_WARNING(LOG_TAG, "nevents=%i\n", nevents);
	//if (nevents == -1) printf("epoll_create failed; errno=%d\n", errno);
    //if (nevents == -1) KLOG_WARNING(LOG_TAG, "epoll_create failed; errno=%d\n", errno);
    
    		 // demultiplex events, il faut read() = traire le wakealarm_fd sinon epoll_wait() ne bloque que la 1ère fois
		for (int n = 0; n < nevents; ++n) {
			
			if (events[n].data.fd == wakealarm_fd) {
				
				unsigned long long wakeups;
				
				if (read(wakealarm_fd, &wakeups, sizeof(wakeups)) == -1) {
					KLOG_WARNING(LOG_TAG, "wakealarm_event: read wakealarm fd failed\n");
					return -1;
				}
				//handle l'event ici
				//KLOG_WARNING(LOG_TAG, "****timer triggered******\n");
				action();
			}
			
			
		}
    
    
    
    
	}
	
	
	
	//error: code will never be executed [-Werror,-Wunreachable-code]
    //_exit(0);
}
