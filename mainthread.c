#include "mainthread.h"
#include "cpuinfo.h"

#include <errno.h>
#include <mqueue.h>
#include <stdio.h>
#include <time.h>

// typedef struct {
//     char name[128];
//     int  age;
//     double score;
// } SAMPLE_OBJ;

// int share_num = 0;

void thmain(){



//     ssize_t rcv_size;
//     struct mq_attr attr;
//     mqd_t mqddes;
//     struct timespec timeout;
//     unsigned int priority;
//     unsigned char rbuf[MSGSIZE];
//     SAMPLE_OBJ *pMsg;
//     const char *que_name = MQ_KS_QUEUE_NAME;
//     CommandData *pcmd_data;

//     mq_unlink(que_name);

//     // set queue attribute
//     attr.mq_flags = O_NONBLOCK;
//     attr.mq_maxmsg = MQ_KS_MAXMSG;
//     attr.mq_msgsize = MSGSIZE;

//     mqddes = mq_open(que_name, O_RDONLY | O_CREAT, S_IRWXU, &attr);
//     if (mqddes < 0) {
//         fprintf(stderr, "mq_open failed mqddes:[%d], errno:[%d]\n", mqddes, errno);
//         return;
//     }
//     fprintf(stdout, "recv loop start...\n");
//     while(1) {
//         clock_gettime(CLOCK_REALTIME, &timeout);

//         // set timeout for 3 seconds
//         timeout.tv_sec += 3;
//         timeout.tv_nsec = 0;

//         // recv message with timeout
//         rcv_size = mq_timedreceive(mqddes, (char *)&rbuf, MSGSIZE, &priority, &timeout);
//         if (rcv_size < 0) {
//             if (errno == 110) {
//                 // timeout
//                 fprintf(stdout, "waiting for recv msg...\n");
//                 continue;
//             }
//             mq_close(mqddes);
//             mq_unlink(que_name);
//             return;
//         }

//         pMsg = (SAMPLE_OBJ *)rbuf;
//         pcmd_data = (CommandData *)pMsg->name;

//         fprintf(stdout, "-------- recv msg --------\n");
// //         fprintf(stdout, "name:[%d], age:[%d], score:[%lf]\n", pMsg->name[0], pMsg->age, pMsg->score);
//         fprintf(stdout, "COMMAND ID:[%d] MESSAGE[%d]\n", pcmd_data->command_id, pcmd_data->testdd);
//         fprintf(stdout, "--------------------------\n\n");
//     }

}


void thsub(){

        mqd_t mqd;
    size_t i;
    char msg[MSGSIZE];
    char msg2[MSGSIZE];
    struct mq_attr attr = {
        .mq_flags = CPUINFO_MQ_FLAG,
        .mq_maxmsg = CPUINFO_MQ_MAXMSG,
        .mq_msgsize = CPUINFO_MQ_MSGSIZE,
        .mq_curmsgs = CPUINFO_MQ_CURMSGS
    };
    
    CpuInfoSendMessage couinfo_msg, couinfo_msg2;

    // share_num+=3;
    // printf("Doing one thing share_num = %d\n", share_num);
    // 送信専用の場合、O_WRONLYを設定する。O_CREATはキューが存在しない場合に作成を行う。
    if ((mqd = mq_open(CPUINFO_MQ_QUEUE_NAME, O_CREAT | O_WRONLY, MODE, &attr)) == -1) {
        perror("mq_open");
        exit(1);
    }

    couinfo_msg.command_id = CPUINFO_CMD_NUMBER_OF_PROCESSORS;
    couinfo_msg.pfunc = cb_nprocs_info;

    memcpy(msg, &couinfo_msg, sizeof(couinfo_msg));

    if (mq_send(mqd, msg, sizeof(msg), TOP_PRIORITY) == -1) {
        perror("mq_send");
        exit(3);
    }

    couinfo_msg2.command_id = CPUINFO_CMD_ACTIVE_PROCESSORS;
    couinfo_msg2.pfunc = cb_nprocs_conf_info;
    
    memcpy(msg2, &couinfo_msg2, sizeof(couinfo_msg2));

    if (mq_send(mqd, msg2, sizeof(msg2), TOP_PRIORITY) == -1) {
        perror("mq_send");
        exit(3);
    }
    
    if (mq_close(mqd) == -1) {
        perror("mq_close");
        exit(4);
    }

    // mqd_t mqd;
    // size_t i;
    // char msg[MSGSIZE];
    // struct mq_attr attr = {
    //     .mq_flags = MQ_KS_FLAG,
    //     .mq_maxmsg = MQ_KS_MAXMSG,
    //     .mq_msgsize = MQ_KS_MSGSIZE,
    //     .mq_curmsgs = MQ_KS_CURMSGS
    // };

    // CommandData cmd_data;
    // cmd_data.command_id = TEMPERATURE;
    // cmd_data.testdd = 4;

    // for(i=0; i<15; i++){
    //     // share_num+=3;
    //     // printf("Doing one thing share_num = %d\n", share_num);
    //     // 送信専用の場合、O_WRONLYを設定する。O_CREATはキューが存在しない場合に作成を行う。
    //     if ((mqd = mq_open(MQ_KS_QUEUE_NAME, O_CREAT | O_WRONLY, MODE, &attr)) == -1) {
    //         perror("mq_open");
    //         exit(1);
    //     }

    //     memcpy(msg, &cmd_data, sizeof(cmd_data));

    //     if (mq_send(mqd, msg, sizeof(msg), TOP_PRIORITY) == -1) {
    //         perror("mq_send");
    //         exit(3);
    //     }
        
    //     if (mq_close(mqd) == -1) {
    //         perror("mq_close");
    //         exit(4);
    //     }
    //     // printf("Send Message = %s\n", msg);
    //     cmd_data.testdd++;

    //     usleep(3);
    // }

}



void cb_nprocs_info(int nprocs_conf){
    printf("OSで設定されているプロセッサ数 : %d\n", nprocs_conf);
}


void cb_nprocs_conf_info(int nprocs_conf){
    printf("システムで現在利用可能なプロセッサ数 : %d\n", nprocs_conf);
}

