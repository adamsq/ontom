#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <termios.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <sys/wait.h>
#include "config.h"
#include "log.h"
#include "error.h"
#include "Hachiko.h"
#include "blueprint.h"
#include "charge.h"

static int uart4_bp_evt_handle(struct bp_uart *self, BP_UART_EVENT evt,
                     struct bp_evt_param *param);
static int uart4_charger_evt_handle(struct bp_uart *self, BP_UART_EVENT evt,
                     struct bp_evt_param *param);
static int uart4_charger_config_evt_handle(struct bp_uart *self, BP_UART_EVENT evt,
                     struct bp_evt_param *param);
static int uart4_charger_module_evt_handle(struct bp_uart *self, BP_UART_EVENT evt,
                     struct bp_evt_param *param);
static int uart4_charger_date_evt_handle(struct bp_uart *self, BP_UART_EVENT evt,
                     struct bp_evt_param *param);
static int uart4_simple_box_evt_handle(struct bp_uart *self, BP_UART_EVENT evt,
                     struct bp_evt_param *param);
static int uart5_background_evt_handle(struct bp_uart *self, BP_UART_EVENT evt,
                     struct bp_evt_param *param);
// 共计两个串口
struct bp_uart uarts[2];
// 串口4 使用者为充电机和采样盒
struct bp_user down_user[] = {
    {101, 0, 5, 0, uart4_simple_box_evt_handle},     // 采样
    {110, 0, 5, 0, uart4_charger_evt_handle},        // 盒充电机运行寄存器，只读
    {120, 0, 5, 0, uart4_charger_config_evt_handle}, // 充电机参数寄存器(参数控制)，读写
    {130, 0, 5, 0, uart4_charger_module_evt_handle}, // 充电机参数寄存器(模块控制)，读写
    {140, 0, 5, 0, uart4_charger_date_evt_handle},   // 充电机参数寄存器(日期时间)，读写
    {0,  0, 0, 0, NULL}
};
// 串口5 使用者为上位机
struct bp_user up_user[] = {
    {100, 0, 5, 0, uart5_background_evt_handle},    // 采样盒
    {0,  0, 0, 0, NULL}
};

#define GPIO_TO_PIN(bank, gpio)	(32 * (bank) + (gpio))
#define	SERIAL4_CTRL_PIN	GPIO_TO_PIN(0, 19)
#define	SERIAL5_CTRL_PIN	GPIO_TO_PIN(0, 20)

#define	RX_LOW_LEVEL			0
#define	TX_HIGH_LEVEL			1

int configure_uart(int fd, int baud_rate, int databits, int stopbits, int parity)
{
    struct termios options;
    int   status;

    if (tcgetattr(fd, &options) != 0) {
        perror("SetupSerial 1");
        return ERR_UART_CONFIG_FAILE;
    }

    tcflush(fd, TCIOFLUSH);
    cfsetispeed(&options, baud_rate);
    cfsetospeed(&options, baud_rate);
    status = tcsetattr(fd, TCSANOW, &options);

    if (tcgetattr(fd, &options) != 0) {
        perror("SetupSerial 1");
        return ERR_UART_CONFIG_FAILE;
    }
    options.c_cflag &= ~CSIZE;

    switch (databits) {
        default:
        case 7:
            options.c_cflag |= CS7;
            break;
        case 8:
            options.c_cflag |= CS8;
            break;
    }

    switch (parity) {
        case 'n':
        case 'N':
            options.c_cflag &= ~PARENB;   /* Clear parity enable */
            options.c_iflag &= ~INPCK;     /* Enable parity checking */
            break;
        case 'o':
        case 'O':
        case 1:
            options.c_cflag |= (PARODD | PARENB);
            options.c_iflag |= INPCK;             /* Disnable parity checking */
            break;
        case 'e':
        case 'E':
        case 2:
            options.c_cflag |= PARENB;     /* Enable parity */
            options.c_cflag &= ~PARODD;
            options.c_iflag |= INPCK;      /* Disnable parity checking */
            break;
        default:
        case 'S':
        case 's':  /*as no parity*/
        case 0:
            options.c_cflag &= ~PARENB;
            options.c_cflag &= ~CSTOPB;
            break;
    }

    switch (stopbits) {
        default:
        case 1:
            options.c_cflag &= ~CSTOPB;
            break;
        case 2:
            options.c_cflag |= CSTOPB;
            break;
    }

    /* Set input parity option */
    if (parity != 'n')
        options.c_iflag |= INPCK;

    //options.c_cflag   |= CRTSCTS;
    options.c_iflag &=~(IXON | IXOFF | IXANY);
    options.c_iflag &=~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    //options.c_lflag |= ISIG;

    tcflush(fd,TCIFLUSH);
    options.c_oflag = 0;
    //options.c_lflag = 0;
    options.c_cc[VTIME] = 0; 						// delay 15 seconds
    options.c_cc[VMIN] = 0; 						// Update the options and do it NOW

    if (tcsetattr(fd,TCSANOW,&options) != 0) {
        perror("SetupSerial 3");
        return ERR_UART_CONFIG_FAILE;
    }

    return ERR_OK;
}

/********************************************************
Name:				gpio_export
Description:		导出所需要控制的GPIO的引脚编号
Input:
Return:
*********************************************************/
void gpio_export(int pin)
{
    // 导出所需要设置的GPIO口
    FILE *fp = fopen("/sys/class/gpio/export","w");
    rewind(fp);
    fprintf(fp, "%d\n", pin);
    fclose(fp);
    return;
}

/********************************************************
Name:				gpio_unexport
Description:		释放所需要控制的GPIO的引脚编号
Input:
Return:
*********************************************************/
void gpio_unexport(int pin)
{
    // 释放导出的GPIO口
    FILE *fp = fopen("/sys/class/gpio/unexport","w");
    rewind(fp);
    fprintf(fp, "%d\n", pin);
    fclose(fp);
    return;
}

/***********************************************************
Name:				set_gpio_output
Description:		设置gpio引脚为输出，并设定输出值
Input:
    1、pin:			gpio引脚号
    2、value:		0:低电平 1:高电平
Return:				0
************************************************************/
int set_gpio_output(int pin, int value)
{
    char file[40], direction[5];

    sprintf(file, "/sys/class/gpio/gpio%d/direction", pin);
    FILE *fp = fopen(file, "w");

    sprintf(direction,"out");

    rewind(fp);
    if (fprintf(fp, "%s",direction) < 0)
        return -2;

    fclose(fp);

    sprintf(file, "/sys/class/gpio/gpio%d/value", pin);
    fp = fopen(file, "w");
    rewind(fp);
    fprintf(fp, "%d\n", value);
    fclose(fp);

    return ERR_OK;
}

/*
 * 串口4作为互动设备是需要该信号的，因此在这里可以不用做判断是那个串口产生了该信号
 */
void uarts_async_sigio(int param)
{
    struct bp_uart * thiz = &uarts[0];
    struct Hachiko_food *p;
    p = & (thiz->tx_seed);

    log_printf(INF, "UART: "RED("SIGIO")" fetched. %d", param);
    if ( BP_UART_STAT_WR == thiz->hw_status && thiz->tx_param.payload_size ) {
        thiz->bp_evt_handle(thiz, BP_EVT_TX_FRAME_DONE, &thiz->tx_param);
        thiz->tx_param.payload_size = 0;
        Hachiko_pause(p);
        memset(thiz->rx_param.buff.rx_buff, 0, thiz->rx_param.buff_size);
        thiz->status = BP_UART_STAT_RD;
        if ( thiz->role == BP_UART_MASTER ) {
            // 主动设备，需要进行接收超时判定
            Hachiko_resume(&thiz->rx_seed);
        }
    } else{
        log_printf(DBG_LV1, "UART: hw_status: %X, payload: %d",
                   thiz->hw_status, thiz->tx_param.payload_size);
    }
}

// 串口4的超时响应
void uart4_Hachiko_notify_proc(Hachiko_EVT evt, void *private,
                            const struct Hachiko_food *self)
{
    struct bp_uart * thiz = (struct bp_uart * __restrict__)private;
    struct Hachiko_food *p;

    if ( evt != HACHIKO_TIMEOUT ) return;
    p = & (thiz->rx_seed);

    if ( self == p ) {
        log_printf(WRN, "UART: rx packet TIME-OUT.need: %d, fetched: %d",
                   thiz->rx_param.need_bytes,
                    thiz->rx_param.payload_size);
        if ( thiz->rx_param.payload_size == 0 ) {
            thiz->bp_evt_handle(thiz, BP_EVT_RX_BYTE_TIMEOUT, &thiz->rx_param);
        } else {
            thiz->bp_evt_handle(thiz, BP_EVT_RX_FRAME_TIMEOUT, &thiz->rx_param);
        }
        Hachiko_pause(&thiz->rx_seed);
        thiz->status = BP_UART_STAT_WR;
        return;
    }

#if 0
    /*
     * 串口发送完成事件由系统提供SIGIO信号来确定，具体逻辑见函数
     *  void uarts_async_sigio(int param)
     */
#else
    p = & thiz->tx_seed;
    if ( self == p ) {
        log_printf(DBG_LV1, "UART: packet send done.");
        thiz->tx_param.payload_size = 0;
        Hachiko_pause(p);
        memset(thiz->rx_param.buff.rx_buff, 0, thiz->rx_param.buff_size);
        thiz->status = BP_UART_STAT_RD;
        if ( thiz->role == BP_UART_MASTER ) {
            // 主动设备，需要进行接收超时判定
            Hachiko_resume(&thiz->rx_seed);
        }
        return;
    }
#endif
}

/*
 * 串口事件响应函数
 */
static int uart4_bp_evt_handle(struct bp_uart *self, BP_UART_EVENT evt,
                     struct bp_evt_param *param)
{
    int ret = ERR_OK;
    int i;
    struct bp_user *u;

    switch ( evt ) {
    // 串口数据结构初始化
    case BP_EVT_INIT:
        self->role = BP_UART_MASTER;
        self->rx_seed.private = (void*)self;
        self->rx_seed.Hachiko_notify_proc = uart4_Hachiko_notify_proc;
        self->rx_param.buff.rx_buff = self->rx_buff;
        self->rx_param.cursor = 0;
        self->rx_param.payload_size = 0;
        self->rx_param.buff_size = sizeof(self->rx_buff);

        self->users = down_user;
        self->master = &self->users[0];

        ret = _Hachiko_new(&self->rx_seed, HACHIKO_AUTO_FEED,
                     160, HACHIKO_PAUSE, (void*)self);
        if ( ret != ERR_OK ) {
            log_printf(ERR, "UART: create uart reciever's timer faile.");
        }
        self->tx_seed.private = (void*)self;
        self->tx_seed.Hachiko_notify_proc = uart4_Hachiko_notify_proc;
        self->tx_param.payload_size = 0;
        self->tx_param.cursor = 0;
        self->tx_param.buff.tx_buff = self->tx_buff;
        self->tx_param.buff_size = sizeof(self->tx_buff);
        ret = _Hachiko_new(&self->tx_seed, HACHIKO_AUTO_FEED,
                     2, HACHIKO_PAUSE, (void*)self);
        if ( ret != ERR_OK ) {
            log_printf(ERR, "UART: create uart transfer's timer faile.");
        }
        break;
    // 串口配置
    case BP_EVT_CONFIGURE:
        gpio_export(SERIAL4_CTRL_PIN);
        self->dev_handle = open(self->dev_name,
                                O_RDWR | O_NOCTTY | O_NONBLOCK | O_NDELAY);
        if ( self->dev_handle == -1 ) {
            return ERR_UART_OPEN_FAILE;
        } else {
            log_printf(DBG, "UART: self->dev_handle: %d", self->bp_evt_handle);
        }
        if ( -1 == fcntl(self->dev_handle, F_SETFL, FASYNC) ) {
            log_printf(ERR,
                    "UART: set uart to async mode failed!!! errno: %d", errno);
        }

        ret = configure_uart(self->dev_handle, B9600, 8, 1, 'N');
        if ( ret == (int)ERR_UART_CONFIG_FAILE ) {
            log_printf(ERR, "UART: configure uart faile.");
            return ERR_UART_CONFIG_FAILE;
        }
        self->status = BP_UART_STAT_WR;
        self->hw_status = BP_UART_STAT_INVALID;
        break;
    // 关闭串口
    case BP_EVT_KILLED:
        gpio_unexport(SERIAL4_CTRL_PIN);
        close(self->dev_handle);
        break;
    // 串口数据帧校验
    case BP_EVT_FRAME_CHECK:
        ret = ERR_ERR;
        if ( self->master && self->master->user_evt_handle ) {
            ret = self->master->user_evt_handle(self, BP_EVT_FRAME_CHECK, param);
        }
        break;
    // 切换到发送模式
    case BP_EVT_SWITCH_2_TX:
        ret = set_gpio_output(SERIAL4_CTRL_PIN, TX_HIGH_LEVEL);
        self->master->seed = 0;
        if ( ret != ERR_OK ) {
            log_printf(ERR, "UART: set uart to TX mode faile");
            break;
        }
        break;
    // 切换到接收模式
    case BP_EVT_SWITCH_2_RX:
        ret = set_gpio_output(SERIAL4_CTRL_PIN, RX_LOW_LEVEL);
        self->master->seed = 0;
        if ( ret != ERR_OK ) {
            log_printf(ERR, "UART: set uart to RX mode faile");
            break;
        }
        break;

    // 串口接收到新数据
    case BP_EVT_RX_DATA:
        if ( self->master && self->master->user_evt_handle ) {
            ret = self->master->user_evt_handle(self, BP_EVT_RX_DATA, param);
        }
        break;
    // 串口收到完整的数据帧
    case BP_EVT_RX_FRAME:
        /*
         * 数据帧格式兼容性问题：
         * 目前可能出现的数据帧格式为：
         *   格式1： 魔数 + 地址 + 长度 + 数据 + 校验   或者
         *   格式2： 地址 + 长度 + 数据 + 校验
         * 因此数据帧识别问题被归结为数据帧长度的识别，格式1中为了降低数据与控制字节之间
         * 的重叠问题，所以采用了魔数方式填充数据帧头，魔数长度不定，因此需要在数据帧定义
         * 的数据结构中加上帧头的魔数长度描述
         */
        if ( self->master && self->master->user_evt_handle ) {
            ret = self->master->user_evt_handle(self, BP_EVT_RX_FRAME, param);
        }
        break;

    // 串口发送数据请求
    case BP_EVT_TX_FRAME_REQUEST:
        log_printf(DBG_LV0, "UART: get into fucking here. %d",
                   param->payload_size);
        if ( param->payload_size ) return ERR_ERR;
#if 1

        for ( u = self->users; u->user_evt_handle; u ++ ) {
            if ( u->seed <= u->frame_freq ) {
                u->seed ++;
            }
        }

        for ( u = self->users; u->user_evt_handle; u ++ ) {
            if ( u->seed > u->frame_freq && self->master != u ) {
                self->master = u;
                /*
                 * 需要在私有事件处理过程中进行事件处理，数据填充
                 * 数据填充格式按照下面的步骤进行：
                 * param->attrib = BP_FRAME_UNSTABLE;
                 *    ......
                 * memcpy(param->buff.tx_buff, &qry, sizeof(qry));
                 * param->payload_size = sizeof(qry);
                 */
                ret = u->user_evt_handle(self, BP_EVT_TX_FRAME_REQUEST, param);
                log_printf(INF, "UART: ret: %d, load: %d",
                           ret, param->payload_size);
                break;
            }
        }
#else
        param->attrib = BP_FRAME_UNSTABLE;
        struct MDATA_QRY qry;
        qry.magic[0] = 0xF0;
        qry.magic[1] = 0xE1;
        qry.magic[2] = 0xD2;
        qry.magic[3] = 0xC3;
        qry.magic[4] = 0xB4;
        qry.addr = 0x05;
        qry.len = 16;
        qry.crc = 0xFFFF;
        memcpy(param->buff.tx_buff, &qry, sizeof(qry));
        param->payload_size = sizeof(qry);
#endif
        break;
    // 串口发送确认
    case BP_EVT_TX_FRAME_CONFIRM:
        if ( self->master && self->master->user_evt_handle ) {
            ret = self->master->user_evt_handle(self, BP_EVT_TX_FRAME_CONFIRM, param);
        } else {
            log_printf(WRN, "UART: "RED("BP_EVT_TX_FRAME_CONFIRM")" without signal procedure.");
        }
        break;
    // 串口数据发送完成事件
    case BP_EVT_TX_FRAME_DONE:
        if ( self->master ) {
            self->master->seed = 0;
        }
        if ( self->master && self->master->user_evt_handle ) {
            ret = self->master->user_evt_handle(self, BP_EVT_TX_FRAME_DONE, param);
        } else {
            log_printf(WRN, "UART: "RED("BP_EVT_TX_FRAME_DONE")" without signal procedure.");
        }
        break;

    // 串口接收单个字节超时，出现在接收帧的第一个字节
    case BP_EVT_RX_BYTE_TIMEOUT:
        log_printf(DBG_LV1, "UART: no data fetched.");
        if ( self->master && self->master->user_evt_handle ) {
            ret = self->master->user_evt_handle(self, BP_EVT_RX_BYTE_TIMEOUT, param);
        } else {
            log_printf(WRN, "UART: "RED("BP_EVT_RX_BYTE_TIMEOUT")" without signal procedure.");
        }
        break;
    // 串口接收帧超时, 接受的数据不完整
    case BP_EVT_RX_FRAME_TIMEOUT:
        log_printf(DBG_LV1, "UART: not all data fetched yet.");
        if ( self->master && self->master->user_evt_handle ) {
            ret = self->master->user_evt_handle(self, BP_EVT_RX_FRAME_TIMEOUT, param);
        } else {
            log_printf(WRN, "UART: "RED("BP_EVT_RX_FRAME_TIMEOUT")" without signal procedure.");
        }
        break;

    // 串口IO错误
    case BP_EVT_IO_ERROR:
        break;
    // 帧校验失败
    case BP_EVT_FRAME_CHECK_ERROR:
        break;
    }
    return ret;
}

// 只读数据段
static int uart4_charger_evt_handle(struct bp_uart *self, BP_UART_EVENT evt,
                     struct bp_evt_param *param)
{
    int ret = ERR_ERR;
    char buff[8];

    switch (evt) {
    case BP_EVT_FRAME_CHECK:
        ret = ERR_ERR;
        break;
    // 串口接收到新数据
    case BP_EVT_RX_DATA:
        break;
    // 串口收到完整的数据帧
    case BP_EVT_RX_FRAME:
        break;
    // 串口发送数据请求
    case BP_EVT_TX_FRAME_REQUEST:
        param->attrib = BP_FRAME_UNSTABLE;
        buff[0] = 0x01;
        buff[1] = 0x04;
        buff[2] = buff[3] = 0x00;
        buff[4] = 0x00;
        buff[5] = 0x64;
        buff[6] = 0xF1;
        buff[7] = 0xE1;
        memcpy(param->buff.tx_buff, buff, sizeof(buff));

        self->rx_param.need_bytes = 16;

        param->payload_size = sizeof(buff);
        ret = ERR_OK;
        log_printf(INF, "UART: %s sent", __FUNCTION__);
        break;
    // 串口发送确认
    case BP_EVT_TX_FRAME_CONFIRM:
        ret = ERR_OK;
        break;
    // 串口数据发送完成事件
    case BP_EVT_TX_FRAME_DONE:
        log_printf(INF, "UART: %s packet send done", __FUNCTION__);
        break;
    // 串口接收单个字节超时，出现在接收帧的第一个字节
    case BP_EVT_RX_BYTE_TIMEOUT:
    // 串口接收帧超时, 接受的数据不完整
    case BP_EVT_RX_FRAME_TIMEOUT:
        log_printf(WRN, "UART: %s get signal TIMEOUT", __FUNCTION__);
        break;
    // 串口IO错误
    case BP_EVT_IO_ERROR:
        break;
    // 帧校验失败
    case BP_EVT_FRAME_CHECK_ERROR:
        break;
    default:
        log_printf(WRN, "UART: unreliable EVENT %08Xh", evt);
        break;
    }
    return ret;
}

// 配置数据
static int uart4_charger_config_evt_handle(struct bp_uart *self, BP_UART_EVENT evt,
                     struct bp_evt_param *param)
{
    int ret = ERR_ERR;
    char buff[8];

    switch (evt) {
    case BP_EVT_FRAME_CHECK:
        ret = ERR_ERR;
        break;
    // 串口接收到新数据
    case BP_EVT_RX_DATA:
        break;
    // 串口收到完整的数据帧
    case BP_EVT_RX_FRAME:
        break;
    // 串口发送数据请求
    case BP_EVT_TX_FRAME_REQUEST:
        param->attrib = BP_FRAME_UNSTABLE;
        buff[0] = 0x01;
        buff[1] = 0x04;
        buff[2] = buff[3] = 0x00;
        buff[4] = 0x00;
        buff[5] = 0x06;
        buff[6] = 0x70;
        buff[7] = 0x08;
        memcpy(param->buff.tx_buff, buff, sizeof(buff));
        param->payload_size = sizeof(buff);
        ret = ERR_OK;

        self->rx_param.need_bytes = 16;

        log_printf(INF, "UART: %s sent", __FUNCTION__);
        break;
    // 串口发送确认
    case BP_EVT_TX_FRAME_CONFIRM:
        ret = ERR_OK;
        break;
    // 串口数据发送完成事件
    case BP_EVT_TX_FRAME_DONE:
        log_printf(INF, "UART: %s packet send done", __FUNCTION__);
        break;
    // 串口接收单个字节超时，出现在接收帧的第一个字节
    case BP_EVT_RX_BYTE_TIMEOUT:
    // 串口接收帧超时, 接受的数据不完整
    case BP_EVT_RX_FRAME_TIMEOUT:
        log_printf(WRN, "UART: %s get signal TIMEOUT", __FUNCTION__);
        break;
    // 串口IO错误
    case BP_EVT_IO_ERROR:
        break;
    // 帧校验失败
    case BP_EVT_FRAME_CHECK_ERROR:
        break;
    default:
        log_printf(WRN, "UART: unreliable EVENT %08Xh", evt);
        break;
    }
    return ret;
}

// 模块操作
static int uart4_charger_module_evt_handle(struct bp_uart *self, BP_UART_EVENT evt,
                     struct bp_evt_param *param)
{
    int ret = ERR_ERR;
    char buff[8];

    switch (evt) {
    case BP_EVT_FRAME_CHECK:
        ret = ERR_ERR;
        break;
    // 串口接收到新数据
    case BP_EVT_RX_DATA:
        break;
    // 串口收到完整的数据帧
    case BP_EVT_RX_FRAME:
        break;
    // 串口发送数据请求
    case BP_EVT_TX_FRAME_REQUEST:
        param->attrib = BP_FRAME_UNSTABLE;
        buff[0] = 0x01;
        buff[1] = 0x04;
        buff[2] = buff[3] = 0x00;
        buff[4] = 0x00;
        buff[5] = 0x06;
        buff[6] = 0x70;
        buff[7] = 0x08;

        self->rx_param.need_bytes = 16;

        memcpy(param->buff.tx_buff, buff, sizeof(buff));
        param->payload_size = sizeof(buff);
        ret = ERR_OK;
        log_printf(INF, "UART: %s sent", __FUNCTION__);
        break;
    // 串口发送确认
    case BP_EVT_TX_FRAME_CONFIRM:
        ret = ERR_OK;
        break;
    // 串口数据发送完成事件
    case BP_EVT_TX_FRAME_DONE:
        log_printf(INF, "UART: %s packet send done", __FUNCTION__);
        break;
    // 串口接收单个字节超时，出现在接收帧的第一个字节
    case BP_EVT_RX_BYTE_TIMEOUT:
    // 串口接收帧超时, 接受的数据不完整
    case BP_EVT_RX_FRAME_TIMEOUT:
        log_printf(WRN, "UART: %s get signal TIMEOUT", __FUNCTION__);
        break;
    // 串口IO错误
    case BP_EVT_IO_ERROR:
        break;
    // 帧校验失败
    case BP_EVT_FRAME_CHECK_ERROR:
        break;
    default:
        log_printf(WRN, "UART: unreliable EVENT %08Xh", evt);
        break;
    }
    return ret;
}

// 系统校时
static int uart4_charger_date_evt_handle(struct bp_uart *self, BP_UART_EVENT evt,
                     struct bp_evt_param *param)
{
    int ret = ERR_ERR;
    char buff[8];

    switch (evt) {
    case BP_EVT_FRAME_CHECK:
        ret = ERR_ERR;
        break;
    // 串口接收到新数据
    case BP_EVT_RX_DATA:
        break;
    // 串口收到完整的数据帧
    case BP_EVT_RX_FRAME:
        break;
    // 串口发送数据请求
    case BP_EVT_TX_FRAME_REQUEST:
        param->attrib = BP_FRAME_UNSTABLE;
        buff[0] = 0x01;
        buff[1] = 0x04;
        buff[2] = buff[3] = 0x00;
        buff[4] = 0x00;
        buff[5] = 0x06;
        buff[6] = 0x70;
        buff[7] = 0x08;

        self->rx_param.need_bytes = 16;

        memcpy(param->buff.tx_buff, buff, sizeof(buff));
        param->payload_size = sizeof(buff);
        ret = ERR_OK;
        log_printf(INF, "UART: %s sent", __FUNCTION__);
        break;
    // 串口发送确认
    case BP_EVT_TX_FRAME_CONFIRM:
        ret = ERR_OK;
        break;
    // 串口数据发送完成事件
    case BP_EVT_TX_FRAME_DONE:
        log_printf(INF, "UART: %s packet send done", __FUNCTION__);
        break;
    // 串口接收单个字节超时，出现在接收帧的第一个字节
    case BP_EVT_RX_BYTE_TIMEOUT:
    // 串口接收帧超时, 接受的数据不完整
    case BP_EVT_RX_FRAME_TIMEOUT:
        log_printf(WRN, "UART: %s get signal TIMEOUT", __FUNCTION__);
        break;
    // 串口IO错误
    case BP_EVT_IO_ERROR:
        break;
    // 帧校验失败
    case BP_EVT_FRAME_CHECK_ERROR:
        break;
    default:
        log_printf(WRN, "UART: unreliable EVENT %08Xh", evt);
        break;
    }
    return ret;
}

static int uart4_simple_box_evt_handle(struct bp_uart *self, BP_UART_EVENT evt,
                     struct bp_evt_param *param)
{
    int ret = ERR_ERR;
    struct MDATA_QRY qry;

    switch (evt) {
    case BP_EVT_FRAME_CHECK:
        ret = ERR_ERR;
        break;
    // 串口接收到新数据
    case BP_EVT_RX_DATA:
        break;
    // 串口收到完整的数据帧
    case BP_EVT_RX_FRAME:
        break;
    // 串口发送数据请求
    case BP_EVT_TX_FRAME_REQUEST:
        param->attrib = BP_FRAME_UNSTABLE;
        qry.magic[0] = 0xF0;
        qry.magic[1] = 0xE1;
        qry.magic[2] = 0xD2;
        qry.magic[3] = 0xC3;
        qry.magic[4] = 0xB4;
        qry.addr = 0x05;
        qry.len = 16;
        qry.crc = 0xFFFF;
        memcpy(param->buff.tx_buff, &qry, sizeof(qry));
        param->payload_size = sizeof(qry);
        ret = ERR_OK;
        log_printf(INF, "UART: %s sent", __FUNCTION__);
        break;
    // 串口发送确认
    case BP_EVT_TX_FRAME_CONFIRM:
        ret = ERR_OK;
        break;
    // 串口数据发送完成事件
    case BP_EVT_TX_FRAME_DONE:
        log_printf(INF, "UART: %s packet send done", __FUNCTION__);
        break;
    // 串口接收单个字节超时，出现在接收帧的第一个字节
    case BP_EVT_RX_BYTE_TIMEOUT:
    // 串口接收帧超时, 接受的数据不完整
    case BP_EVT_RX_FRAME_TIMEOUT:
        log_printf(WRN, "UART: %s get signal TIMEOUT", __FUNCTION__);
        break;
    // 串口IO错误
    case BP_EVT_IO_ERROR:
        break;
    // 帧校验失败
    case BP_EVT_FRAME_CHECK_ERROR:
        break;
    default:
        log_printf(WRN, "UART: unreliable EVENT %08Xh", evt);
        break;
    }
    return ret;
}

static int uart5_background_evt_handle(struct bp_uart *self, BP_UART_EVENT evt,
                     struct bp_evt_param *param)
{
    int ret = ERR_ERR;
    switch (evt) {
    case BP_EVT_FRAME_CHECK:
        break;
    // 串口接收到新数据
    case BP_EVT_RX_DATA:
        break;
    // 串口收到完整的数据帧
    case BP_EVT_RX_FRAME:
        break;
    // 串口发送数据请求
    case BP_EVT_TX_FRAME_REQUEST:
        break;
    // 串口发送确认
    case BP_EVT_TX_FRAME_CONFIRM:
        break;
    // 串口数据发送完成事件
    case BP_EVT_TX_FRAME_DONE:
        break;
    // 串口接收单个字节超时，出现在接收帧的第一个字节
    case BP_EVT_RX_BYTE_TIMEOUT:
    // 串口接收帧超时, 接受的数据不完整
    case BP_EVT_RX_FRAME_TIMEOUT:
        log_printf(WRN, "UART: %s get signal TIMEOUT", __FUNCTION__);
        break;
    // 串口IO错误
    case BP_EVT_IO_ERROR:
        break;
    // 帧校验失败
    case BP_EVT_FRAME_CHECK_ERROR:
        break;
    default:
        log_printf(WRN, "UART: unreliable EVENT %08Xh", evt);
        break;
    }
    return ret;
}

void *thread_uart_service(void *arg) ___THREAD_ENTRY___
{
    int *done = (int *)arg;
    int mydone = 0, ret;
    struct bp_uart *thiz = &uarts[0];
    int retval, max_handle = 0;
    size_t cursor;

    if ( done == NULL ) done = &mydone;

    memset(uarts, 0, sizeof(uarts));

    uarts[0].bp_evt_handle = uart4_bp_evt_handle;
    uarts[0].dev_handle = -1;
    uarts[0].dev_name = "/dev/ttyO4";

    uarts[1].bp_evt_handle = NULL;
    uarts[1].dev_handle = -1;
    uarts[1].dev_name = "/dev/ttyO5";

    if ( thiz ) {
        // 出错误后尝试的次数
        thiz->init_magic = 5;
    }
    if ( SIG_ERR == signal(SIGIO, uarts_async_sigio) ) {
        log_printf(ERR, "UART: signal(SIGIO, uarts_async_sigio) failed!!!"
                   "errno: %d", errno);
    }

    while ( ! *done ) {
        usleep(3000);
        if ( thiz == NULL ) continue;
        if ( thiz->bp_evt_handle == NULL ) continue;
        if ( thiz->status == BP_UART_STAT_ALIENT ) continue;

        //signal(SIGIO, uarts_async_sigio);

        if ( thiz->status == BP_UART_STAT_INVALID ) {
            if ( thiz->init_magic <= 0 ) {
                thiz->status = BP_UART_STAT_ALIENT;
                log_printf(ERR, "UART: open UART faile, thread panic.....");
                continue;
            }

            // 初始化数据结构, 设定串口的初始状态
            // 串口的初始状态决定了串口的工作模式
            thiz->bp_evt_handle(thiz, BP_EVT_INIT, NULL);

            // 打开并配置串口
            // 如果初始化失败，则会不断的尝试
            ret = thiz->bp_evt_handle(thiz, BP_EVT_CONFIGURE, NULL);
            if ( ret == (int)ERR_UART_OPEN_FAILE ) {
                thiz->status = BP_UART_STAT_INVALID;
                log_printf(ERR, "UART: try open %s faile.", thiz->dev_name);
                thiz->init_magic --;
                continue;
            }
            if ( ret == (int)ERR_UART_CONFIG_FAILE ) {
                thiz->status = BP_UART_STAT_INVALID;
                // 首先关闭串口，然后才能进行下一次尝试
                thiz->bp_evt_handle(thiz, BP_EVT_KILLED, NULL);
                log_printf(ERR, "UART: configure %s faile.", thiz->dev_name);
                thiz->init_magic --;
                continue;
            }

            if ( thiz->status != BP_UART_STAT_RD &&
                 thiz->status != BP_UART_STAT_WR ) {
                // 默认采用被动方式
                thiz->status = BP_UART_STAT_RD;
            }

            ret = ERR_ERR;
            if ( thiz->status == BP_UART_STAT_RD ) {
                int trynr = 0;
                do {
                    usleep(50000);
                    ret = thiz->bp_evt_handle(thiz, BP_EVT_SWITCH_2_RX, NULL);
                } while ( ret != ERR_OK && trynr ++ < 100 );
            }
            if ( thiz->status == BP_UART_STAT_WR ) {
                int trynr = 0;
                do {
                    usleep(50000);
                    ret = thiz->bp_evt_handle(thiz, BP_EVT_SWITCH_2_TX, NULL);
                } while ( ret != ERR_OK && trynr ++ < 100  );
            }
            if ( ret != ERR_OK ) {
                thiz->bp_evt_handle(thiz, BP_EVT_KILLED, NULL);
                thiz->status = BP_UART_STAT_ALIENT;
                log_printf(ERR, "UART: switch to defaute mode faile, thread panic..");
                continue;
            }

            if ( thiz->dev_handle > max_handle ) {
                max_handle = thiz->dev_handle;
            }

            log_printf(INF, "UART: open UART %d:%s correct.",
                       thiz->dev_handle, thiz->dev_name);
            continue;
        }

        /*
         * 串口在这里扮演的角色必定是主动设备或者从动设备
         * 因此读取数据是否需要进行计时操作完全取决于扮演的角色类型。
         *
         * 如果是主动设备，那么必须要设定超时计时器，以确定接收数据是否超时
         * 如果是从动设备，设定超时计时器并非是必须的，而是根据是否需要接收
         * 连续的串口数据流来判定，若不需要连续的数据流，那么就不必要设定
         * 超时计时器，反之需要设定超时计时器来确定能否在指定时间内收到完整
         * 的数据帧。
         */
        if ( thiz->status == BP_UART_STAT_RD ) {
            char *buff = thiz->rx_param.buff.rx_buff;
            int rd = 0;
            static int nr = 0;

            if ( thiz->hw_status != BP_UART_STAT_RD ) {
                errno = 0;
                thiz->bp_evt_handle(thiz, BP_EVT_SWITCH_2_RX, NULL);
                thiz->hw_status = BP_UART_STAT_RD;
                thiz->rx_param.cursor = 0;
                thiz->rx_param.payload_size = 0;
                nr = 0;
                memset(thiz->rx_buff, 0, sizeof(thiz->rx_buff));
                tcflush(thiz->dev_handle, TCIFLUSH);
                log_printf(DBG_LV0, "UART: switch to RX mode %d.", errno);
            }

            errno = 0;
            cursor = thiz->rx_param.cursor;
            do {
                rd = read(thiz->dev_handle,
                          &thiz->rx_param.buff.rx_buff[cursor], 256);
                if ( rd > 0 ) {
                    Hachiko_feed(&thiz->rx_seed);
                    thiz->rx_param.payload_size += rd;
                    thiz->rx_param.cursor = thiz->rx_param.payload_size;
                    nr += rd;
                    log_printf(DBG_LV1,
                               "UART: RD:%d:%d:%d <%02X %02X %02X %02X %02X %02X %02X"
                               " %02X %02X %02X %02X %02X %02X %02X %02X>",
                               rd, nr, cursor,
                               buff[0], buff[1], buff[2], buff[3],
                               buff[4], buff[5], buff[6], buff[7],
                               buff[0+8], buff[1+8], buff[2+8], buff[3+8],
                               buff[4+8], buff[5+8], buff[6+8], buff[7+8]);
                }
                ret = thiz->bp_evt_handle(thiz, BP_EVT_FRAME_CHECK,
                                          &thiz->rx_param);
                switch ( ret ) {
                // 数据接收，校验完成, 完成数据接收过程，停止接收
                case ERR_OK:
                    log_printf(DBG_LV2, "UART: fetched a new frame.");
                    break;
                // 数据接收完成，但校验失败, 停止接收
                case ERR_FRAME_CHECK_ERR:
                    log_printf(DBG_LV2,
                               "UART: lenth fetched but check "RED("faile."));
                    break;
                // 数据接收长度不足，继续接收
                case ERR_FRAME_CHECK_DATA_TOO_SHORT:
                    break;
                default:
                    break;
                }

                if ( thiz->rx_param.payload_size >=
                     (size_t)(thiz->rx_param.buff.rx_buff[1] + 4) ) {
                    thiz->status = BP_UART_STAT_WR;
                    Hachiko_pause(&thiz->rx_seed);
                    log_printf(DBG_LV1, "UART: recv done.need: %d, fetched: %d",
                               thiz->rx_param.buff.rx_buff[1]+4,
                            thiz->rx_param.payload_size);
                }
            } while (0);
            continue;
        }

        // 程序默认采用9600 的波特率， 大致估算出每发送一个字节耗时1.04ms
        // 抛去程序运行时的延迟，发送延迟，可估计每发送一个字节耗时1.1 ms
        if ( thiz->status == BP_UART_STAT_WR ) {

            if ( thiz->hw_status != BP_UART_STAT_WR ) {
                tcflush(thiz->dev_handle, TCOFLUSH);
                thiz->bp_evt_handle(thiz, BP_EVT_SWITCH_2_TX, NULL);
                thiz->hw_status = BP_UART_STAT_WR;
                memset(thiz->tx_buff, 0, sizeof(thiz->tx_buff));
                log_printf(DBG_LV1, "UART: switch to TX mode.");
                continue;
            }

            if ( thiz->tx_param.cursor < thiz->tx_param.payload_size &&
                 thiz->tx_param.payload_size > 0 ) {
                // 前一次没有发送完成， 继续发送
                log_printf(DBG_LV0, "UART: goto continue_to_send");
                goto continue_to_send;
            }
            if ( thiz->tx_param.payload_size ) {
                log_printf(DBG_LV0, "UART: continue becouse: thiz->tx_param.payload_size > 0");
                continue;
            }

            tcflush(thiz->dev_handle, TCOFLUSH);

            thiz->tx_param.buff.tx_buff = thiz->tx_buff;
            thiz->tx_param.buff_size = sizeof(thiz->tx_buff);
            thiz->tx_param.payload_size = 0;
            thiz->tx_param.cursor = 0;
            ret = thiz->bp_evt_handle(thiz, BP_EVT_TX_FRAME_REQUEST,
                                      &thiz->tx_param);
            if ( ret != ERR_OK || thiz->tx_param.payload_size <= 0 ||
                 thiz->tx_param.payload_size > 255 ) {
                thiz->tx_param.buff.tx_buff = thiz->tx_buff;
                thiz->tx_param.buff_size = sizeof(thiz->tx_buff);
                thiz->tx_param.payload_size = 0;
                thiz->tx_param.cursor = 0;
                continue;
            }

            ret = thiz->bp_evt_handle(thiz, BP_EVT_TX_FRAME_CONFIRM,
                                      &thiz->tx_param);
            if ( ret != ERR_OK ) {
                thiz->tx_param.buff.tx_buff = thiz->tx_buff;
                thiz->tx_param.buff_size = sizeof(thiz->tx_buff);
                thiz->tx_param.payload_size = 0;
                thiz->tx_param.cursor = 0;
                continue;
            }

            if ( thiz->tx_param.cursor > thiz->tx_param.payload_size ) {
                thiz->tx_param.buff.tx_buff = thiz->tx_buff;
                thiz->tx_param.buff_size = sizeof(thiz->tx_buff);
                thiz->tx_param.payload_size = 0;
                thiz->tx_param.cursor = 0;
                continue;
            }

continue_to_send:
            cursor = thiz->tx_param.cursor;
            retval = write(thiz->dev_handle,
                           & thiz->tx_param.buff.tx_buff[cursor],
                           thiz->tx_param.payload_size - cursor);
            if ( retval <= 0 ) {
                log_printf(ERR, "UART: send error, TX REQUEST AUTOMATIC ABORTED.");
                thiz->tx_param.buff.tx_buff = thiz->tx_buff;
                thiz->tx_param.buff_size = sizeof(thiz->tx_buff);
                thiz->tx_param.payload_size = 0;
                thiz->tx_param.cursor = 0;
                continue;
            }

            if ( retval == (int)(thiz->tx_param.payload_size - cursor) ) {
                // 发送完成，但仅仅是数据写入到发送缓冲区，此时数据没有完全通过传输介质
                // 此时启动发送计时器，用来确定数据发送完成事件
                thiz->tx_param.cursor = thiz->tx_param.payload_size;
                thiz->tx_seed.ttl = thiz->tx_param.payload_size / 20 +
                        (thiz->tx_param.payload_size % 10 ? 1 : 0);
                log_printf(DBG_LV1, "UART: send data len: %d, TX ttl: %d unit",
                           thiz->tx_param.payload_size,
                           thiz->tx_seed.ttl);
                Hachiko_resume( & thiz->tx_seed );
            } else if ( retval < (int)(thiz->tx_param.payload_size - cursor) ) {
                // 发送了一部分
                thiz->tx_param.cursor = retval;
            } else {
                // Unexpected. Exception
                log_printf(ERR, "UART: send error, TX AUTO ABORTED.");
                thiz->tx_param.buff.tx_buff = thiz->tx_buff;
                thiz->tx_param.buff_size = sizeof(thiz->tx_buff);
                thiz->tx_param.payload_size = 0;
                thiz->tx_param.cursor = 0;
                continue;
            }
            continue;
        }
    }
    return NULL;
}
