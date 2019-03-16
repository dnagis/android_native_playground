/**
 * Avoir du repeating doze-resistant. Oui je l'ai en java je sais
 * MAIS
 * pour uploader de la database il faut une usine à gaz (voir loctrack)
 * je voulais qq chose de simple.
 * 
 * la découverte majeure c'est quand j'ai greppé wakealarm dans les sources et que je suis tombé sur 
 * system/core/healthd/healthd_common.cpp
 * et que j'ai découvert leur système de epoll/timerfd qui donne équivalent de alarm
 * 
 * Mécanisme basé sur epoll et timerfd qui sont des syscalls
 * epoll (epoll_create, epoll_ctl, epoll_wait) -> écouter un socket (wait bloque tant qu'il n'y a rien sur le socket)
 * timerfd (timerfd_create, timerfd_settime) -> demander au kernel de renvoyer un event sur un fd. même si en sleep, en repeat possible.
 * la bible: les man pages (mais philosophie pas bien expliquée, notamment le fait qu'il faut read le socket pour le traire sinon 
 * il ne bloque plus)
 * c'est faisable sur un ordi of course (voir le dossier x86_64)
 * 
 * un des meilleurs tutos:
 * http://www.sourcexr.com/articles/2013/11/12/timer-notifications-using-file-descriptors
 * 
 * 
 * 
* frameworks/base/cmds/
* 
adb push out/target/product/mido/system/bin/alrmvvnx /system/bin


démarrage par init:

   /etc/init/alrmvvnx.rc (chmod 755)
service alrmvvnx /system/bin/alrmvvnx
    class main
    oneshot
 
chmod 755 /system/bin/alrmvvnx
chcon u:object_r:healthd_exec:s0 /system/bin/alrmvvnx

rw fichier "/data/data/essai.txt" dans data/data selinux rules:
system/sepolicy/public/healthd.te ajouter -> allow healthd system_data_file:file { getattr read write open };
commenter les lignes de neverallow system_data_file:file no_w_file_perms; ligne 875 system/sepolicy/public/domain.te
make selinux_policy
pushd out/target/product/mido/system/etc/selinux/; adb push plat_sepolicy.cil plat_and_mapping_sepolicy.cil.sha256 plat_file_contexts /etc/selinux/; popd


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

#include "header.h" //mon header, où est définie l'action déclenchée à chaque récurrence (dans un autre fichier pour lisibilité)

#define LOG_TAG "alrmvvnx"
#define KLOG_LEVEL 6





static int intervalle = 300; //secondes

//des outils pour le fonctionnement d'epoll. ToDo: Il faudrait décrire le mécanisme pour que ce soit moins obscurs
//ce qu'est la struct epoll_event
static int eventct = 10;
static int epollfd;
static int wakealarm_fd;




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
	itval.it_value.tv_sec = 20; //le premier déclenchement
	itval.it_value.tv_nsec = 0;
	itval.it_interval.tv_sec = intervalle; //repeating
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
    //if (nevents == -1) KLOG_WARNING(LOG_TAG, "epoll_create failed; errno=%d\n", errno); //kernel/xiaomi/msm8953/include/uapi/asm-generic/errno-base.h
    
		// "demultiplex events", il faut read() = traire le wakealarm_fd sinon epoll_wait() ne bloque que la 1ère fois
		for (int n = 0; n < nevents; ++n) {
			
			if (events[n].data.fd == wakealarm_fd) {
				
				unsigned long long wakeups;
				
				if (read(wakealarm_fd, &wakeups, sizeof(wakeups)) == -1) {
					KLOG_WARNING(LOG_TAG, "wakealarm_event: read wakealarm fd failed\n");
					return -1;
				}

				action(); //action déclenchée à chaque récurrence (dans un autre fichier pour lisibilité)
			}
			
			
		}
    
    
    
    
	}
	
	
	
	//error: code will never be executed [-Werror,-Wunreachable-code]
    //_exit(0);
}
