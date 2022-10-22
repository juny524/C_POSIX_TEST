#include <stdio.h>
#include "mainthread.h"
#include "read.h"
#include "cpuinfo.h"

void thread_start();

int main(){
    thread_start();

    return 0;
}

void thread_start(){
    // pthread_t thread1, thread2;
    // pthread_create(&thread1,NULL,(void *)thmain,NULL);
    // pthread_create(&thread2,NULL,(void *)thsub,NULL);

    // pthread_join(thread1, NULL);
    // pthread_join(thread2, NULL);
    // printf("thread end......\n");
    pthread_t thread_cpuinfo;
    pthread_t thread2;
    pthread_create(&thread_cpuinfo,NULL,(void *)cpu_thmain,NULL);
    pthread_create(&thread2,NULL,(void *)thsub,NULL);

    pthread_join(thread_cpuinfo, NULL);
    pthread_join(thread2, NULL);
    printf("thread end......\n");

}


