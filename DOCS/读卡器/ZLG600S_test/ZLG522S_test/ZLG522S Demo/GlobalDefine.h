////////////////////////////////////////////////Copyright (c)////////////////////////////////////////////////
//                                      ������������Ƭ����չ���޹�˾
//                                            ��    ��    ��
//                                              �𿨲�Ʒ�� 
//
//                                          http://www.zlgmcu.com
//
//--------------�ļ���Ϣ-------------------------------------------------------------------------------------
// �� �� ��:    GlobalDefine.h
// �ļ�����:    ȫ�ֶ���
// ��    ��:    V1.00 
// �� �� ��:    ����ɭ
// ��������:    2010.11.23
//===========================================================================================================
//-----------------�޸ļ�¼----------------------------------------------------------------------------------
// �޸�����:
// ��ǰ�汾:    
// �� �� ��:
// �޸�����:
//-----------------------------------------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef __GLOBAL_DEFINE_H
#define __GLOBAL_DEFINE_H

#ifdef __cplusplus
	extern "C" {
#endif
//============= �������� ====================================================================================
// ģ�����Ͷ���
#define MT_ZLG500S					0
#define MT_ZLG522S					1
#define MT_ZLG522SP					2			// ��׼֡ģʽ	
#define MT_ZLG522SP_L				3			// ��֡ģʽ	
#define MT_ZLG522SP_EX				4			// ��չ֡ģʽ

// PCD����ģʽ
#define ISO14443_TYPEA				0			// 
			
// ֡�ṹ
#define CMD_STX						0x20		// ֡��ʼ�ַ�
#define CMD_ETX						0x03		// ֡�����ַ�
#define CMD_ACK						0x06		// Ӧ���ַ�
#define CMD_NAK						0x15		// ��Ӧ���ַ�

//  ����/��Ӧ֡�ṹ
// ��ͬ��ģ�飬֡���Ȳ�һ�£���׼֡���ȷ�����3�֣�
#define CMD_SHORT_FRAME_SIZE		(64 + 6)	// ��׼֡���(ZLG500S)
#define CMD_STAN_FRAME_SIZE			(64 + 8)	// ��׼֡���(ZLG522S���Ͱ汾��ZLG522SPҲʹ�����ֳ���)
#define CMD_LONG_FRAME_SIZE			255			// ��׼֡��󳤶�(ZLG522SP)
		
#define CMD_STAN_PRTC_LEN			6			// ��׼֡Э�鳤��
// ��չ֡��ʽ		
#define CMD_MAX_SIZE	            300			// ��չ֡����󳤶�
#define CMD_PRTC_LEN				7
#define CMD_INFO_MAX_SIZE	        (CMD_MAX_SIZE - CMD_PRTC_LEN)			
												// �������� APDU( 256 + 8 ) + CMD_PRTC_LEN( 6/8 )
												// ������󳤶� = 256
												// APDUЭ�鳤�� = 8
												// ExChangeBlock3����Э�鳤�� = 6
// �������	
#define ISO14443_3A_REQALL          0x52		// �������еĿ�
#define ISO14443_3A_REQIDL          0x26		// ������еĿ�
#define SELECT_LEVEL_1				0x93		// һ������ײ/ѡ��
#define SELECT_LEVEL_2				0x95		// ��������ײ/ѡ��
#define SELECT_LEVEL_3				0x97		// ��������ײ/ѡ��

#define KEY_TYPE_A            		0x60		// ��ԿA
#define KEY_TYPE_B            		0x61		// ��ԿB
#define KEY_SOURCE_EXT				0x00		// ʹ���ⲿ�������Կ��֤
#define KEY_SOURCE_E2				0x80		// ʹ���ڲ�E2����Կ��֤
	
#define MIFARE_INCREMENT            0xC1		// ֵ��ģʽ
#define MIFARE_DECREMENT            0xC0		// ֵ��ģʽ
#define MIFARE_RESTORE              0xC2		// ����ģʽ
#define MIFARE_TRANSFER             0xB0		// ����ģʽ
//============= ȫ�ֱ���/�������� ===========================================================================
typedef struct 
{
	unsigned char ATQ[2];						// ISO14443A�������Ӧ
	unsigned char UIDLen;						// ISO14443A��UID���ֽ���
	unsigned char UID[10];						// ISO14443A��UID
	unsigned char SAK;							// ISO14443A��ѡ��Ӧ��
} ACTIVEPARAA;
	
typedef struct									// �Զ�������
{	unsigned char ADMode;						// �Զ����ģʽ	
												// ADMode.0			= 0 -- ���ڲ����ͣ�		1 -- ������������
												// ADMode.1			= 0 -- �������жϣ�		1 -- �����ж�
												// ADMode.2			= 0 -- ���������ֹͣ��	1 -- ���������������
												// ADMode.3			= 0 -- ���ִ��Halt()��	1 -- ���ִ��Halt()
	unsigned char TXMode;						// ��������ģʽ		
												// ADMode.1:0		= 00 -- TX1��TX2��������
												//					= 01 -- ��TX1����
												//					= 10 -- ��TX2����
												//					= 11 -- TX1��TX2ͬʱ����
	unsigned char ReqCode;						// �������			0x26 -- IDLE;	0x52 -- ALL
	unsigned char AuthMode;						//��֤ģʽ		
												// AuthMode.6:0		= 0x60 -- ��ԿA
												//					= 0x61 -- ��ԿB
												//					= ���� -- ����֤
												// AuthMode.7		= 0 -- �ⲿ��Կ��֤
												//					= 1 -- E2PRAM��Կ��֤
	unsigned char Key[16];						// ��Կ(1/16�ֽ�)	����E2PRAM��Կ��֤����Ϊ��Կ���ţ�1�ֽڣ�
												//					���ⲿ��Կ��֤����Ϊ��Կ��16�ֽڣ�
	unsigned char BNr;							// ������ʼ���
	unsigned char Ext;							// ���Ŀ���			ȡֵ��Χ1~3(ͬһ������)
}AUTO_DETECT_PAR;
	
typedef struct
{
	ACTIVEPARAA TAInfo;					    	// A�Ϳ���Ϣ
	unsigned int nBytes;						// �������ݵ��ֽ���
	unsigned char Buf[48];						// ����������
}AUTO_DETECT_RES;

// ��׼֡��ʽ
typedef struct
{
	unsigned char FarmeLen;						// ֡����
	unsigned char SeqOrType;                 	// ���ź���������
	unsigned char CmdOrStatus;                	// ������/״̬
	unsigned char InfoLen;						// ֡��Ϣ����
	unsigned char Info[CMD_INFO_MAX_SIZE];		// ֡������Ϣ��֡β(֡β����BCC�ͽ����ַ�)
}CMD_STAN_FRAME;
// ��չ֡��ʽ
typedef struct
{
	unsigned char SeqOrType;                 	// ���ź���������
	unsigned char CmdOrStatus;                	// ������/״̬
	unsigned short InfoLen;						// ֡��Ϣ����
	unsigned char Info[CMD_INFO_MAX_SIZE];		// ֡������Ϣ��֡β(֡β����BCC�ͽ����ַ�)
}CMD_EX_FRAME;

typedef union
{
	unsigned char Buf[CMD_MAX_SIZE];
	CMD_EX_FRAME Stru;
}CDU;	
//============= ȫ�ֱ���/�������� ===========================================================================

//===========================================================================================================
#ifdef __cplusplus
	}
#endif

#endif				// __GLOBAL_DEFINE_H
