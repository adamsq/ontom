#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct user_card {
    // ��������ʱ�Ŀ���
    char triger_card_sn[64];
    struct {
        unsigned char id[16];
        unsigned char sector_num;
        union {
            unsigned char buff[16];
            struct {
                unsigned int magic;           /*����ħ�� 'ONPW', Only-Power 0x4F4E5057 */
                unsigned char remain_money[3]; /*�� ��Χ 0.00 ~ 167772.15 */
                unsigned char remain_sum;    /*���У��λ */
                unsigned char passwd_code[3];  /*6 λBCD ������*/
                unsigned char passwd_sum;    /*����У��� */
                unsigned short reserved;
                unsigned char unuesed;
                unsigned char sum;            /*У��� */
            }data;
        }sector_4;
    }card;
};

#define SECT   4

// ����IO�¼�
typedef enum {
    // �������ݽṹ��ʼ��
    BP_EVT_INIT                  = 0x00,
    // ��������
    BP_EVT_CONFIGURE,
    // �رմ���
    BP_EVT_KILLED,
    // ��������֡У��
    BP_EVT_FRAME_CHECK,
    // �л�������ģʽ
    BP_EVT_SWITCH_2_TX,
    // �л�������ģʽ
    BP_EVT_SWITCH_2_RX,

    // ���ڽ��յ�������
    BP_EVT_RX_DATA               = 0x10,
    // �����յ�����������֡
    BP_EVT_RX_FRAME,

    // ���ڷ�����������
    BP_EVT_TX_FRAME_REQUEST      = 0x20,
    // ���ڷ���ȷ��
    BP_EVT_TX_FRAME_CONFIRM,
    // �������ݷ�������¼�
    BP_EVT_TX_FRAME_DONE,

    // ���ڽ��յ����ֽڳ�ʱ�������ڽ���֡�ĵ�һ���ֽ�
    BP_EVT_RX_BYTE_TIMEOUT       = 0x40,
    // ���ڽ���֡��ʱ, ���ܵ����ݲ�����
    BP_EVT_RX_FRAME_TIMEOUT,

    // ����IO����
    BP_EVT_IO_ERROR              = 0x80,
    // ֡У��ʧ��
    BP_EVT_FRAME_CHECK_ERROR
}BP_UART_EVENT;

typedef enum {
    // Ѱ��ģʽ
    SEQ_FIND_CARD = 0,
    // ������������֤
    SEQ_SECTOR_RD_AUTH,
    // д����������֤
    SEQ_SECTOR_WR_AUTH,
    // ��ȡ����������
    SEQ_READ_PUBLIC_BLK,
    // д�빫��������
    SEQ_WRITE_PUBLIC_BLK,
    // ��ȡ˽��������1
    SEQ_READ_PRIVATE_BLK1,
    // ��ȡ˽��������2
    SEQ_READ_PRIVATE_BLK2,
    // ��ȡ˽��������3
    SEQ_READ_PRIVATE_BLK3,
    // ��ȡ˽��������4
    SEQ_READ_PRIVATE_BLK4,
    // ��ȡ˽��������5
    SEQ_READ_PRIVATE_BLK5,
    // ��ȡ˽��������6
    SEQ_READ_PRIVATE_BLK6,
    // ��ȡ˽��������7
    SEQ_READ_PRIVATE_BLK7,
    SEQ_INVALID
}QUERY_STAT;

unsigned char BCC_code(unsigned char *da,size_t len) {
    size_t i  = 0;
    unsigned char BCC = 0;

    for ( ; i < len; i ++ ) {
        BCC ^= da[i];
    }
    return ~BCC;
}

#if 0
int card_reader_handle(struct bp_uart *self, struct bp_user *me, BP_UART_EVENT evt,
                     struct bp_evt_param *param)
{
    static QUERY_STAT query_stat = SEQ_FIND_CARD;
    static struct charge_job *job = NULL;
    static char ID[16], id_len = 0, def_passwd[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

    int ret = ERR_ERR;
    unsigned char buff[64];
    int nr = 0, l;

    switch (evt) {
    case BP_EVT_FRAME_CHECK:
        if ( param->payload_size > 1 && param->payload_size >= param->buff.rx_buff[0] ) {
            if ( param->buff.rx_buff[ param->payload_size - 2 ] ==
                 BCC_code(param->buff.rx_buff, param->payload_size - 2) ) {
                return ERR_OK;
            } else {
                log_printf(DBG_LV2, "UART: SUM check result: need: %02X, gave: %02X",
                           BCC_code(param->buff.rx_buff, param->payload_size - 1),
                           param->buff.rx_buff[ param->payload_size - 2 ]);
                return ERR_FRAME_CHECK_ERR;
            }
        } else {
            return ERR_FRAME_CHECK_DATA_TOO_SHORT;
        }
        break;
    // ���ڽ��յ�������
    case BP_EVT_RX_DATA:
        break;
    // �����յ�����������֡
    case BP_EVT_RX_FRAME:
        if ( bit_read(task, S_CARD_READER_COMM_DOWN) ) {
            log_printf(INF, "������ͨ�Żָ�");
            bit_clr(task, S_CARD_READER_COMM_DOWN);
        }
        switch ( query_stat ) {
        case SEQ_FIND_CARD:
            if ( param->buff.rx_buff[0] <= 8 ) return ERR_OK;
            memcpy(ID, &param->buff.rx_buff[8], param->buff.rx_buff[7]);
            id_len = param->buff.rx_buff[7];
            log_printf(INF, "UART: Ѱ������ID:(%02X%02X%02X%02X).",
                        ID[3], ID[2], ID[1], ID[0]);
            query_stat = SEQ_SECTOR_RD_AUTH;
            ret = ERR_NEED_ECHO;
            break;
        case SEQ_SECTOR_RD_AUTH:
            if ( param->buff.rx_buff[2] == 0x00 ) {
                // ��֤�ɹ�
                log_printf(INF, "UART: ����֤�ɹ�");
                query_stat = SEQ_READ_PUBLIC_BLK;
                ret = ERR_NEED_ECHO;
            } else {
                // ��֤ʧ��
                log_printf(WRN, "UART: ����֤ʧ��");
                query_stat = SEQ_FIND_CARD;
            }
            break;
        case SEQ_SECTOR_WR_AUTH:
            if ( param->buff.rx_buff[2] == 0x00 ) {
                // ��֤�ɹ�
                log_printf(INF, "UART: д��֤�ɹ�");
                query_stat = SEQ_WRITE_PUBLIC_BLK;
                ret = ERR_NEED_ECHO;
            } else {
                // ��֤ʧ��
                log_printf(WRN, "UART: д��֤ʧ��, �۷�ʧ��!");
                query_stat = SEQ_FIND_CARD;
            }
            break;
        case SEQ_READ_PUBLIC_BLK:
            if ( param->buff.rx_buff[2] != 0 ) {
                log_printf(WRN, "UART: ��������ȡ������ʧ��, ������: %d",
                           param->buff.rx_buff[2]);
                query_stat = SEQ_FIND_CARD;
                ret = ERR_OK;
            } else {
                struct user_card cd;

                query_stat = SEQ_FIND_CARD;
                ret = ERR_OK;
                memcpy(cd.card.sector_4.buff, &param->buff.rx_buff[4], 16);
                if ( cd.card.sector_4.data.magic != 0x4F4E5057 ) {
                    log_printf(WRN, "UART: �޷�ʶ��Ŀ� %08X.", cd.card.sector_4.data.magic);
                } else if ( cd.card.sector_4.data.sum !=
                            check_sum(cd.card.sector_4.buff, 15) ) {
                    log_printf(WRN, "UART: ��������%02X, У��ʧ�� %02X.",
                               cd.card.sector_4.data.sum, check_sum(cd.card.sector_4.buff, 15));
                } else {
                    int faile = 0;
                    if ( cd.card.sector_4.data.remain_sum !=
                            check_sum(cd.card.sector_4.data.remain_money, 3) ) {
                        log_printf(WRN, "UART: �������ֶΣ� ���У��ʧ��.");
                        faile ++;
                    }
                    if ( cd.card.sector_4.data.passwd_sum !=
                                check_sum(cd.card.sector_4.data.passwd_code, 3) ) {
                        log_printf(WRN, "UART: �������ֶΣ� ����У��ʧ��.");
                        faile ++;
                    }

                    if ( ! faile ) {
                        unsigned int money = *(unsigned int *)(void*)cd.card.sector_4.data.remain_money;
                        money &= 0x00FFFFFF;
                        log_printf(INF, GRN("UART: ˢ�����[����: %02X%02X%02X%02X, ���: %.2f]"),
                                   ID[3], ID[2], ID[1], ID[0], money / 100.0f);
                        char buff[32];
                        config_write("card_status", "NORMAL");
                        sprintf(buff, "%.2f", money / 100.0f);
                        config_write("card_remaind_money", buff);
                        sprintf(buff, "%d%d%d%d%d%d",
                                cd.card.sector_4.data.passwd_code[2]>>4,
                                cd.card.sector_4.data.passwd_code[2]&0xF,
                                cd.card.sector_4.data.passwd_code[1]>>4,
                                cd.card.sector_4.data.passwd_code[1]&0xF,
                                cd.card.sector_4.data.passwd_code[0]>>4,
                                cd.card.sector_4.data.passwd_code[0]&0xF);
                        config_write("card_passwd", buff);
                        sprintf(buff, "%02X%02X%02X%02X", ID[3], ID[2], ID[1], ID[0]);

                        if ( task->uipage == UI_PAGE_JOBS ) {
                            job = job_search(task->ui_job_id);
                            if ( job == NULL ) {
                                log_printf(WRN, "��Ч��ˢ��.");
                            } else {
                                if ( 0 == strcmp(job->card.triger_card_sn, buff) ) {
                                    if ( job->charge_job_create_timestamp + 10 > time(NULL) ) {
                                        log_printf(INF, "UART: ˢ��̫�죬��ҵ����.");
                                        return ERR_OK;
                                    } else {
                                        log_printf(INF, "������ֹ����ʼ�۷ѡ�");
                                        query_stat = SEQ_SECTOR_WR_AUTH;
                                        ret = ERR_NEED_ECHO;
                                    }
                                }
                                config_write("triger_card_sn", buff);
                            }
                        } else {
                            config_write("triger_card_sn", buff);
                            //log_printf(INF, "fasdfafdsfasdfasdfasfasdfsad");
                        }
                    }
                }
            }
            break;
        case SEQ_WRITE_PUBLIC_BLK:
            log_printf(INF, "UART: ˢ���۷ѳɹ�");
            query_stat = SEQ_FIND_CARD;
            ret = ERR_OK;
            break;
        default:
            break;
        }
        break;
    // ���ڷ�����������
    case BP_EVT_TX_FRAME_REQUEST:
        switch ( query_stat ) {
        case SEQ_FIND_CARD:
            buff[ nr ++ ] = 0x08;
            buff[ nr ++ ] = 0x02;
            buff[ nr ++ ] = 0x4D;
            buff[ nr ++ ] = 0x02;
            buff[ nr ++ ] = 0x00;
            buff[ nr ++ ] = 0x26;
            l = nr;
            buff[ nr ++ ] = BCC_code(buff, l);
            buff[ nr ++ ] = 0x03;

            memcpy(param->buff.tx_buff, buff, nr);
            param->payload_size = nr;
            self->master->time_to_send = param->payload_size * 1000 / 960;
            self->rx_param.need_bytes = 15;
            log_printf(DBG_LV3, "UART: %s:SEQ_FIND_CARD requested.", __FUNCTION__);
            ret = ERR_OK;
            break;
        case SEQ_SECTOR_WR_AUTH:
        case SEQ_SECTOR_RD_AUTH:
            buff[ nr ++ ] = 0x12;
            buff[ nr ++ ] = 0x02;
            buff[ nr ++ ] = 0x46;
            buff[ nr ++ ] = 0x0C;  // 6�ֽ���Կ
            buff[ nr ++ ] = 0x60;  // ��ԿA
            buff[ nr ++ ] = ID[0];
            buff[ nr ++ ] = ID[1];
            buff[ nr ++ ] = ID[2];
            buff[ nr ++ ] = ID[3];

            buff[ nr ++ ] = def_passwd[0];
            buff[ nr ++ ] = def_passwd[1];
            buff[ nr ++ ] = def_passwd[2];
            buff[ nr ++ ] = def_passwd[3];
            buff[ nr ++ ] = def_passwd[4];
            buff[ nr ++ ] = def_passwd[5];

            buff[ nr ++ ] = 0x04;  // Ĭ�ϴ���ڵ�������

            l = nr;
            buff[ nr ++ ] = BCC_code(buff, l);
            buff[ nr ++ ] = 0x03;

            memcpy(param->buff.tx_buff, buff, nr);
            param->payload_size = nr;
            self->master->time_to_send = (param->payload_size + 1 ) * 1000 / 960;
            self->rx_param.need_bytes = 7;
            log_printf(DBG_LV3, "UART: %s:SEQ_SECTOR_RD_AUTH requested.", __FUNCTION__);
            ret = ERR_OK;
            break;
        case SEQ_READ_PUBLIC_BLK:
            buff[ nr ++ ] = 0x07;
            buff[ nr ++ ] = 0x02;
            buff[ nr ++ ] = 0x47;
            buff[ nr ++ ] = 0x01;
            buff[ nr ++ ] = 0x04;  // Ĭ�ϴ���ڵ�������
            l = nr;
            buff[ nr ++ ] = BCC_code(buff, l);
            buff[ nr ++ ] = 0x03;

            memcpy(param->buff.tx_buff, buff, nr);
            param->payload_size = nr;
            self->master->time_to_send = param->payload_size * 1000 / 960;
            self->rx_param.need_bytes = 23;
            log_printf(DBG_LV3, "UART: %s:SEQ_READ_PUBLIC_BLK requested.", __FUNCTION__);
            ret = ERR_OK;
            break;
        case SEQ_WRITE_PUBLIC_BLK:

            if ( job == NULL ) {
                log_printf(WRN, "UART: ������ʱ�����.");
                query_stat = SEQ_FIND_CARD;
                return ERR_ERR;
            }
            buff[ nr ++ ] = 0x17;
            buff[ nr ++ ] = 0x02;
            buff[ nr ++ ] = 0x48;
            buff[ nr ++ ] = 0x11;
            buff[ nr ++ ] = 0x04; // д��4����

            do {
                int i = 0;
                // ����ÿ�ο�1.5
                __card_write_remain(&job->card, __card_read_remain(&job->card) - 1.5f);
                job->card.card.sector_4.data.sum =
                        check_sum(job->card.card.sector_4.buff, 15);
                for ( i = 0; i < 16; i ++ ) {
                    buff[ nr ++ ] = job->card.card.sector_4.buff[i];
                }
            } while (0);

            l = nr;
            buff[ nr ++ ] = BCC_code(buff, l);
            buff[ nr ++ ] = 0x03;

            memcpy(param->buff.tx_buff, buff, nr);
            param->payload_size = nr;
            self->master->time_to_send = (param->payload_size + 1) * 1000 / 960;
            self->rx_param.need_bytes = 7;
            log_printf(DBG_LV3, "UART: %s:SEQ_WRITE_PUBLIC_BLK requested.", __FUNCTION__);
            ret = ERR_OK;
            break;
        default:
            query_stat = SEQ_FIND_CARD;
            break;
        }
        break;
    // ���ڷ���ȷ��
    case BP_EVT_TX_FRAME_CONFIRM:
        ret = ERR_OK;
        break;
    // �������ݷ�������¼�
    case BP_EVT_TX_FRAME_DONE:
        break;
    // ���ڽ��յ����ֽڳ�ʱ�������ڽ���֡�ĵ�һ���ֽ�
    case BP_EVT_RX_BYTE_TIMEOUT:
    // ���ڽ���֡��ʱ, ���ܵ����ݲ�����
    case BP_EVT_RX_FRAME_TIMEOUT:
        //self->master->died ++;
        if ( self->master->died < self->master->died_line ) {
            //self->master->died ++;
        } else {
            //self->master->died ++;
            if ( ! bit_read(task, S_CARD_READER_COMM_DOWN) ) {
            }
            printf("������ͨ���ж�, ���Ų����,(%d)");
        }
        query_stat = SEQ_FIND_CARD;
        break;
    // ����IO����
    case BP_EVT_IO_ERROR:
        break;
    // ֡У��ʧ��
    case BP_EVT_FRAME_CHECK_ERROR:
        break;
    default:
        log_printf(WRN, "UART: unreliable EVENT %08Xh", evt);
        break;
    }
    return ret;
}
#endif

void show_help(int cmd) 
{
}

int __read_card_sector(unsigned char *obuf, int dev, unsigned char *id, unsigned char *passwd, unsigned char sec)
{
	
}

int __read_result(int dev, unsigned char *obuf)
{
    fd_set rfds;
    struct timeval tv ;
	int retval;
	
	usleep(100 * 1000); // 100 ms

	FD_ZERO(&rfds);
	FD_SET(dev, &rfds);
	tv.tv_sec = 1;
	tv.tv_usec = 0;
	retval = select(dev+1, &rfds, NULL, NULL, &tv);
	if ( 0 >= retval ) {
		return 0;
	}

	rd = read(thiz->dev_handle, obuf, 64);
	
	return rd;
}

int find_card(int dev, unsigned char *id)
{
	unsigned char buff[32] = {0};
	unsigned char rx_buff[64] = {0};
	unsigned char bcc;
	int l, nr, ret;
	

	buff[ nr ++ ] = 0x08;
	buff[ nr ++ ] = 0x02;
	buff[ nr ++ ] = 0x4D;
	buff[ nr ++ ] = 0x02;
	buff[ nr ++ ] = 0x00;
	buff[ nr ++ ] = 0x26;
	l = nr;
	buff[ nr ++ ] = BCC_code(buff, l);
	buff[ nr ++ ] = 0x03;

	ret = write(dev, buff, nr);
	if ( 0 == ret ) {
		printf("Ѱ��ʧ��!");
		return 0;
	}
	ret = __read_result(dev, rx_buff);
	if ( ret == 0 ) {
		return 0;
	}
	
	bcc = BCC_code(rx_buff, ret - 2);
	if ( bcc != rx_buff[ ret - 2] ) {
		printf("�޷�ʶ��Ŀ�!");
		return 0;
	}

	memcpy(id, &rx_buff[8], rx_buff[7]);

	return 1;
}

int auth_card(int dev, unsigned char * id, unsigned char *passwd, unsigned char sect)
{
	unsigned char buff[32] = {0};
	unsigned char rx_buff[64] = {0};
	unsigned char bcc;
	int l, nr, ret;
	
	buff[ nr ++ ] = 0x12;
	buff[ nr ++ ] = 0x02;
	buff[ nr ++ ] = 0x46;
	buff[ nr ++ ] = 0x0C;  // 6�ֽ���Կ
	buff[ nr ++ ] = 0x60;  // ��ԿA
	buff[ nr ++ ] = id[0];
	buff[ nr ++ ] = id[1];
	buff[ nr ++ ] = id[2];
	buff[ nr ++ ] = id[3];

	buff[ nr ++ ] = passwd[0];
	buff[ nr ++ ] = passwd[1];
	buff[ nr ++ ] = passwd[2];
	buff[ nr ++ ] = passwd[3];
	buff[ nr ++ ] = passwd[4];
	buff[ nr ++ ] = passwd[5];

	buff[ nr ++ ] = sect;  // Ĭ�ϴ���ڵ�������

	l = nr;
	buff[ nr ++ ] = BCC_code(buff, l);
	buff[ nr ++ ] = 0x03;
	
	ret = write(dev, buff, nr);
	if ( 0 == ret ) {
		printf("����֤ʧ��!\n");
		return 0;
	}
	
	ret = __read_result(dev, rx_buff);
	if ( ret == 0 ) {
		return 0;
	}
	bcc = BCC_code(rx_buff, ret - 2);
	if ( bcc != rx_buff[ ret - 2] ) {
		printf("�޷�ȷ����֤���!");
		return 0;
	}
	if ( rx_buff[2] != 0x00 ) {
		printf("��֤ʧ�ܣ� %d\n", rx_buff[2]);
		return 1;
	}

	printf("����֤�ɹ�!\n");
	return 1;
}

int read_card(int dev, unsigned char *id, unsigned char *passwd, unsigned char sec)
{
	unsigned char rx_buff[128]  = {0};
	struct user_card cd;

	if ( ! auth_card(dev, id, passwd, sec) ) {
		return 0;
	}
	
	if ( ! __read_card_sector(rx_buff, dev, id, passwd, sec) ) {
		printf("��ȡ����%dʧ��!\n", sec);
		return 0;
	}

	printf("��ȡ�ɹ�!\n");
	printf("\t%16s: %02X%02X%02X%02X\n", "ID", id[3], id[2], id[1], id[0]);
	memcpy(cd.card.sector_4.buff, &rx_buff[4], 16);
	if ( cd.card.sector_4.data.magic != 0x4F4E5057 ) {
		printf("\t%16s: %08X\n", "Magic", cd.card.sector_4.data.magic);
		printf("\t%16s: %s\n", "Status", "Unrecognized");
		return 0;
	} else if ( cd.card.sector_4.data.sum !=
				check_sum(cd.card.sector_4.buff, 15) ) {
		char str[32];

		printf("\t%16s: %08X\n", "Magic", cd.card.sector_4.data.magic);
		printf("\t%16s: %s\n", "Status", "Fault");
		sprintf(str, "Faile, gave: %02X, need: %02X", 
		cd.card.sector_4.data.sum, check_sum(cd.card.sector_4.buff, 15));
		printf("\t%16s: %s\n", "Check Code", str);
	} else {
		int faile = 0;
		char str[32];

		printf("\t%16s: %08X\n", "Magic", cd.card.sector_4.data.magic);
		printf("\t%16s: %s\n", "Status", "OK");
		sprintf(str, "OK, gave: %02X, need: %02X", 
		cd.card.sector_4.data.sum, check_sum(cd.card.sector_4.buff, 15));
		printf("\t%16s: %s\n", "Check Code", str);

		if ( cd.card.sector_4.data.remain_sum !=
				check_sum(cd.card.sector_4.data.remain_money, 3) ) {
			faile ++;
			printf("\t%16s: %s\n", "Money Check Code", str);		
		}
		if ( cd.card.sector_4.data.passwd_sum !=
					check_sum(cd.card.sector_4.data.passwd_code, 3) ) {
			faile ++;
			printf("\t%16s: %s\n", "Password Check Code", str);	
		}
		if ( ! faile ) {
			unsigned int money = *(unsigned int *)(void*)cd.card.sector_4.data.remain_money;
			char buff[32];
			money &= 0x00FFFFFF;
			sprintf(buff, "%.2f", money / 100.0f);
			printf("\t%16s: %s\n", "Reamin money", buff);
			sprintf(buff, "%d%d%d%d%d%d",
					cd.card.sector_4.data.passwd_code[2]>>4,
					cd.card.sector_4.data.passwd_code[2]&0xF,
					cd.card.sector_4.data.passwd_code[1]>>4,
					cd.card.sector_4.data.passwd_code[1]&0xF,
					cd.card.sector_4.data.passwd_code[0]>>4,
					cd.card.sector_4.data.passwd_code[0]&0xF);
			printf("\t%16s: %s\n", "Password", buff);
		}
	}
	return 0;
}

int format_card(int, dev, unsigned char *id, unsigned char *def_passwd, unsigned int money, unsigned int sec)
{
	return 0;
}

int write_card(int, dev, unsigned char *id, unsigned char *new_passwd, unsigned int money, unsigned int sec)
{
	return 0;
}

typedef enum {
	READ_CARD,
	FORMAT_CARD,
	WRITE_CARD
}RUNMOD;

int main(int argc, const char *argv[])
{
	unsigned char passwd[16] = {255, 255, 255, 255, 255, 255};
	int money = 0.0f;
	char device[256] = {0};
	unsigned char id[16] = {0};
	int dev = -1;
	unsigned char frame_tx[128];
	unsigned char frame_rx[128];
	int done = 0;
	int inifinal = 0;
	int ok = 0;
	RUNMOD mode = READ_CARD;

	while((oc = getopt(argc, argv, "rfwID:P:M:")) != -1)    
	{         
		switch(oc)        
		{
		case 'r':
			mode = READ_CARD;
		break;
		case 'f':
			mode = FORMAT_CARD;
		break;
		case 'w':
			mode = WRITE_CARD;
		break;
		case 'I':
			inifinal = 1;
		break;
		case 'D'
			strncpy(device, optarg, 256);
		break;
		case 'P':
			strncpy(passwd, optarg, 6);
			do {
				int len = strlen(passwd);
				int i;
				for ( i = 0; i < len; i ++ )
					passwd[i] = passswd[i] - '0';
			} while ( 0);
		break;
		case 'M'
			money = (int)atof(optarg) * 100;
		break;                
		default:
			show_help(1);
			exit(1);
		break;  
		}
	}
	
	dev = open(dev, O_RDWR | O_NOCTTY);
	if ( dev == -1 ) {
		printf("open device %s faile!\n", device);
		exit(1);
	}

	while ( ! done )
	{
		memset(id, 0, sizeof(id));
		while ( ! find_card(dev, id) ) {
			sleep(1);
		}
	
		printf("Ѱ����: %s\n", id);
		
		switch ( mode ) {
		case READ_CARD:
			read_card(dev, id, passswd, SECT);
		break;
		case FORMAT_CARD:
		ok = format_card(dev, id, passwd, money, SECT);
		if ( ok ) {
			getch();
		}
		break;
		case WRITE_CARD:
		ok = write_card(dev, id, passwd, money, SECT);
		if ( ok ) {
			getch();
		}
		break;
		}

		if ( inifinal ) continue;
		done = 1;
	}

	return 0;
}