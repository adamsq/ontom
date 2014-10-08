#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include <signal.h>
#include <net/if.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>

#include <linux/can.h>
#include <linux/can/raw.h>
#include "mongoose.h"
#include "ajax.h"
#include "config.h"
#include "error.h"
#include "log.h"
extern void * thread_xml_service(void *) ___THREAD_ENTRY___;
extern void * thread_bms_write_service(void *) ___THREAD_ENTRY___;
extern void * thread_bms_read_service(void *) ___THREAD_ENTRY___;
extern void * thread_measure_service(void *) ___THREAD_ENTRY___;
extern void * thread_charger_service(void *) ___THREAD_ENTRY___;
extern void * thread_backgroud_service(void *) ___THREAD_ENTRY___;
extern void * thread_charge_task_service(void *) ___THREAD_ENTRY___;

// 串口通信 服务线程
// 提供串口通信服务
void *thread_measure_service(void *arg) ___THREAD_ENTRY___
{
    int *done = (int *)arg;
    int mydone = 0;
    if ( done == NULL ) done = &mydone;
    log_printf(INF, "%s running...", __FUNCTION__);

    while ( ! *done ) {
        usleep(5000);
    }
}

// 充电机通信服务线程
void *thread_charger_service(void *arg) ___THREAD_ENTRY___
{
    int *done = (int *)arg;
    int mydone = 0;
    if ( done == NULL ) done = &mydone;
    log_printf(INF, "%s running...", __FUNCTION__);

    while ( ! *done ) {
        usleep(5000);
    }
}

// 后台通信服务线程
void *thread_backgroud_service(void *arg) ___THREAD_ENTRY___
{
    int *done = (int *)arg;
    int mydone = 0;
    if ( done == NULL ) done = &mydone;
    log_printf(INF, "%s running...", __FUNCTION__);

    while ( ! *done ) {
        usleep(5000);
    }
}

// 定时器
static void ontom_timer_sig()
{

}

int main()
{
    const char *user_cfg = NULL;
    pthread_t tid = 0;
    int thread_done[ 7 ] = {0};
    char buff[32];
    int errcode = 0, ret;

    // 读取配置文件的顺序必须是
    // 1. 系统配置文件
    // 2. 用户参数配置文件
    // 需要注意的是，用户配置数据和用户配置数据可以有相同的配置项
    // 但优先级最高的是用户配置数据，如果某个配置项同时出现在系统配置
    // 和用户配置中，那么系统最终采用的值将是用户配置数据中的值
    // 因此这里需要注意的是：
    // * 有两个配置文件是一种冗余设计
    // * 非必要的情况下，分别将系统配置和用户配置分开存储到两个文件中
    config_initlize("ontom.cfg");
    user_cfg = config_read("user_config_file");
    if ( user_cfg == NULL ) {
        log_printf(WRN, "not gave user config file,"
                   "use 'user.cfg' by default.");
        user_cfg = "user.cfg";
    }
    config_initlize(user_cfg);

    // 检查是否需要开启SOCKET端的配置服务器以接受网络端的配置
    user_cfg = config_read("socket_config");
    if ( strcmp(user_cfg, "TURE") ||
         strcmp(user_cfg, "true") ) {
        pthread_create( & tid, NULL, config_drive_service, NULL);
        sprintf(buff, "%d", tid);
        config_write("thread_config_server_id", buff);
    }

    // 启动八公定时器
    Hachiko_init();

    // mongoose 线程，用来处理AJAX请求，解析由客户提交的请求，返回应答的xml文件或其他数据
    ret = pthread_create( & tid, NULL, thread_xml_service, &thread_done[0]);
    if ( 0 != ret ) {
        errcode  = 0x1000;
        log_printf(ERR,
                   "mongoose service start up.                     FAILE!!!!");
        goto die;
    }
    log_printf(INF, "mongoose service start up.                         DONE.");

    // BMS 数据包写线程，从队列中取出要写的数据包并通过CAN总线发送出去
    ret = pthread_create( & tid, NULL, thread_bms_write_service,
                          &thread_done[1]);
    if ( 0 != ret ) {
        errcode  = 0x1001;
        log_printf(ERR,
                   "CAN-BUS reader start up.                       FAILE!!!!");
        goto die;
    }
    log_printf(INF, "CAN-BUS reader start up.                           DONE.");

    // BMS读书举报线程，从CAN总线读取数据包后将数据存入读入数据队列等待处理。
    ret = pthread_create( & tid, NULL, thread_bms_read_service,
                          &thread_done[2]);
    if ( 0 != ret ) {
        errcode  = 0x1002;
        log_printf(ERR,
                   "CAN-BUS writer start up.                       FAILE!!!!");
        goto die;
    }
    log_printf(INF, "CAN-BUS writer start up.                           DONE.");

    // 串口服务线程，和读卡器，采样盒，电能表进行数据交换，测量
    ret = pthread_create( & tid, NULL, thread_measure_service, &thread_done[3]);
    if ( 0 != ret ) {
        errcode  = 0x1003;
        log_printf(ERR,
                   "EX-measure service start up.                   FAILE!!!!");
        goto die;
    }
    log_printf(INF, "EX-measure service start up.                       DONE.");

    // 串口服务线程，和充电机进行通信
    ret = pthread_create( & tid, NULL, thread_charger_service,
                          &thread_done[4]);
    if ( 0 != ret ) {
        errcode  = 0x1004;
        log_printf(ERR,
                   "charger service start up.                      FAILE!!!!");
        goto die;
    }
    log_printf(INF, "charger service start up.                          DONE.");

    // 后台通信线程
    ret = pthread_create( & tid, NULL, thread_backgroud_service,
                          &thread_done[5]);
    if ( 0 != ret ) {
        errcode  = 0x1004;
        log_printf(ERR,
                   "backgroud service start up.                    FAILE!!!!");
        goto die;
    }
    log_printf(INF, "backgroud service start up.                        DONE.");

    // 充电线程，负责充电逻辑
    ret = pthread_create( & tid, NULL, thread_charge_task_service,
                          &thread_done[6]);
    if ( 0 != ret ) {
        errcode  = 0x1005;
        log_printf(ERR,
                   "charge service start up.                       FAILE!!!!");
        goto die;
    }
    log_printf(INF, "charge service start up.                           DONE.");


#if CONFIG_DEBUG_CONFIG >= 1
    config_print();
#endif
    // 主循环中放置看门狗代码
    for ( ;; ) {
        sleep(1);
    }
    return 0;
die:
    log_printf(ERR, "going to die. system aborted!");
    return errcode;
}
