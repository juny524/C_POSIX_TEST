#ifndef __MAINTHREAD_H__
#define  __MAINTHREAD_H__
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <mqueue.h>
#include <sys/stat.h>

#define MQ_KS_QUEUE_NAME "/mqueue_kssasas"
#define MQ_KS_QUEUE_NAME2 "/mqueue_kssasas2"
#define MQ_KS_FLAG 0
#define MQ_KS_MAXMSG 10
#define MQ_KS_MSGSIZE 8192
#define MQ_KS_CURMSGS 0
#define MSGSIZE 1024
#define MODE 0666
#define TOP_PRIORITY 0

// COMMAND ID
#define TEMPERATURE  3
#define TEST_COMMAND  1

typedef struct {
    int command_id;
    int testdd;
}CommandData;


void thmain();
void thsub();

// コールバック関数として扱う
void cb_nprocs_info();
void cb_nprocs_conf_info();

#endif