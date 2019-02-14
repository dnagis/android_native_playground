/**
***
* 
* envoi de résultats d'une requete sqlite en POST.
* basé sur les fonctionnalités/folders: 
* sqlite
* httpcurl
* epoll_timerfd

* path où je le mets d'habitude: frameworks/base/cmds/

adb push out/target/product/mido/system/bin/sync_db /system/bin

chmod 755 /system/bin/sync_db
chcon u:object_r:vvnx_exec:s0 /system/bin/sync_db 




* démarrage par init:

   /etc/init/sync_db.rc (chmod 755)
service sync_db /system/bin/sync_db
    class main
    oneshot
 


selinux pour aller bricoler /data/data/com.example.android.bluealrm/databases
system/sepolicy/private/vvnx.te
	typeattribute vvnx coredomain;
	init_daemon_domain(vvnx)
	allow vvnx kmsg_device:chr_file { write open };
	allow vvnx system_app_data_file:dir { search };
	allow vvnx system_app_data_file:file { getattr read write open ioctl lock };
	allow vvnx self:capability { dac_override dac_read_search };
	allow vvnx self:{ udp_socket tcp_socket } { getattr write create getopt setopt connect };
	allow vvnx self:capability { net_raw };
	allow vvnx fwmarkd_socket:sock_file write;
	allow vvnx port:tcp_socket { name_connect };
	allow vvnx netd:unix_stream_socket { connectto };
	wakelock_use(vvnx)
system/sepolicy/public/domain.te
	"-vvnx" dans le neverallow system_app_data_file:dir_file_class_set { create unlink open }; (commentaire: "respect system_app sandboxes")
system/sepolicy/public/netd.te
	allow netd vvnx:fd use;
	allow netd vvnx:tcp_socket { read write getopt setopt };

et il faut pour le package du folder auquel tu veux accéder:
* android:sharedUserId="android.uid.system" dans la première balise manifest
* LOCAL_CERTIFICATE := platform dans le Android.mk
* 
* 
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
#include <cutils/klog.h>

#include <sys/epoll.h> 
#include <sys/timerfd.h>

#include <curl/curl.h>
#include <sqlite3.h>

#define LOG_TAG "sync_db"
#define KLOG_LEVEL 6

static int eventct = 10;
static int epollfd;
static int wakealarm_fd;

void send_via_curl(int i, const unsigned char *mac) {
	//printf("resultat: %i, %s \n", i, mac);
	CURL *curl;
	CURLcode res;	

	curl_global_init(CURL_GLOBAL_ALL);	

	curl = curl_easy_init();
	if(curl) {
	curl_easy_setopt(curl, CURLOPT_URL, "192.168.1.118:8000");
	char str[80];
	char number[40];
	strcpy(str, "id=");	
	sprintf(number, "%i", i);
	strcat(str, number);
	strcat(str, "&mac=");
	strcat(str, (char *)(mac));
	KLOG_WARNING(LOG_TAG, "le POST a cette gueule: %s\n", str); //"id=205&mac=30:AE:A4:04:C3:5A"
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, str);


	res = curl_easy_perform(curl);

	if(res != CURLE_OK)
	KLOG_WARNING(LOG_TAG, "curl_easy_perform() failed: %s\n",
	curl_easy_strerror(res));
	curl_easy_cleanup(curl);
	}
	curl_global_cleanup();
				
}

void fetch_db() {
	
	sqlite3 *db;
   	sqlite3_stmt *stmt;
	int rc;

	rc = sqlite3_open("/data/data/com.example.android.bluealrm/databases/temp.db", &db);  
	char const *sql = "select * from temp WHERE ALRMTIME=1550171171;";	
	
	rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);	
	if (rc != SQLITE_OK) {
		KLOG_WARNING(LOG_TAG, "error: %s", sqlite3_errmsg(db));
	}
	
	while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
		int id           = sqlite3_column_int (stmt, 0);
		const unsigned char *mac = sqlite3_column_text(stmt, 2);
		send_via_curl(id, mac);
	}
	
	if (rc != SQLITE_DONE) {
	KLOG_WARNING(LOG_TAG, "error: %s", sqlite3_errmsg(db));
	}
	
	sqlite3_finalize(stmt);
	sqlite3_close(db);	
	
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
	itval.it_interval.tv_sec = 60; //repeating
	itval.it_interval.tv_nsec = 0;
	
	ev.events = EPOLLIN | EPOLLWAKEUP;	
	ev.data.fd = wakealarm_fd;
	
	if (epoll_ctl(epollfd, EPOLL_CTL_ADD, wakealarm_fd, &ev) == -1) {
	//printf("Plantade epollfd\n"); //kernel/xiaomi/msm8953/include/uapi/asm-generic/errno-base.h
	KLOG_WARNING(LOG_TAG, "Plantade epollfd\n");
	_exit(1);
	}	
	
	if (timerfd_settime(wakealarm_fd, 0, &itval, NULL) != 0){
	//printf("timerfd_settime() error\n"); //kernel/xiaomi/msm8953/include/uapi/asm-generic/errno-base.h
	KLOG_WARNING(LOG_TAG, "timerfd_settime() error\n");
	_exit(1);
	}
	

	
	
	/**loop epoll_wait()/read() le coeur du fonctionnement**/
	while (1) {
		
		
	nevents = epoll_wait(epollfd, events, eventct, -1);  //arg4 = to en ms, -1=infini
	//KLOG_WARNING(LOG_TAG, "nevents=%i\n", nevents);
	//if (nevents == -1) printf("epoll_create failed; errno=%d\n", errno); //kernel/xiaomi/msm8953/include/uapi/asm-generic/errno-base.h
    //if (nevents == -1) KLOG_WARNING(LOG_TAG, "epoll_create failed; errno=%d\n", errno); //kernel/xiaomi/msm8953/include/uapi/asm-generic/errno-base.h
    
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
				fetch_db();
			}
			
			
		}
    
    
    
    
	}
	
	
	
	//error: code will never be executed [-Werror,-Wunreachable-code]
    //_exit(0);
	
}
