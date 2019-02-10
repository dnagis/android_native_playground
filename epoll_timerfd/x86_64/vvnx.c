/**
 * gcc -o vvnx vvnx.c
 * epoll -> écoute un fd
 * timerfd -> une alarme, qui peut être repeating + sleep resistant.
 * 		http://man7.org/linux/man-pages/man2/timerfd_create.2.html
 * 
 * inspiration: 
 * aosp ->  system/core/healthd/healthd_common.cpp
 * http://www.sourcexr.com/articles/2013/11/12/timer-notifications-using-file-descriptors
 * https://github.com/seiyak/ssebot-samples/blob/master/sample-timerfd-epoll.c
 * 
 * 
 * **/
 
#include <errno.h>
#include <stdio.h>
#include <sys/epoll.h> 
#include <sys/timerfd.h>
#include <unistd.h>
 
static int eventct = 10;
static int epollfd;
static int wakealarm_fd;


int main() { 


		struct epoll_event ev;
		struct epoll_event events[eventct];
		struct itimerspec itval;
		

		
		epollfd = epoll_create(eventct);
		wakealarm_fd = timerfd_create(CLOCK_BOOTTIME_ALARM, TFD_NONBLOCK);
		printf("Création du fd epoll=%i et du fd timerfd=%i\n", epollfd, wakealarm_fd);  
		

		//int timerfd_settime(int fd, int flags, const struct itimerspec *new_value, struct itimerspec *old_value);
		//remplir les 4 sinon settime renvoie -1
		itval.it_value.tv_sec = 3; //initial trigger
		itval.it_value.tv_nsec = 0;
		itval.it_interval.tv_sec = 1; //repeating -> période
		itval.it_interval.tv_nsec = 0;
		
		
		ev.events = EPOLLIN | EPOLLWAKEUP;	
		ev.data.fd = wakealarm_fd;
				
        if (epoll_ctl(epollfd, EPOLL_CTL_ADD, wakealarm_fd, &ev) == -1) {
		printf("Plantade epoll_ctl() add errno=%d\n", errno); // /usr/include/asm-generic/errno-base.h
        return -1;
		}
					
		if (timerfd_settime(wakealarm_fd, 0, &itval, NULL) != 0){
		printf("Plantade timerfd_settime() \n");
		return -1;
		}
		
		int nevents = 0;
		while (1) {	
		
						
		nevents = epoll_wait(epollfd, events, eventct, -1); //to en ms, -1=infini
		//printf("on vient de passer epoll_wait et nevents=%i\n", nevents);
		
		 // demultiplex events, il faut read() = traire le wakealarm_fd sinon epoll_wait() ne bloque que la 1ère fois
		for (int n = 0; n < nevents; ++n) {
			
			if (events[n].data.fd == wakealarm_fd) {
				
				unsigned long long wakeups;
				
				if (read(wakealarm_fd, &wakeups, sizeof(wakeups)) == -1) {
					printf("wakealarm_event: read wakealarm fd failed\n");
					return -1;
				}
				//handle l'event ici
				printf("****timer triggered******\n");
			}
			
			
		}
		
	

		}
                    
        return 0; 
}
