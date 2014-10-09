#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <malloc.h>
#include <net/if.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include "bms.h"
#include "charge.h"
#include "config.h"
#include "log.h"
#include "error.h"

/*
 * 本可以将串口通信，SOCKET通信方式也纳入该事件函数，但本着CAN通信优先的原则，暂时将
 * CAN通信时间数据独立出来进行处理。
 *
 * CAN数据包消息处理函数，必须是可重入函数
 * 该函数处于所有需要互斥对象的临界点，有多个线程会直接调用该函数。
 */
static int can_packet_callback(
        struct charge_task * thiz, EVENT_CAN ev, struct event_struct* param)
{
    switch ( ev ) {
    case EVENT_CAN_INIT:
        // 事件循环函数初始化
        thiz->can_bms_status = CAN_NORMAL;
        break;
    case EVENT_CAN_RESET:
        // 事件循环函数复位
        break;
    case EVENT_INVALID:
        // 无效事件，事件填充
        break;
    case EVENT_RX_DONE:
        // 数据包接受成功了
        break;
    case EVENT_RX_ERROR:
        // 数据包接受失败了
        break;
    case EVENT_TX_FAILS:
        // 数据包发送失败了
        break;
    case EVENT_TX_DONE:
        // 数据包发送完成了
        break;
    case EVENT_TX_PRE:
        // 决定是否要发送刚刚准备发送的数据包
        param->evt_param = EVT_RET_OK;
        break;
    case EVENT_TX_REQUEST:
        /*
         * 在这里进行CAN数据包的发送处理
         * 进行数据包发送的条件是：充电枪物理连接正常，进入车辆识别过程，或充电过程。
         */
        break;
    case EVENT_TX_TP_RTS: // 本系统中BMS通信暂时不会使用
        //串口处于连接管理状态时，将会收到该传输数据报请求。
        break;
    case EVENT_TX_TP_CTS:
        //串口处于连接管理状态时，将会收到该传输数据报请求。
        thiz->can_tp_buff_tx[0] = 0x11;
        // 目前的多数据包发送策略是： 无论要发送多少数据包，都一次传输完成
        thiz->can_tp_buff_tx[1] = thiz->tp_param.tp_pack_nr;
        thiz->can_tp_buff_tx[2] = 1;
        thiz->can_tp_buff_tx[3] = 0xFF;
        thiz->can_tp_buff_tx[4] = 0xFF;
        thiz->can_tp_buff_tx[5] = (thiz->tp_param.tp_pgn >> 16) & 0xFF;
        thiz->can_tp_buff_tx[6] = (thiz->tp_param.tp_pgn >> 8 ) & 0xFF;
        thiz->can_tp_buff_tx[7] = thiz->tp_param.tp_pgn & 0xFF;
        thiz->can_tp_buff_nr = 8;
        param->evt_param = EVT_RET_OK;
        break;
    case EVENT_TX_TP_ACK:
        //串口处于连接管理状态时，将会收到该传输数据报请求。
        break;
    case EVENT_TX_TP_ABRT:
        //串口处于连接管理状态时，将会收到该传输数据报请求。
        break;
    default:
        break;
    }

    return 0;
}

// bms 通信 写 服务线程
// 提供bms通信服务
void *thread_bms_write_service(void *arg) ___THREAD_ENTRY___
{
    int *done = (int *)arg;
    int mydone = 0;
    if ( done == NULL ) done = &mydone;

    int s, ti = 0;
    struct sockaddr_can addr;
    struct ifreq ifr;
    struct can_frame frame;
    struct event_struct param;
    unsigned char txbuff[32];

    s = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    int nbytes;

    strcpy(ifr.ifr_name, "can0" );
    ioctl(s, SIOCGIFINDEX, &ifr);

    addr.can_family = PF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;
    bind(s, (struct sockaddr *)&addr, sizeof(addr));

    log_printf(INF, "%s running...s=%d", __FUNCTION__, s);

    param.buff_payload = 0;
    param.evt_param = EVT_RET_INVALID;

    can_packet_callback(task, EVENT_CAN_INIT, &param);

    while ( ! *done ) {
        usleep(90000);

        /*
         * 写线程同时负责写数据和进行连接管理时的控制数据写出，这里需要对当前CAN的
         * 状态进行判定，当CAN处于CAN_NORMAL时进行普通的写操作，当CAN处于CAN_TP_RD
         * 时，采用EVENT_TX_REQUEST 当CAN处于CAN_TP_RD时采用EVENT_TX_TP_REQUEST
         */
        param.buff.tx_buff = txbuff;
        param.buff_size = sizeof(txbuff);
        if ( task->can_bms_status & CAN_NORMAL ) {
            can_packet_callback(task, EVENT_TX_REQUEST, &param);
        } else if ( task->can_bms_status & CAN_TP_RD ) {
            switch ( task->can_bms_status & 0xF0 ) {
            case CAN_TP_CTS:
                can_packet_callback(task, EVENT_TX_TP_CTS, &param);
                break;
            case CAN_TP_ACK:
                can_packet_callback(task, EVENT_TX_TP_ACK, &param);
                break;
            case CAN_TP_ABRT:
                can_packet_callback(task, EVENT_TX_TP_ABRT, &param);
                break;
            default:
                log_printf(WRN, "BMS: can_bms_status crashed(%d).",
                           task->can_bms_status);
                continue;
                break;
            }
        } else if ( task->can_bms_status & CAN_TP_WRITE ) {
            // 当前协议没有用到
            log_printf(WRN, "BMS: CAN_TP_WRITE not implement.");
            continue;
        } else {
            log_printf(WRN, "BMS: invalid can_bms_status: %d.",
                       task->can_bms_status);
            continue;
        }
        if ( EVT_RET_OK != param.evt_param ) {
            continue;
        }

        param.evt_param = EVT_RET_INVALID;
        can_packet_callback(task, EVENT_TX_PRE, &param);
        if ( EVT_RET_TX_ABORT == param.evt_param ) {
            // packet sent abort.
            continue;
        } else if ( EVT_RET_OK != param.evt_param ) {
            continue;
        } else {
            // confirm to send.
        }

        /* 根据GB/T 27930-2011 中相关规定，充电机向BMS发送数据包都没有超过
         * 8字节限制，因此这里不用进行连接管理通信。
         */
        if ( param.buff_payload <= 8 ) {
            nbytes = write(s, &frame, sizeof(struct can_frame));
            if ( nbytes != param.buff_payload ) {
                param.evt_param = EVT_RET_ERR;
                can_packet_callback(task, EVENT_TX_FAILS, &param);
            } else {
                param.evt_param = EVT_RET_OK;
                can_packet_callback(task, EVENT_TX_DONE, &param);
            }
        } else {
            // 大于8字节的数据包在这里处理，程序向后兼容
            static unsigned int notimplement = 0;
            if ( notimplement % 10000 == 0 ) {
                log_printf(INF,
                           "Connection manage for send has not implemented.");
            }
            notimplement ++;
        }

        // 应答结束
        if ( task->can_bms_status == (CAN_TP_RD | CAN_TP_ACK) ) {
            task->can_bms_status = CAN_NORMAL;
            log_printf(DBG, "BMS: connection closed normally.");
        }
        // 传输终止
        if ( task->can_bms_status == (CAN_TP_RD | CAN_TP_ABRT) ) {
            task->can_bms_status = CAN_NORMAL;
            log_printf(DBG, "BMS: connection aborted.");
        }
    }
}

/*
 * Adele - <<rolling in the deep>>
 *
 * 独立处理连接管理时的连接控制，数据重新组装问题，直到数据接收完成，或者出错
 */
typedef enum {
    // 连接管理正常，在数据接收完成前将返回该值
    TP_OK = 0,
    // 数据接收完成后将返回该值
    TP_DONE = 1,
    // 数据连接被终止
    TP_ABORTED = 2,
    // 数据连接中断，主机通信超时将返回该值
    TP_LOSSCONN = 3
}TP_RESULT;
static inline TP_RESULT rolling_in_the_buffer(struct charge_task *thiz)
{
    return TP_OK;
}

// bms 通信 读 服务线程
// 提供bms通信服务
void *thread_bms_read_service(void *arg) ___THREAD_ENTRY___
{
    int *done = (int *)arg;
    int mydone = 0;
    int s, ti = 0;
    struct sockaddr_can addr;
    struct ifreq ifr;
    struct can_frame frame, *buff_in;
    int nbytes;
    struct event_struct param;
    // 用于链接管理的数据缓冲
    unsigned char tp_buff[2048];

    // 缓冲区数据字节数
    unsigned int tp_cnt = 0;
    // 数据包个数
    unsigned int tp_packets_nr = 0;
    // 数据包字节总数
    unsigned int tp_packets_size = 0;
    // 数据包对应的PGN
    unsigned int tp_packet_PGN = 0;

    if ( done == NULL ) done = &mydone;
    s = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    strcpy(ifr.ifr_name, "can0" );
    ioctl(s, SIOCGIFINDEX, &ifr);
    addr.can_family = PF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;
    bind(s, (struct sockaddr *)&addr, sizeof(addr));
    log_printf(INF, "%s running...s=%d", __FUNCTION__, s);

    param.buff.rx_buff = tp_buff;
    param.buff_size = sizeof(tp_buff);
    param.buff_payload = 0;
    while ( ! *done ) {
        usleep(90000);

        if ( task->can_bms_status == CAN_INVALID ) {
            continue;
        }

        memset(&frame, 0, sizeof(frame));
        nbytes = read(s, &frame, sizeof(struct can_frame));
        if ( (frame.can_id & 0xFFFF) != CAN_RCV_ID_MASK ) {
            log_printf(DBG_LV0, "BMS: id not accept %x", frame.can_id);
            continue;
        }
        if ( nbytes != sizeof(struct can_frame) ) {
            param.evt_param = EVT_RET_ERR;
            log_printf(DBG_LV3, "BMS: read frame error %x", frame.can_id);
            can_packet_callback(task, EVENT_RX_ERROR, &param);
            continue;
        }

        /*
         * CAN通信处于普通模式
         *
         * SAE J1939-21 Revised December 2006 版协议中规定
         * TP.DT.PGN 为 0x00EB00  连接管理
         * TP.CM.PGN 为 0x00EC00  数据传输
         * CAN 的数据包大小最大为8字节，因此需要传输大于等于9字节的数据包
         * 时需要使用连接管理功能来传输数据包。
         * 首先由数据发送方，发送一个数据发送请求包，请求包中包含的数据内容有
         * 消息的总长度，需要发送的数据包个数（必须大于1），最大的数据包编号，
         * 这个消息的PGN等
         */
        if ( ((frame.can_id & 0x00FF0000) >> 16) == 0xEB ) {
            /* Data transfer
             * byte[1]: 数据包编号
             * byte[2:8]: 数据
             */

            /*
             * 当数据包接收完成后向BMS发送消息结束应答数据包
             *
             * byte[1]: 0x13
             * byte[2:3]: 消息大小，字节数目
             * byte[4]: 全部数据包数目
             * byte[5]: 0xFF
             * byte[6:8]: PGN
             */
            memcpy(&tp_buff[ (frame.data[0] - 1) * 7 ], &frame.data[1], 7);
            task->tp_param.tp_rcv_pack_nr ++;
            if ( task->tp_param.tp_rcv_pack_nr >= task->tp_param.tp_pack_nr ) {
                param.buff_payload = frame.can_dlc;
                param.evt_param = EVT_RET_INVALID;
                can_packet_callback(task, EVENT_RX_DONE, &param);
                // 数据链接接受完成
                task->can_bms_status = CAN_TP_RD | CAN_TP_ACK;
            }
        } else if ( ((frame.can_id & 0x00FF0000) >> 16 ) == 0xEC ) {
            // Connection managment
            if ( 0x10 == frame.data[0] ) {
                if ( task->can_tp_buff_nr ) {
                    /*
                     * 数据传输太快，还没将缓冲区的数据发送出去
                     */
                    log_printf(WRN, "CAN data transfer too fast.");
                    continue;
                }
                /* request a connection. TP.CM_RTS
                 * byte[1]: 0x10
                 * byte[2:3]: 消息大小，字节数目
                 * byte[4]: 全部数据包的数目
                 * byte[5]: 0xFF
                 * byte[6:8]: PGN
                 */
                tp_cnt = 0;
                tp_packets_size = frame.data[2] * 256 + frame.data[1];
                tp_packets_nr = frame.data[3];
                tp_packet_PGN = frame.data[5] +
                        (frame.data[6] << 8) + (frame.data[7] << 16);
                /*
                 * 接收到这个数据包后向BMS发送准备发送数据包
                 *
                 * byte[1]: 0x11
                 * byte[2]: 可发送的数据包个数
                 * byte[3]: 下一个要发送的数据包编号
                 * byte[4:5]: 0xFF
                 * byte[6:8]: PGN
                 */
                if ( tp_packets_size <= 8 ) {
                    log_printf(WRN,
                         "detect a BMS transfer error, pack size < 8 bytes");
                    continue;
                }
                if ( tp_packets_nr <= 1 ) {
                    log_printf(WRN,
                               "detect a BMS transfer error, pack count < 2");
                    continue;
                }
/*
                // connection request check ok, then do a answer.
                task->can_tp_buff_tx[0] = 0x11;
                // 目前的多数据包发送策略是： 无论要发送多少数据包，都一次传输完成
                task->can_tp_buff_tx[1] = tp_packets_nr;
                task->can_tp_buff_tx[2] = 1;
                task->can_tp_buff_tx[3] = 0xFF;
                task->can_tp_buff_tx[4] = 0xFF;
                task->can_tp_buff_tx[5] = frame.data[5];
                task->can_tp_buff_tx[6] = frame.data[6];
                task->can_tp_buff_tx[7] = frame.data[7];
                task->can_tp_buff_nr = 8;
                log_printf(DBG, "CAN status change to CAN_TP_RD");
*/
                task->tp_param.tp_pack_nr = tp_packets_nr;
                task->tp_param.tp_size = tp_packets_size;
                task->tp_param.tp_pgn = tp_packet_PGN;
                task->tp_param.tp_rcv_bytes = 0;
                task->tp_param.tp_rcv_pack_nr = 0;
                task->can_bms_status = CAN_TP_RD | CAN_TP_CTS;
                log_printf(DBG_LV2,
                           "BMS: data connection accepted, rolling...");
            } else if ( 0xFF == frame.data[0] ) {
                /* connection abort.
                 * byte[1]: 0xFF
                 * byte[2:5]: 0xFF
                 * byte[6:8]: PGN
                 */
                log_printf(DBG_LV2, "BMS: %08X",
                           *(unsigned int*)(&frame.data[0]));
            } else {
                //omited.
                log_printf(DBG_LV3, "BMS: %08X",
                           *(unsigned int*)(&frame.data[0]));
            }
        } else {
            param.buff_payload = frame.can_dlc;
            param.evt_param = EVT_RET_INVALID;
            can_packet_callback(task, EVENT_RX_DONE, &param);
            log_printf(DBG_LV0, "BMS: read a frame done. %08X", frame.can_id);
        }

        if ( task->can_bms_status == CAN_NORMAL ) {
        } else if ( task->can_bms_status == CAN_TP_RD ) {
            // CAN通信处于连接管理模式
        }

#if 0
        log_printf(DBG, "TX%d---%X:%d %02X %02X %02X %02X %02X %02X %02X %02X",
                    ti++,
                    frame.can_id,
                    nbytes,
                    frame.data[0],
                    frame.data[1],
                    frame.data[2],
                    frame.data[3],
                    frame.data[4],
                    frame.data[5],
                    frame.data[6],
                    frame.data[7]
                );
#endif
    }
}

