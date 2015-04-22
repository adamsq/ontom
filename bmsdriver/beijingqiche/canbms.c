#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include <stdarg.h>
#include <malloc.h>
#include <ifaddrs.h>
#include <signal.h>
#include <netdb.h>
#include <fcntl.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <net/if.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <arpa/inet.h>

#include "../../thirdpart/sqlite/sqlite3.h"

#include "../../ontom/error.h"
#include "../../lib/lj/stdafx.h"
#include "../../lib/bms/father.h"
#include "../../ontom/charge.h"
#include "pgn.h"

int about_packet_reciev_done(struct charge_job *thiz, struct bms_event_struct *param);
void heart_beart_notify_proc(Hachiko_EVT evt, void* _private, const struct Hachiko_food *self);

//---------------------------------------------------------------------
//---------------------------------------------------------------------
// 握手-CRM-充电机辨识报文
int gen_packet_PGN256(struct charge_job * thiz, struct bms_event_struct* param)
{
    struct can_pack_generator *gen = gen_search(thiz->bms.generator, thiz->bms.can_pack_gen_nr, PGN_CRM);

    if ( 0 == bit_read(thiz, F_BMS_RECOGNIZED) ) {
        param->buff.tx_buff[0] = BMS_NOT_RECOGNIZED;
    } else {
        param->buff.tx_buff[0] = BMS_RECOGNIZED;
        bit_set(thiz, F_VEHICLE_RECOGNIZED);
    }
    log_printf(DBG_LV3, "BMS: 握手-CRM-充电机辨识报文"RED("%s"),
               bit_read(thiz, F_BMS_RECOGNIZED)?"未识别":"已识别");

    param->buff.tx_buff[1] = 0x01;
    strcpy((char * __restrict__)&param->buff.tx_buff[2], "ZH-CN");
    param->buff.tx_buff[7] = 0xFF;
    param->buff_payload = gen->datalen;
    param->can_id = gen->prioriy << 26 | gen->can_pgn << 8 | CAN_TX_ID_MASK | CAN_EFF_FLAG;

    param->evt_param = EVT_RET_OK;

    gen->can_counter ++;

    return 0;
}

// 配置-CTS-充电机发送时间同步信息
int gen_packet_PGN1792(struct charge_job * thiz, struct bms_event_struct* param)
{
    struct can_pack_generator *gen = gen_search(thiz->bms.generator, thiz->bms.can_pack_gen_nr, PGN_CTS);
    struct pgn1792_CTS cts;
    time_t timep;
    struct tm *p;

    time(&timep);
    p =localtime(&timep);
    if ( p == NULL ) {
        param->evt_param = EVT_RET_ERR;
        return 0;
    }
    cts.spn2823_bcd_sec = (((p->tm_sec / 10 ) & 0x0F ) << 4) |
            ((p->tm_sec % 10) & 0x0F);
    cts.spn2823_bcd_min = (((p->tm_min / 10 ) & 0x0F ) << 4) |
            ((p->tm_min % 10) & 0x0F);
    cts.spn2823_bcd_hour = (((p->tm_hour / 10 ) & 0x0F ) << 4) |
            ((p->tm_hour % 10) & 0x0F);
    cts.spn2823_bcd_day = (((p->tm_mday / 10 ) & 0x0F ) << 4) |
            ((p->tm_mday % 10) & 0x0F);
    cts.spn2823_bcd_mon = (((p->tm_mon / 10 ) & 0x0F ) << 4) |
            ((p->tm_mon % 10) & 0x0F);
    cts.spn2823_bcd_year_h = (((p->tm_year / 100 ) & 0x0F ) << 4) |
            ((p->tm_year % 100) & 0x0F);
    cts.spn2823_bcd_year_l = (((p->tm_year / 10 ) & 0x0F ) << 4) |
            ((p->tm_year % 10) & 0x0F);

    memset(param->buff.tx_buff, 0xFF, 8);
    memcpy(param->buff.tx_buff, &cts, sizeof(struct pgn1792_CTS));

    param->buff_payload = gen->datalen;
    param->can_id =  gen->prioriy << 26 | gen->can_pgn << 8 | CAN_TX_ID_MASK | CAN_EFF_FLAG;

    param->evt_param = EVT_RET_OK;

    gen->can_counter ++;

    return 0;
}

// 配置-CML-充电机最大输出能力
int gen_packet_PGN2048(struct charge_job * thiz, struct bms_event_struct* param)
{
    struct can_pack_generator *gen = gen_search(thiz->bms.generator, thiz->bms.can_pack_gen_nr, PGN_CML);
    struct pgn2048_CML cml;

    cml.spn2824_max_output_voltage = 7500;
    cml.spn2825_min_output_voltage = 2400;
    cml.spn2826_max_output_current = 3000;
    memset(param->buff.tx_buff, 0xFF, 8);
    memcpy((void * __restrict__)param->buff.tx_buff, &cml, sizeof(struct pgn2048_CML));

    param->buff_payload = gen->datalen;
    param->can_id =  gen->prioriy << 26 | gen->can_pgn << 8 | CAN_TX_ID_MASK | CAN_EFF_FLAG;

    param->evt_param = EVT_RET_OK;

    gen->can_counter ++;

    return 0;
}

// 配置-CRO-充电机输出准备就绪状态
int gen_packet_PGN2560(struct charge_job * thiz, struct bms_event_struct* param)
{
    struct can_pack_generator *gen = gen_search(thiz->bms.generator, thiz->bms.can_pack_gen_nr, PGN_CRO);
    struct pgn2560_CRO cro;

    cro.spn2830_charger_ready_for_charge = CHARGER_READY_FOR_CHARGE;

    memset(param->buff.tx_buff, 0xFF, 8);
    memcpy(param->buff.tx_buff, &cro, sizeof(struct pgn2560_CRO));

    param->buff_payload = gen->datalen;
    param->can_id =  gen->prioriy << 26 | gen->can_pgn << 8 | CAN_TX_ID_MASK | CAN_EFF_FLAG;

    param->evt_param = EVT_RET_OK;

    gen->can_counter ++;

    return 0;
}

// 充电-CCS-充电机充电状态
int gen_packet_PGN4608(struct charge_job * thiz, struct bms_event_struct* param)
{
    struct can_pack_generator *gen = gen_search(thiz->bms.generator, thiz->bms.can_pack_gen_nr, PGN_CCS);
    struct pgn4608_CCS ccs;

    ccs.spn3081_output_voltage = 7500;
    ccs.spn3082_outpu_current  = 1200;
    ccs.spn3083_charge_time = 122;

    memset(param->buff.tx_buff, 0xFF, 8);
    memcpy((void * __restrict__)param->buff.tx_buff, &ccs, sizeof(struct pgn4608_CCS));

    param->buff_payload = gen->datalen;
    param->can_id =  gen->prioriy << 26 | gen->can_pgn << 8 | CAN_TX_ID_MASK | CAN_EFF_FLAG;

    param->evt_param = EVT_RET_OK;

    gen->can_counter ++;

    return 0;
}

// 充电-CST-充电机中止充电
int gen_packet_PGN6656(struct charge_job * thiz, struct bms_event_struct* param)
{
    struct can_pack_generator *gen = gen_search(thiz->bms.generator, thiz->bms.can_pack_gen_nr, PGN_CST);
    (void)thiz;
    (void)param;
    (void)gen;
    return 0;

}

// 结束-CSD-充电机统计数据
int gen_packet_PGN7424(struct charge_job * thiz, struct bms_event_struct* param)
{
    struct can_pack_generator *gen = gen_search(thiz->bms.generator, thiz->bms.can_pack_gen_nr, PGN_CSD);
    (void)thiz;
    (void)param;
    (void)gen;
    return 0;

}

// 错误-CEM-充电机错误报文
int gen_packet_PGN7936(struct charge_job * thiz, struct bms_event_struct* param)
{
    struct can_pack_generator *gen = gen_search(thiz->bms.generator, thiz->bms.can_pack_gen_nr, PGN_CEM);
    (void)thiz;
    (void)param;
    (void)gen;
    return 0;
}

// 驱动对外接口
int driver_main_proc(struct charge_job *thiz, BMS_EVENT_CAN ev,
                        struct bms_event_struct *param, struct bmsdriver *drv)
{
    int ret;

    switch ( ev ) {
    case EVENT_CAN_INIT:
        // 事件循环函数初始化
        thiz->bms.can_bms_status = CAN_NORMAL;
        thiz->bms.can_heart_beat.Hachiko_notify_proc= heart_beart_notify_proc;
        ret = Hachiko_new(&thiz->bms.can_heart_beat, HACHIKO_AUTO_FEED, 1, thiz);
        if ( ret == ERR_OK ) {
            log_printf(INF, "BMS: CHARGER change stage to "RED("CHARGE_STAGE_HANDSHACKING"));
            thiz->bms.charge_stage = CHARGE_STAGE_HANDSHACKING;
            //thiz->charge_stage = CHARGE_STAGE_CONFIGURE;
        } else {
            log_printf(ERR, "BMS: 启动定时器错误.");
        }
        break;
    case EVENT_CAN_RESET:
        // 事件循环函数复位
        // 当发生通信中断时会收到该事件
        break;
    case EVENT_INVALID:
        // 无效事件，事件填充
        break;
    case EVENT_RX_DONE:
        // 数据包接受成功了
        about_packet_reciev_done(thiz, param);
        break;
    case EVENT_RX_ERROR:
        // 数据包接受失败了
        break;
    case EVENT_TX_FAILS:
        // 数据包发送失败了
        log_printf(ERR, "BMS: packet send faile.");
        break;
    case EVENT_TX_DONE:
        // 数据包发送完成了
        log_printf(DBG_LV0, "BMS: packet sent. %08X", param->can_id);
        if ( (param->can_id & 0x00FF0000) == (PGN_CRM << 8) &&
             bit_read(thiz, F_BMS_RECOGNIZED ) &&
             bit_read(thiz, F_VEHICLE_RECOGNIZED ) &&
             thiz->bms.charge_stage == CHARGE_STAGE_HANDSHACKING) {
            thiz->bms.charge_stage = CHARGE_STAGE_CONFIGURE;
            log_printf(INF, "BMS: CHARGER change stage to "RED("CHARGE_STAGE_CONFIGURE"));
        }
        if ( (param->can_id & 0x00FF0000) == (PGN_CRO << 8) &&
             bit_read(thiz, F_CHARGER_READY) &&
             bit_read(thiz, F_BMS_READY ) &&
             thiz->bms.charge_stage == CHARGE_STAGE_CONFIGURE ) {
            thiz->bms.charge_stage = CHARGE_STAGE_CHARGING;
            log_printf(INF,
              "BMS: CHARGER change stage to "RED("CHARGE_STAGE_CHARGING"));
        }
        break;
    case EVENT_TX_PRE:
        // 决定是否要发送刚刚准备发送的数据包
        param->evt_param = EVT_RET_OK;
        break;
    case EVENT_TX_REQUEST:
        /*
         * 在这里进行CAN数据包的发送处理
         * 进行数据包发送的条件是：充电枪物理连接正常，进入车辆识别过程，或充电过程。
         *
         * 数据包的发送，优先级最高的是错误报文输出，若是遇到周期性发送的数据包在发送
         * 时序上有重叠的问题，那么在这里的处理方式是，先到先处理，例如在若干个循环内
         * 数据包A，B的发送周期是10ms, 30ms，那么A，B的发送时时序应该是如下方式
         * T (ms)      数据包
         * |             |
         * 0             A
         * 0  + $$       B
         * |             |
         * |             |
         * 10            A
         * |             |
         * |             |
         * 20            A
         * |             |
         * |             |
         * 30            A
         * 30 + $$       B
         * |             |
         * |             |
         * ...         ...
         *
         * $$ 表示最小的循环控制周期，一般来说是绝对小于数据包的发送周期的
         * 所以会有如下的控制逻辑结构
         * if ( ... ) {
         * } else if ( ... ) {
         * } else if ( ... ) {
         * } else ;
         * 在若干个循环控制周期内，数据包都能按照既定的周期发送完成.
         */
        switch ( thiz->bms.charge_stage ) {
        case CHARGE_STAGE_INVALID:
            param->evt_param = EVT_RET_ERR;
            break;
        case CHARGE_STAGE_HANDSHACKING:
            #if 1
            do {
                struct can_pack_generator *gen = gen_search(thiz->bms.generator,
                                                            thiz->bms.can_pack_gen_nr, PGN_CRM);
                if ( gen && gen->heartbeat >= gen->period ) {
                    gen_packet_PGN256(thiz, param);
                    gen->heartbeat = 0;
                } else {
                    //log_printf(DBG_LV0, "BMS: inner error. %d", __LINE__);
                }
                gen = gen_search(thiz->bms.generator, thiz->bms.can_pack_gen_nr, PGN_CEM);
                if ( gen && gen->heartbeat >= gen->period ) {
                    gen_packet_PGN7936(thiz, param);
                    gen->heartbeat = 0;
                } else {
                    //log_printf(DBG_LV0, "BMS: inner error. %d", __LINE__);
                }
            } while (0);
            #endif
            break;
        case CHARGE_STAGE_CONFIGURE:
            do {
                struct can_pack_generator *gen = gen_search(thiz->bms.generator, thiz->bms.can_pack_gen_nr, PGN_CTS);
                if ( gen && gen->heartbeat >= gen->period ) {
                    gen_packet_PGN1792(thiz, param);
                    gen->heartbeat = 0;
                }
                else if ( gen = gen_search(thiz->bms.generator, thiz->bms.can_pack_gen_nr, PGN_CML) &&
                          gen->heartbeat >= gen->period ) {
                    gen_packet_PGN2048(thiz, param);
                    gen->heartbeat = 0;
                }
                else if (gen = gen_search(thiz->bms.generator, thiz->bms.can_pack_gen_nr, PGN_CRO) &&
                         gen->heartbeat >= gen->period ) {
                    gen_packet_PGN2560(thiz, param);
                    gen->heartbeat = 0;
                }
                gen = gen_search(thiz->bms.generator, thiz->bms.can_pack_gen_nr, PGN_CEM);
                if ( gen && gen->heartbeat >= gen->period ) {
                    gen_packet_PGN7936(thiz, param);
                    gen->heartbeat = 0;
                }
            } while (0);
            break;
        case CHARGE_STAGE_CHARGING:
            do {
                struct can_pack_generator *gen = gen_search(thiz->bms.generator, thiz->bms.can_pack_gen_nr, PGN_CCS);
                if ( gen && gen->heartbeat >= gen->period ) {
                    gen_packet_PGN4608(thiz, param);
                    gen->heartbeat = 0;
                }
                else if ( gen = gen_search(thiz->bms.generator, thiz->bms.can_pack_gen_nr, PGN_CST) &&
                          gen->heartbeat >= gen->period ) {
                    gen_packet_PGN6656(thiz, param);
                    gen->heartbeat = 0;
                }
                gen = gen_search(thiz->bms.generator, thiz->bms.can_pack_gen_nr, PGN_CEM);
                if ( gen && gen->heartbeat >= gen->period ) {
                    gen_packet_PGN7936(thiz, param);
                    gen->heartbeat = 0;
                }
            } while (0);
            break;
        case CHARGE_STAGE_DONE:
            do {
                struct can_pack_generator *gen = gen_search(thiz->bms.generator, thiz->bms.can_pack_gen_nr, PGN_CSD);
                if ( gen && gen->heartbeat >= gen->period ) {
                    gen_packet_PGN7424(thiz, param);
                    gen->heartbeat = 0;
                }
                gen = gen_search(thiz->bms.generator, thiz->bms.can_pack_gen_nr, PGN_CEM);
                if ( gen && gen->heartbeat >= gen->period ) {
                    gen_packet_PGN7936(thiz, param);
                    gen->heartbeat = 0;
                }
            } while (0);
            break;
        default:
            break;
        }
        break;
    case EVENT_TX_TP_RTS: // 本系统中BMS通信暂时不会使用
        //串口处于连接管理状态时，将会收到该传输数据报请求。
        break;
    case EVENT_TX_TP_CTS:
    {
        /*串口处于连接管理状态时，将会收到该传输数据报请求。
         * 当数据包接收完成后向BMS发送消息结束应答数据包
         *
         * byte[1]: 0x13
         * byte[2:3]: 消息大小，字节数目
         * byte[4]: 全部数据包数目
         * byte[5]: 0xFF
         * byte[6:8]: PGN
         */

        param->buff.tx_buff[0] = 0x11;
        // 目前的多数据包发送策略是： 无论要发送多少数据包，都一次传输完成
        param->buff.tx_buff[1] = thiz->bms.can_tp_param.tp_pack_nr;
        param->buff.tx_buff[2] = 1;
        param->buff.tx_buff[3] = 0xFF;
        param->buff.tx_buff[4] = 0xFF;
        param->buff.tx_buff[5] = (thiz->bms.can_tp_param.tp_pgn >> 16) & 0xFF;
        param->buff.tx_buff[6] = (thiz->bms.can_tp_param.tp_pgn >> 8 ) & 0xFF;
        param->buff.tx_buff[7] = thiz->bms.can_tp_param.tp_pgn & 0xFF;
        param->buff_payload = 8;
        param->can_id = 0x1cecf456 | CAN_EFF_FLAG;
        param->evt_param = EVT_RET_OK;
    }
        break;
    case EVENT_TX_TP_ACK:
    {
        //串口处于连接管理状态时，将会收到该传输数据报请求。
        param->buff.tx_buff[0] = 0x13;
        // 目前的多数据包发送策略是： 无论要发送多少数据包，都一次传输完成
        param->buff.tx_buff[1] = thiz->bms.can_tp_param.tp_size & 0xFF;
        param->buff.tx_buff[2] = (thiz->bms.can_tp_param.tp_size >> 8) & 0xFF;
        param->buff.tx_buff[3] = thiz->bms.can_tp_param.tp_pack_nr;
        param->buff.tx_buff[4] = 0xFF;
        param->buff.tx_buff[5] = (thiz->bms.can_tp_param.tp_pgn >> 16) & 0xFF;
        param->buff.tx_buff[6] = (thiz->bms.can_tp_param.tp_pgn >> 8 ) & 0xFF;
        param->buff.tx_buff[7] = thiz->bms.can_tp_param.tp_pgn & 0xFF;
        param->buff_payload = 8;
        param->can_id = 0x1cecf456 | CAN_EFF_FLAG;
        param->evt_param = EVT_RET_OK;
    }
        break;
    case EVENT_TX_TP_ABRT:
        //串口处于连接管理状态时，将会收到该传输数据报请求。
        break;
    default:
        break;
    }

    return ERR_OK;
}

// 数据包超时心跳包, 定时器自动复位, 一个单位时间一次
void heart_beart_notify_proc(Hachiko_EVT evt, void* _private, const struct Hachiko_food *self)
{
    if (evt == HACHIKO_TIMEOUT ) {
        unsigned int i = 0;
        struct charge_job * thiz = (struct charge_job *)_private;
        struct can_pack_generator *gen, *me;
        for ( i = 0; i < thiz->bms.can_pack_gen_nr; i++ ) {
            gen = & thiz->bms.generator[i];
            if ( gen->stage == thiz->bms.charge_stage ) {
                if ( gen->heartbeat < gen->period ) {
                    gen->heartbeat += 1;
                } else {
                    gen->heartbeat = gen->period;
                }
            } else {
                gen->heartbeat = 0;
            }
        }


        /*
         * 为了能够侦探到接受数据包的超时事件，需要在这里进行一个计数操作
         * 当can_silence 计数大于等于 can_tolerate_silence 时认为对应数据包接收超时，需要在BMS逻辑主线程
         * 中做相应处理.
         *
         * BEM和CEM不在超时统计范围内
         */
        for ( i = 0; i < thiz->bms.can_pack_gen_nr; i++ ) {
            me = &thiz->bms.generator[i];
            if ((bit_read(thiz, F_GUN_1_PHY_CONN_STATUS)&&
                 bit_read(thiz, F_GUN_1_ASSIT_PWN_SWITCH_STATUS))
                    ||
                (bit_read(thiz, F_GUN_2_PHY_CONN_STATUS)&&
                 bit_read(thiz, F_GUN_1_ASSIT_PWN_SWITCH_STATUS))){
                me->can_silence ++;
            } else continue;
            if ( me->can_tolerate_silence < me->can_silence ) {
                switch (thiz->bms.charge_stage) {
                case CHARGE_STAGE_HANDSHACKING:
                    if (me->can_pgn != PGN_BRM) break;
                        if ( !bit_read(thiz, S_BMS_COMM_DOWN) ) {
                            bit_set(thiz, S_BMS_COMM_DOWN);
                            log_printf(WRN, "BMS: 握手阶段BMS通信"RED("故障"));
                            thiz->bms.charge_stage = CHARGE_STAGE_HANDSHACKING;
                        }
                    break;
                case CHARGE_STAGE_CONFIGURE:
                    if (me->can_pgn != PGN_BCP) break;
                        if ( !bit_read(thiz, S_BMS_COMM_DOWN) ) {
                            bit_set(thiz, S_BMS_COMM_DOWN);
                            log_printf(WRN, "BMS: 配置阶段BMS通信"RED("故障"));
                            thiz->bms.charge_stage = CHARGE_STAGE_HANDSHACKING;
                        }
                    break;
                case CHARGE_STAGE_CHARGING:
                    if (me->can_pgn != PGN_BCL) break;
                        if ( !bit_read(thiz, S_BMS_COMM_DOWN) ) {
                            bit_set(thiz, S_BMS_COMM_DOWN);
                            log_printf(WRN, "BMS: 充电阶段BMS通信"RED("故障"));
                            thiz->bms.charge_stage = CHARGE_STAGE_HANDSHACKING;
                        }
                    break;
                case CHARGE_STAGE_DONE:
                    if (me->can_pgn != PGN_BSD) break;
                        if ( !bit_read(thiz, S_BMS_COMM_DOWN) ) {
                            bit_set(thiz, S_BMS_COMM_DOWN);
                            log_printf(WRN, "BMS: 充电完成阶段BMS通信"RED("故障"));
                            thiz->bms.charge_stage = CHARGE_STAGE_HANDSHACKING;
                        }
                    break;
                default:
                    break;
                }
            }
        }
    }
}


// CAN数据包接受完成
int about_packet_reciev_done(struct charge_job *thiz, struct bms_event_struct *param)
{
    struct can_pack_generator *gen = NULL;

    switch ( param->can_id & 0x00FF00 ) {
    case PGN_CRM :// 0x000100,
        break;
    case PGN_CTS :// 0x000700,
        break;
    case PGN_CML :// 0x000800,
        break;
    case PGN_CCS :// 0x001200,
        break;
    case PGN_CST :// 0x001A00,
        break;
    case PGN_CSD :// 0x001D00,
        break;
    case PGN_CRO :// 0x000A00,
        break;
    case PGN_CEM :// 0x001F00
        break;
    case PGN_BRM :// 0x000200, BMS 车辆辨识报文
        gen = gen_search(thiz->bms.generator, thiz->bms.can_pack_gen_nr, PGN_BRM);
        if ( gen ) {
            gen->can_counter ++;
            gen->can_silence = 0;
        }
        if ( bit_read(thiz, S_BMS_COMM_DOWN) ) {
            log_printf(INF, "BMS: BMS 通信"GRN("恢复"));
        }
        bit_clr(thiz, S_BMS_COMM_DOWN);
        __dump_can_param(param);

        if ( param->buff_payload == 8 ) {
            memcpy(&thiz->bms.vehicle_info, param->buff.rx_buff, 8);
        } else if ( param->buff_payload == sizeof(struct pgn512_BRM) ) {
            memcpy(&thiz->bms.vehicle_info,
                   param->buff.rx_buff, sizeof(struct pgn512_BRM));
        }

        if ( thiz->bms.vehicle_info.spn2565_bms_version[0] == 0x00 &&
             thiz->bms.vehicle_info.spn2565_bms_version[1] == 0x01 &&
             thiz->bms.vehicle_info.spn2565_bms_version[2] == 0x00 ) {

        } else {
            log_printf(WRN,
                  "BMS: BMS not recognized due to invalid BMS VERSION(SPN2565)."
                       "%02X%02X%02X",
                       thiz->bms.vehicle_info.spn2565_bms_version[0],
                       thiz->bms.vehicle_info.spn2565_bms_version[1],
                       thiz->bms.vehicle_info.spn2565_bms_version[2]);
            bit_clr(thiz, F_BMS_RECOGNIZED);
            break;
        }

        if ( thiz->bms.vehicle_info.spn2566_battery_type == 0 ||
             (thiz->bms.vehicle_info.spn2566_battery_type > 0x08 &&
              thiz->bms.vehicle_info.spn2566_battery_type < 0xFF) ) {
            log_printf(WRN,
                   "BMS: BMS not recognized due to invalid BATTERY TYPE(SPN2566)");
            bit_clr(thiz, F_BMS_RECOGNIZED);
            break;
        }

        if ( thiz->bms.vehicle_info.spn2567_capacity / 10.0f > 1000.0f ) {
            log_printf(WRN,
                   "BMS: BMS not recognized due to invalid CAP INFO(SPN2567)");
            bit_clr(thiz, F_BMS_RECOGNIZED);
            break;
        }

        if ( thiz->bms.vehicle_info.spn2568_volatage / 10.0f > 750.0f ) {
            log_printf(WRN,
                  "BMS: BMS not recognized due to invalid VOLTAGE INFO(SPN2568)");
            bit_clr(thiz, F_BMS_RECOGNIZED);
            break;
        }
        log_printf(INF, "BMS: BMS recognized....CAP: %d A.H, VOL: %d V",
                   thiz->bms.vehicle_info.spn2567_capacity,
                   thiz->bms.vehicle_info.spn2568_volatage);
        if ( ! bit_read(thiz, F_BMS_RECOGNIZED ) ) {
            // send recognized event from here.
        }
        bit_set(thiz, F_BMS_RECOGNIZED);
        break;
    case PGN_BCP :// 0x000600, BMS 配置报文
        gen = gen_search(thiz->bms.generator, thiz->bms.can_pack_gen_nr, PGN_BCP);
        if ( gen ) {
            gen->can_counter ++;
            gen->can_silence = 0;
        }
        if ( bit_read(thiz, S_BMS_COMM_DOWN) ) {
            log_printf(INF, "BMS: BMS 通信"GRN("恢复"));
        }
        bit_clr(thiz, S_BMS_COMM_DOWN);

        if ( param->buff_payload != 13 ) {
            log_printf(WRN, "BMS: BCP packet size crash, need 13 gave %d",
                       param->buff_payload);
            break;
        }
        memcpy(&thiz->bms.bms_config_info, param->buff.rx_buff,
               sizeof(struct pgn1536_BCP));

        if ( thiz->bms.bms_config_info.spn2816_max_charge_volatage_single_battery /
                100.0f > 24.0f ) {
            log_printf(WRN,
                       "BMS: max_charge_volatage_single_battery out of rang(0-24)"
                       "gave %.2f V (SPN2816)",
             thiz->bms.bms_config_info.spn2816_max_charge_volatage_single_battery /
                       100.0f);
            break;
        }

        if ( thiz->bms.bms_config_info.spn2817_max_charge_current / 10.0f >
             400.0f ) {
            log_printf(WRN, "BMS: max_charge_current out of rang(-400-0)"
                       "gave %.1f V (SPN2816)",
                     thiz->bms.bms_config_info.spn2817_max_charge_current / 10.0f);
            break;
        }

        if ( thiz->bms.bms_config_info.spn2818_total_energy / 10.0f > 1000.0f ) {
            log_printf(WRN, "BMS: total_energy out of rang(0-1000 KW.H)"
                       "gave %.1f KW.H (SPN2818)",
                     thiz->bms.bms_config_info.spn2818_total_energy / 10.0f);
            break;
        }

        if ( thiz->bms.bms_config_info.spn2819_max_charge_voltage / 10.0f >
                750.0f ) {
            log_printf(WRN, "BMS: max_charge_voltage out of rang(0-750 V)"
                       "gave %.1f V (SPN2819)",
                     thiz->bms.bms_config_info.spn2819_max_charge_voltage / 10.0f);
            break;
        }

        log_printf(INF, "BMS: BCP done, BSVH: %d V, MAXi: %d A, "
                   "CAP: %d KW.H, M-V-C: %d V, M-T: %d C, CAP-statu: %d %%",
                   "V: %d V",
                   thiz->bms.bms_config_info.spn2816_max_charge_volatage_single_battery,
                   thiz->bms.bms_config_info.spn2817_max_charge_current,
                   thiz->bms.bms_config_info.spn2818_total_energy,
                   thiz->bms.bms_config_info.spn2819_max_charge_voltage,
                   thiz->bms.bms_config_info.spn2820_max_temprature,
                   thiz->bms.bms_config_info.spn2821_soc,
                   thiz->bms.bms_config_info.spn2822_total_voltage);
        break;
    case PGN_BRO :// 0x000900, BMS 充电准备就绪报文
        gen = gen_search(thiz->bms.generator, thiz->bms.can_pack_gen_nr, PGN_BRO);
        if ( gen ) {
            gen->can_counter ++;
            gen->can_silence = 0;
        }
        log_printf(INF, "BMS is %s for charge.",
                   param->buff.rx_buff[0] == 0x00 ?
                    "not ready" :
                    param->buff.rx_buff[0] == 0xAA ?
                    "ready" : "<unkown status>");
        if ( param->buff.rx_buff[0] == 0x00 ) {
            bit_clr(thiz, F_BMS_READY);
            bit_clr(thiz, F_CHARGER_READY);
        } else if ( param->buff.rx_buff[0] == 0xAA ) {
            bit_set(thiz, F_BMS_READY);
            bit_set(thiz, F_CHARGER_READY);
        } else {
            log_printf(WRN, "BMS: wrong can package data.");
        }
        break;
    case PGN_BCL :// 0x001000, BMS 电池充电需求报文
        gen = gen_search(thiz->bms.generator, thiz->bms.can_pack_gen_nr, PGN_BCL);
        if ( gen ) {
            gen->can_counter ++;
            gen->can_silence = 0;
        }
        if ( bit_read(thiz, S_BMS_COMM_DOWN) ) {
            log_printf(INF, "BMS: BMS 通信"GRN("恢复"));
        }
        bit_clr(thiz, S_BMS_COMM_DOWN);

        memcpy(&thiz->bms.bms_charge_need_now,
               param->buff.rx_buff, sizeof(struct pgn4096_BCL));
        if ( thiz->bms.bms_charge_need_now.spn3072_need_voltage/10.0f > 750 ) {
            log_printf(WRN, "BMS: spn3072 range 0-750V gave: %d V",
                       thiz->bms.bms_charge_need_now.spn3072_need_voltage);
        }
        if ( thiz->bms.bms_charge_need_now.spn3073_need_current/10.0f > 400 ) {
            log_printf(WRN, "BMS: spn3073 range -400-0A gave: %d A",
                       thiz->bms.bms_charge_need_now.spn3073_need_current);
        }

        log_printf(INF, "BMS: PGN_BCL fetched, V-need: %.1f V, I-need: %d mode: %s",
                   thiz->bms.bms_charge_need_now.spn3072_need_voltage/10.0,
                   thiz->bms.bms_charge_need_now.spn3073_need_current,
                   thiz->bms.bms_charge_need_now.spn3074_charge_mode ==
                    CHARGE_WITH_CONST_VOLTAGE ? "恒压充电" :
                   thiz->bms.bms_charge_need_now.spn3074_charge_mode ==
                        CHARGE_WITH_CONST_CURRENT ? "恒流充电" : "无效模式");
        break;
    case PGN_BCS :// 0x001100, BMS 电池充电总状态报文
        gen = gen_search(thiz->bms.generator, thiz->bms.can_pack_gen_nr, PGN_BCS);
        if ( gen ) {
            gen->can_counter ++;
            gen->can_silence = 0;
        }
        log_printf(INF, "BMS: PGN_BCS fetched.");
        memcpy(&thiz->bms.bms_all_battery_status, param->buff.rx_buff,
               sizeof(struct pgn4352_BCS));
        if (thiz->bms.bms_all_battery_status.spn3075_charge_voltage/10.0 > 750.0f) {
            log_printf(WRN, "BMS: spn3075 range 0-750 gave: %.1f V",
                     thiz->bms.bms_all_battery_status.spn3075_charge_voltage/10.0);
        }
        if (thiz->bms.bms_all_battery_status.spn3076_charge_current/10.0 > 400.0f) {
            log_printf(WRN, "BMS: spn3076 range -400-0 gave: %.1f V",
                   -(thiz->bms.bms_all_battery_status.spn3076_charge_current/10.0));
        }
        if (thiz->bms.bms_all_battery_status.spn3077_max_v_g_number/100.0 > 24.0 ) {
            log_printf(WRN, "BMS: spn3077 range 0-24 gave: %.1f V",
                   -(thiz->bms.bms_all_battery_status.spn3077_max_v_g_number/100.0));
        }
        if (thiz->bms.bms_all_battery_status.spn3078_soc > 100 ) {
            log_printf(WRN, "BMS: spn3078 range 0%%-100%% gave: %d%%",
                   -(thiz->bms.bms_all_battery_status.spn3078_soc));
        }
        break;
    case PGN_BSM :// 0x001300, 动力蓄电池状态信息报文
        gen = gen_search(thiz->bms.generator, thiz->bms.can_pack_gen_nr, PGN_BSM);
        if ( gen ) {
            gen->can_counter ++;
            gen->can_silence = 0;
        }
        log_printf(INF, "BMS: PGN_BSM fetched.");
        memcpy(&thiz->bms.bms_battery_status, param->buff.rx_buff,
               sizeof(struct pgn4864_BSM));
        if ( SINGLE_BATTERY_VOLTAGE_HIGH ==
             (thiz->bms.bms_battery_status.remote_single&SINGLE_BATTERY_VOLTAGE_HIGH)){
        }
        if (SINGLE_BATTERY_VOLTAGE_LOW ==
             (thiz->bms.bms_battery_status.remote_single&SINGLE_BATTERY_VOLTAGE_LOW)){

        }

        if (BATTERY_CHARGE_CURRENT_HIGH ==
             (thiz->bms.bms_battery_status.remote_single&BATTERY_CHARGE_CURRENT_HIGH)){

        }
        if (BATTERY_CHARGE_CURRENT_LOW ==
             (thiz->bms.bms_battery_status.remote_single&BATTERY_CHARGE_CURRENT_LOW)){
        }

        if (BATTERY_TEMPRATURE_HIGH ==
                (thiz->bms.bms_battery_status.remote_single&BATTERY_TEMPRATURE_HIGH)){
        }
        if (BATTERY_TEMPRATURE_LOW ==
                (thiz->bms.bms_battery_status.remote_single&BATTERY_TEMPRATURE_LOW)) {
        }

        if (INSULATION_FAULT ==
                (thiz->bms.bms_battery_status.remote_single&INSULATION_FAULT)){
        }
        if (INSULATION_UNRELIABLE==
                (thiz->bms.bms_battery_status.remote_single&INSULATION_UNRELIABLE)){
        }

        if (CONNECTOR_STATUS_FAULT==
                (thiz->bms.bms_battery_status.remote_single&CONNECTOR_STATUS_FAULT)){
        }
        if (CONNECTOR_STATUS_UNRELIABLE==
                (thiz->bms.bms_battery_status.remote_single&CONNECTOR_STATUS_UNRELIABLE)){
        }

        if (CHARGE_ALLOWED==
                (thiz->bms.bms_battery_status.remote_single&CHARGE_ALLOWED)){
        }
        if (CHARGE_FORBIDEN==
                (thiz->bms.bms_battery_status.remote_single&CHARGE_FORBIDEN)){
        }
        break;
    case PGN_BMV :// 0x001500, 单体动力蓄电池电压报文
        gen = gen_search(thiz->bms.generator, thiz->bms.can_pack_gen_nr, PGN_BMV);
        if ( gen ) {
            gen->can_counter ++;
            gen->can_silence = 0;
        }
        log_printf(INF, "BMS: PGN_BMV fetched.");
        break;
    case PGN_BMT :// 0x001600, 单体动力蓄电池温度报文
        gen = gen_search(thiz->bms.generator, thiz->bms.can_pack_gen_nr, PGN_BMT);
        if ( gen ) {
            gen->can_counter ++;
            gen->can_silence = 0;
        }
        log_printf(INF, "BMS: PGN_BMT fetched.");
        break;
    case PGN_BSP :// 0x001700, 动力蓄电池预留报文
        gen = gen_search(thiz->bms.generator, thiz->bms.can_pack_gen_nr, PGN_BSP);
        if ( gen ) {
            gen->can_counter ++;
            gen->can_silence = 0;
        }

        log_printf(INF, "BMS: PGN_BSP fetched.");
        break;
    case PGN_BST :// 0x001900, BMS 中止充电报文
        gen = gen_search(thiz->bms.generator, thiz->bms.can_pack_gen_nr, PGN_BST);
        if ( gen ) {
            gen->can_counter ++;
            gen->can_silence = 0;
        }

        log_printf(INF, "BMS: PGN_BST fetched.");
        break;
    case PGN_BSD :// 0x001C00, BMS 统计数据报文
        gen = gen_search(thiz->bms.generator, thiz->bms.can_pack_gen_nr, PGN_BSD);
        if ( gen ) {
            gen->can_counter ++;
            gen->can_silence = 0;
        }
        if ( bit_read(thiz, S_BMS_COMM_DOWN) ) {
            log_printf(INF, "BMS: BMS 通信"GRN("恢复"));
        }
        bit_clr(thiz, S_BMS_COMM_DOWN);

        log_printf(INF, "BMS: PGN_BSD fetched.");
        break;
    case PGN_BEM :// 0x001E00, BMS 错误报文
        gen = gen_search(thiz->bms.generator, thiz->bms.can_pack_gen_nr, PGN_BEM);
        if ( gen ) {
            gen->can_counter ++;
            gen->can_silence = 0;
        }
        log_printf(INF, "BMS: PGN_BEM fetched.");
        break;
    default:
        log_printf(WRN, "BMS: un-recognized PGN %08X",
                   param->can_id);
        break;
    }
    return ERR_OK;
}

