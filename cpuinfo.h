#ifndef __COUINFO_H__
#define  __COUINFO_H__
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <errno.h>
#include <time.h>
#include <sys/sysinfo.h>

// 当スレッドが見ているキュー名
#define CPUINFO_MQ_QUEUE_NAME "/cpu_info"
#define CPUINFO_MQ_FLAG 0
#define CPUINFO_MQ_MAXMSG 10
#define CPUINFO_MQ_MSGSIZE 8192
#define CPUINFO_MQ_CURMSGS 0
#define CPUINFO_MSGSIZE 1024
#define CPUINFO_MQ_MODE 0666
#define CPUINFO_MQ_TOP_PRIORITY 0

// COMMAND ID
#define CPUINFO_CMD_NUMBER_OF_PROCESSORS  0x00000001
#define CPUINFO_CMD_ACTIVE_PROCESSORS  0x00000002

typedef void (* CPU_FUNC_POINTER)(int); 


typedef struct {
    char name[128];
    int  age;
    double score;
} MSG_DATA_OBJ;

typedef struct {
    int command_id;
    CPU_FUNC_POINTER pfunc;
} CpuInfoSendMessage;


void cpu_thmain();



#endif