#include "cpuinfo.h"

// 処理分けを行う関数
static void cpu_info_handle(CpuInfoSendMessage *);
// OSで設定されているプロセッサ数を取得してコールバック関数に代入する
static void cpu_get_number(CpuInfoSendMessage *);
// システムで現在利用可能なプロセッサ数取得してコールバック関数に代入する
static void cpu_get_nprocs(CpuInfoSendMessage *);

void cpu_thmain(){

    ssize_t rcv_size;
    int timeout_count = 0;
    struct mq_attr attr;
    mqd_t mqddes;
    struct timespec timeout;
    unsigned int priority;
    unsigned char rbuf[CPUINFO_MSGSIZE];
    MSG_DATA_OBJ *pMsg;
    const char *que_name = CPUINFO_MQ_QUEUE_NAME;
    CpuInfoSendMessage *pcpuinfo;

    mq_unlink(que_name);

    // set queue attribute
    attr.mq_flags = O_NONBLOCK;
    attr.mq_maxmsg = CPUINFO_MQ_MAXMSG;
    attr.mq_msgsize = CPUINFO_MQ_MSGSIZE;

    mqddes = mq_open(que_name, O_RDONLY | O_CREAT, S_IRWXU, &attr);
    if (mqddes < 0) {
        fprintf(stderr, "mq_open failed mqddes:[%d], errno:[%d]\n", mqddes, errno);
        return;
    }
    fprintf(stdout, "スレッド開始...\n");
    
    while(1) {
        clock_gettime(CLOCK_REALTIME, &timeout);

        timeout.tv_sec += 3;
        timeout.tv_nsec = 0;

        rcv_size = mq_timedreceive(mqddes, (char *)&rbuf, CPUINFO_MQ_MSGSIZE, &priority, &timeout);
        if (rcv_size < 0) {
            if (errno == 110) {
                // timeout
                timeout_count++;
                if(timeout_count > 10){
                    fprintf(stdout, "タイムアウト...スレッドを終了します。\n");
                    mq_close(mqddes);
                    mq_unlink(que_name);
                    return;
                }
                fprintf(stdout, "メッセージ待ち...\n");
                continue;
            }
            mq_close(mqddes);
            mq_unlink(que_name);
            return;
        }
        timeout_count = 0;

        pMsg = (MSG_DATA_OBJ *)rbuf;
        pcpuinfo = (CpuInfoSendMessage *)pMsg->name;
        cpu_info_handle(pcpuinfo);
    }

}


static void cpu_info_handle(CpuInfoSendMessage *pcpuinfo){

    switch (pcpuinfo->command_id)
    {
    case CPUINFO_CMD_NUMBER_OF_PROCESSORS:
        cpu_get_number(pcpuinfo);
        break;
    case CPUINFO_CMD_ACTIVE_PROCESSORS:
        cpu_get_nprocs(pcpuinfo);
        break;
    
    default:
        break;
    }


}

// OSで設定されているプロセッサ数を取得してコールバック関数に代入する
static void cpu_get_number(CpuInfoSendMessage *pcpuinfo){
    int nprocs = get_nprocs();
    pcpuinfo->pfunc(nprocs);
}

// システムで現在利用可能なプロセッサ数取得してコールバック関数に代入する
static void cpu_get_nprocs(CpuInfoSendMessage *pcpuinfo){
    int nprocs_conf = get_nprocs_conf();
    pcpuinfo->pfunc(nprocs_conf);
}
