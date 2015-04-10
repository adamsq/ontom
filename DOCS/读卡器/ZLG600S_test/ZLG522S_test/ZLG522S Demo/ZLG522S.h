////////////////////////////////////////////////Copyright (c)////////////////////////////////////////////////
//                                      ������������Ƭ����չ���޹�˾
//                                            ��    ��    ��
//                                              �𿨲�Ʒ�� 
//
//                                          http://www.zlgmcu.com
//
//--------------�ļ���Ϣ-------------------------------------------------------------------------------------
// �� �� ��:    ZLG522S(P).h
// �ļ�����:    ZLG522S(P) ģ�鶯̬��
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
#ifndef __ZLG522S_H
#define __ZLG522S_H

#ifdef __cplusplus
	extern "C" {
#endif
//===========================================================================================================
#include "GlobalDefine.h"
#include "StatusCode.h"
//============= �������� ====================================================================================	
//============= ȫ�ֱ���/�������� ===========================================================================
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:	bool ZLG522S_Init(const char *pszComName, unsigned int uiBaudRate, unsigned int uiModuleType)
// ��������:	ZLG5xxSϵ��ģ���ʼ��
// ��ڲ���:	const char *pszComName			// ��'\0'�����Ĵ������ƣ���"COM1"
//				unsigned int uiBaudRate			// ���ò�����
//				unsigned int uiModuleType		// ģ������				
// ���ڲ���:	-
// �� �� ֵ:	true -- ��ʼ���ɹ�;	false -- ��ʼ��ʧ��
// ˵    ��:	������ֻ����CBR_9600, CBR_19200, CBR_28800, CBR_38400, CBR_57600, CBR_115200
//				ģ������ΪMT_ZLG500S, MT_ZLG522S, MT_ZLG522SP, MT_ZLG522SP_L, MT_ZLG522SP_EX
//				ֻ��ZLG522SP V1.01����(����V1.01)����ѡ��MT_ZLG522SP_L��MT_ZLG522SP_EX2��ģʽ
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool __stdcall ZLG522S_Init(const char *pszComName, unsigned int uiBaudRate, unsigned int uiModuleType);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:	HANDLE ZLG522S_Open(unsigned int uiModuleAdr)
// ��������:	��ZLG522S
// ��ڲ���:	unsigned int uiModuleAdr		// ��ַ(��������չͨѶ֡��0x00Ϊͨ�õ�ַ)				
// ���ڲ���:	-
// �� �� ֵ:	�Ǹ��� -- �ɹ���	-1 -- ʧ��
// ˵    ��:	��û�г�ʼ������Ĭ��ѡ��COM1
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
HANDLE __stdcall ZLG522S_Open(unsigned int uiModuleAdr);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:	void ZLG522S_Exit(HANDLE hCtrl)
// ��������:	ZLG522S�ر��˳�
// ��ڲ���:	HANDLE hCtrl					// ͨѶ����				
// ���ڲ���:	-
// �� �� ֵ:	-
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __stdcall ZLG522S_Exit(HANDLE hCtrl);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char ZLG522S_TransmitCmd(HANDLE hCtrl, CDU *RWCmd, CDU *RWRes)
// ��������:    ���д������������ջ�Ӧ
// ��ڲ���:    HANDLE hCtrl					// ͨѶ����
//				CDU *RWCmd						// ����				
// ���ڲ���:    CDU *RWRes						// ��Ӧ
// �� �� ֵ:    -
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall ZLG522S_TransmitCmd(HANDLE hCtrl, CDU *pRWCmd, CDU *pRWRes);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char ZLG522S_GetResponse(HANDLE hCtrl, unsigned int uiTimeOver, CDU *pRWRes)
// ��������:    ���ջ�Ӧ
// ��ڲ���:    HANDLE hCtrl					// ͨѶ����
//				unsigned int uiTimeOverCnt		// ��ʱʱ��(ms)				
// ���ڲ���:    CDU *RWRes						// ��Ӧ
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ�
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall ZLG522S_GetResponse(HANDLE hCtrl, unsigned int uiTimeOver, CDU *pRWRes);

//===========================================================================================================
// DevCtrl
//===========================================================================================================
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:	void  ZLG522S_GetDllInfo(HANDLE hCtrl, char *pDllInfo,unsigned int nRBufSize)
// ��������:	��ȡDLL�汾��Ϣ
// ��ڲ���:	HANDLE hCtrl					// ͨѶ����
//				unsigned int nRBufSize			// ���ջ�������С			
// ���ڲ���:	char *pDllInfo					// DLL�汾��Ϣ
//
// �� �� ֵ:
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __stdcall ZLG522S_GetDllInfo(HANDLE hCtrl, char *pDllInfo, unsigned int nRBufSize);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:	unsigned char ZLG522S_GetDevInfo(HANDLE hCtrl, unsigned char *pDevInfo, 
//												 unsigned int nRBufSize, unsigned int *pInfoLen)
// ��������:	��ȡ�豸��Ϣ
// ��ڲ���:	HANDLE hCtrl					// ͨѶ����
//				unsigned int nRBufSize			// ���ջ������ĳ���				
// ���ڲ���:	unsigned char *pDevInfo			// �豸��Ϣ
//				unsigned int *pInfoLen			// �豸��Ϣ����
// �� �� ֵ:	STATUS_SUCCESS -- �����ɹ�������ֵ -- ����ʧ��	
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall ZLG522S_GetDevInfo(HANDLE hCtrl, unsigned char *pDevInfo, unsigned int nRBufSize, 
										   unsigned int *pInfoLen);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:	unsigned char ZLG522S_PcdConfig(HANDLE hCtrl, unsigned char ucType)
// ��������:	���ö�д����PCD
// ��ڲ���:	HANDLE hCtrl					// ͨѶ����
//				unsigned char ucType			//  ����
// ���ڲ���:	-
// �� �� ֵ:	STATUS_SUCCESS -- �����ɹ�������ֵ -- ����ʧ��
// ˵    ��:    ZLG522Sϵ��ģ��ֻ֧��ISO14443_TYPEA	
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall ZLG522S_PcdConfig(HANDLE hCtrl, unsigned char ucType);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:	unsigned char ZLG522S_PcdClose(HANDLE hCtrl)
// ��������:	�رն�д����PCD
// ��ڲ���:	HANDLE hCtrl					// ͨѶ����
// ���ڲ���:	-
// �� �� ֵ:	STATUS_SUCCESS -- �����ɹ�������ֵ -- ����ʧ��	
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall ZLG522S_PcdClose(HANDLE hCtrl);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char ZLG522S_PcdLoadKey(HANDLE hCtrl, unsigned char ucKeyType, 
//												 unsigned char ucKeySector, unsigned char ucKeyLen, 
//												 const unsigned char *pKey)
// ��������:    ��Mifare ����֤��Կ��ŵ���д����
// ��ڲ���:    HANDLE hCtrl					// ͨѶ����
//				unsigned char ucKeyType			// ��Կ���� KEY_TYPE_A -- ��ԿA;	KEY_TYPE_B -- ��ԿB;
//              unsigned char ucKeySector		// ��Կ�������
//				unsigned char ucKeyLen			// ��Կ���ֽ���(6/16�ֽ�)
//				unsigned char *pKey				// д�����Կ
// ���ڲ���:    -
// �� �� ֵ:    STATUS_SUCCESS -- �����ɹ�������ֵ -- ����ʧ��
// ˵    ��:	�ú����ǶԶ�д�����������ǶԿ�Ƭִ����֤��Կ������
//				ZLG522SP֧��2����Կ����,��������6�ֽڵ���Կ����ģ���Զ�����2��ȡǰ16�ֽ���Ϊ��Կ
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall ZLG522S_PcdLoadKey(HANDLE hCtrl, unsigned char ucKeyType, unsigned char ucKeySector,
										   unsigned char ucKeyLen, const unsigned char *pKey);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char ZLG522S_PcdWriteReg(HANDLE hCtrl, unsigned char ucRegAdr, unsigned char ucValue)
// ��������:    д�ǽӴ�ʽ��д��оƬ�ļĴ���
// ��ڲ���:    HANDLE hCtrl					// ͨѶ����
//				unsigned char ucRegAdr          // �Ĵ�����ַ
//              unsigned char ucValue           // д�������
// ���ڲ���:    -
// �� �� ֵ:    STATUS_SUCCESS -- �����ɹ�������ֵ -- ����ʧ��
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall ZLG522S_PcdWriteReg(HANDLE hCtrl, unsigned char ucRegAdr, unsigned char ucValue);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char ZLG522S_PcdReadReg(HANDLE hCtrl, unsigned char ucRegAdr, unsigned char *pValue)
// ��������:    ���ǽӴ�ʽ��д��оƬ�ļĴ���
// ��ڲ���:    HANDLE hCtrl					// ͨѶ����
//				unsigned char ucRegAdr			// �Ĵ�����ַ
// ���ڲ���:    unsigned char *pValue			// ����������
// �� �� ֵ:    STATUS_SUCCESS -- �����ɹ�������ֵ -- ����ʧ��
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall ZLG522S_PcdReadReg(HANDLE hCtrl, unsigned char ucRegAdr, unsigned char *pValue);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char ZLG522S_SetDevWorkMode(HANDLE hCtrl, unsigned char ucChangeMode, 
//													 unsigned char ucWorkMode, unsigned char ucBaudRateNO,
//													 unsigned char ucModuleAdr, AUTO_DETECT_PAR AutoDetPar)
// ��������:    ����ģ�鹤��ģʽ(ZLG522SP)
// ��ڲ���:    HANDLE hCtrl					// ͨѶ����
//				unsigned char ucChangeMode		// ucChangeMode.0	= 1		�޸Ĺ���ģʽ
//												// ucChangeMode.1	= 1		�޸Ĳ����� 
//												// ucChangeMode.2	= 1		�޸�ģ���ַ 
//												// ucChangeMode.3	= 1		�޸Ŀ�Ƭ�Զ����ģʽ 
//				unsigned char ucWorkMode		// ucWorkMode.2:0	= 000	�Զ����ģʽ
//												//					= 001	UARTͨѶģʽ
//												//					= 010	I2CͨѶ��ģʽ 
//												//					= ����	����
//												// ucWorkMode.3		= 0 -- ��׼ͨѶ֡ģʽ��1 -- ��չͨѶ֡ģʽ
//												// ucWorkMode.4		= 0	-- ����--��Ӧģʽ��1 -- ��Ƭ�Զ����ģʽ		
//				unsigned char ucBaudRateNO		// �����ʱ�ţ�ȡֵ��Χ0��7(Ĭ��ֵΪ1)
//												// ���:   0	 1	   2	 3	   4	 5	    6(��֧��)	7(��֧��)
//												// ������: 9600  19200 28800 38400 57600 115200 172800		230400
//				unsigned char ucModuleAdr		// ģ���ַ���õ�ַ����UART����չͨ��֡��I2Cͨ�ŵĵ�ַ
//				OTO_DETECT_PAR AutoDetPar		// �Զ�������
// ���ڲ���:	-
// �� �� ֵ:    STATUS_SUCCESS -- �����ɹ�������ֵ -- ����ʧ��
// ˵	 ��:	ֻ������ΪUARTͨ�Ż�I2Cͨ��ģʽ��������Ϊ��Ƭ�Զ����ģʽ
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall ZLG522S_SetDevWorkMode(HANDLE hCtrl,  unsigned char ucChangeMode, 
											   unsigned char ucWorkMode, unsigned char ucBaudRateNO,
											   unsigned char ucModuleAdr, AUTO_DETECT_PAR AutoDetPar);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char ZLG522S_PcdSetTX(HANDLE hCtrl, unsigned char ucTXMode)
// ��������:    ������������ģʽ(ZLG522SP)
// ��ڲ���:    HANDLE hCtrl					// ͨѶ����
//				unsigned char ucTXMode			// 0x03 -- TX1��TX2ͬʱ��������
//												// 0x02 -- TX2��������
//												// 0x01 -- TX1��������
//												// 0x00 -- ��������ر�
// ���ڲ���:	-
// �� �� ֵ:    STATUS_SUCCESS -- �����ɹ�������ֵ -- ����ʧ��
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall ZLG522S_PcdSetTX(HANDLE hCtrl, unsigned char ucTXMode);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char ZLG522S_GetDevWorkMode(HANDLE hCtrl, unsigned char *pWorkMode, 
//													 unsigned char *pBaudRateNO, unsigned char *pModuleAdr, 
//													 AUTO_DETECT_PAR *pAutoDetPar)
// ��������:    ��ȡģ�鹤��ģʽ(ZLG522SP)
// ��ڲ���:    -				
// ���ڲ���:	HANDLE hCtrl					// ͨѶ����
//				unsigned char *pWorkMode		// ucWorkMode.2:0	= 000	�Զ����ģʽ
//												//					= 001	UARTͨѶģʽ
//												//					= 010	I2CͨѶ��ģʽ 
//												//					= ����	����
//												// ucWorkMode.3		= 0 -- ��׼ͨѶ֡ģʽ��1 -- ��չͨѶ֡ģʽ
//												// ucWorkMode.4		= 0	-- ����--��Ӧģʽ��1 -- ��Ƭ�Զ����ģʽ	
//				unsigned char *pBaudRateNO		// �����ʱ�ţ�ȡֵ��Χ0��7(Ĭ��ֵΪ1)
//												// ���:   0	 1	   2	 3	   4	 5	    6(��֧��)	7(��֧��)
//												// ������: 9600  19200 28800 38400 57600 115200 172800		230400
//				unsigned char *pModuleAdr		// ģ���ַ���õ�ַ����UART����չͨ��֡��I2Cͨ�ŵĵ�ַ
//				OTO_DETECT_PAR *pAutoDetPar		// �Զ�������
// �� �� ֵ:    STATUS_SUCCESS -- �����ɹ�������ֵ -- ����ʧ��
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char ZLG522S_GetDevWorkMode(HANDLE hCtrl, unsigned char *pWorkMode, unsigned char *pBaudRateNO,
									 unsigned char *pModuleAdr, AUTO_DETECT_PAR *pAutoDetPar);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:	unsigned char CardReset(HANDLE hCtrl, unsigned char ucPause_1ms,unsigned char ucWait_1ms)
// ��������:	ʹRF������1����ͣ���ÿ���λ
// ��ڲ���:	HANDLE hCtrl					// ͨѶ����
//				unsigned char ucPause_1ms				// ��ͣʱ�䣬�ر�RF����ʱ������´򿪣�
//														// ��Ϊ0�����´�
//				unsigned char ucWait_1ms				// RF�����´򿪺�ȴ�ʱ�䣬��Ϊ0�򲻵ȴ�
// ���ڲ���:	-
// �� �� ֵ:	-
// ˵    ��:	�ȴ�ʱ��ɸ��ݿ��Ĺ��Ķ�������Mifare1���Ĺ��Ľ�С���ȴ������뼴�ɣ�
//				��CPU�����Ľϴ���Ҫ�ȴ�80�������ҡ�
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall ZLG522S_CardReset(HANDLE hCtrl, unsigned char ucPause_1ms, unsigned char ucWait_1ms);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char ZLG522S_ExChangeBlock(HANDLE hCtrl, unsigned char WTXM_CRC, unsigned char ucFWI, 
//													const unsigned char *pSBuf, unsigned int uiSBytes,
//													unsigned char *pRBuf, unsigned int uiRBufSize, 
//													unsigned int *pRBytes)
// ��������:    ���ݽ���
// ��ڲ���:    HANDLE hCtrl					// ͨѶ����
//				unsigned char WTXM_CRC			// WTXM_CRC.0	= 0  -- CRC��ֹ��	1 -- CRCʹ��
//												// WTXM_CRC.7:2	= WTXM��֡��ʱ�ȴ�����ϵ��
//				unsigned char FWI				// ֡�ȴ�ϵ��ȡֵ��Χ(1-14)
//				unsigned char *pSBuf			// �������ݻ�����
//				unsigned int uiSBytes			// ���͵��ֽ���
//				unsigned int uiRBufSize			// ���ջ�������С
// ���ڲ���:    unsigned char *pRBuf			// �������ݻ�����
//				unsigned int *pRBytes			// ���յ��ֽ���
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ���	����ֵ -- ʧ�ܡ�
// ˵    ��:	֡�ȴ�ʱ��Ϊ ((0x01 << FWI) * WTXM) * 302us
//				0x01 << FWI) * WTXM��ֵ���ܳ���0xFFFF
//				��ͬ��ģ�飬��������ĳ��Ȳ�ͬ
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall ZLG522S_ExChangeBlock(HANDLE hCtrl, unsigned char WTXM_CRC, unsigned char ucFWI, 
											  const unsigned char *pSBuf, unsigned int uiSBytes,
											  unsigned char *pRBuf, unsigned int uiRBufSize, 
											  unsigned int *pRBytes);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char ZLG522S_ExChangeBlock2(HANDLE hCtrl, unsigned char CRC_EN, unsigned int uiTre,
//													 const unsigned char *pSBuf, unsigned int nSendBits,
//													 unsigned int *pTime, unsigned char *pRBuf,
//													 unsigned int nRBufSize, unsigned int *pRcvBits)
// ��������:    PCD��PICC�������ݿ�
// ��ڲ���:    HANDLE hCtrl					// ͨѶ����
//				unsigned char CRC_EN,			// CRC_EN.7 = 1 -- ����ʹ��CRC;	0 -- ���ͽ�ֹCRC; 
//												// CRC_EN.6 = 1 -- ����ʹ��CRC; 0 -- ���ս�ֹCRC; 
//				unsigned int uiTre				// ��ʱʱ�� = uiTre *  4.7us
//				unsigned char *pSBuf			// ���͵�����
//				unsigned int nSendBits			// ���͵�λ���������͵�λ������8��������������CRCʹ����Ч		
//				unsigned int nRBufSize			// ���ջ������Ĵ�С
// ���ڲ���:    unsigned int *pTime				// ִ�е�ʱ�� = pTime *  4.7us
//				unsigned char *pRBuf			// ���յ�����
//				unsigned int *pRcvBits			// ���յ�λ��
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ�
// ˵    ��:	ZLG500S��ZLG522S��֧�ָ����� 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall ZLG522S_ExChangeBlock2(HANDLE hCtrl,  unsigned char CRC_EN, unsigned int uiTre,
												const unsigned char *pSBuf, unsigned int nSendBits,
												unsigned int *pTime, unsigned char *pRBuf,
												unsigned int nRBufSize, unsigned int *pRcvBits);
//===========================================================================================================
// Mifare
//===========================================================================================================
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    void ZLG522S_ValueToData(HANDLE hCtrl, unsigned char *pBuf, unsigned char usBNr, long lValue)
// ��������:    ��ֵת��Ϊ���ݿ�ģʽ
// �������:    HANDLE hCtrl					// ͨѶ����
//				unsigned char usBNr				// ֵ��ŵ�ַ
//				long iValue						// ֵ
// �������:    unsigned char *pBuf				// 16�ֽ�ֵ���ʽ����
// �� �� ֵ:    -
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __stdcall ZLG522S_ValueToData(unsigned char *pBuf, unsigned char ucBNr, long lValue);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char ZLG522S_DataToValue(unsigned char *pBuf, long *pValue)
// ��������:    �����ݿ�ת��Ϊֵ
// �������:    unsigned char *pBuf				// 16�ֽ�ֵ���ʽ����	
// �������:    long *pValue					// ֵ()
// �� �� ֵ:    -
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall ZLG522S_DataToValue(unsigned char *pBuf, long *pValue);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char ZLG522S_MFActivate(HANDLE hCtrl,  unsigned char ucMode, unsigned char ucReqCode,  
//												 ACTIVEPARAA *pAInfo)
// ��������:   	Mifareϵ�п�����
// ��ڲ���:    HANDLE hCtrl					// ͨѶ����
//				unsigned char ucMode			// ����
//				unsigned char ucReqCode			// �������
// ���ڲ���:    ACTIVEPARAA *pAInfo				// ISO14443 A��������Ϣ
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ�  
// ˵    ��:	�ú����Զ�ʶ��M1 ��PLUS CPU����SL1��SL2��SL3
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall ZLG522S_MFActivate(HANDLE hCtrl, unsigned char ucMode, unsigned char ucReqCode, 
										   ACTIVEPARAA *pAInfo);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char MFDeSelect(HANDLE hCtrl)
// ��������:    ������Ŀ�����(ISO14443-4A)
// �������:     HANDLE hCtrl					// ͨѶ����
// ���ڲ���:    -
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ�
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall ZLG522S_MFDeSelect(HANDLE hCtrl);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char ZLG522S_PiccHalt(HANDLE hCtrl)
// ��������:   	������ΪHALT״̬(ISO14443-3A)
// ��ڲ���:    -
// ���ڲ���:    -
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ�
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall ZLG522S_MFHalt(HANDLE hCtrl);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char ZLG522S_MFAuth(HANDLE hCtrl, unsigned char ucAuthMode, unsigned char ucBNr, 
//											 const unsigned char *pKey)
// ��������:   	Mifare����Կ��֤
// ��ڲ���:    HANDLE hCtrl					// ͨѶ����
//				unsigned char ucAuthMode		// ��֤ģʽ 
//												//	KEY_TYPE_A | KEY_SOURCE_EXT	�ⲿ�������ԿA��֤
//												//	KEY_TYPE_A | KEY_SOURCE_E2	�ڲ�E2����ԿA��֤
//												//	KEY_TYPE_B | KEY_SOURCE_EXT	�ⲿ�������ԿB��֤
//												//	KEY_TYPE_B | KEY_SOURCE_E2	�ڲ�E2����ԿB��֤
//				unsigned char ucBNr				// ��֤�Ŀ��
//				unsigned char *pKey				// ʹ��KEY_SOURCE_E2ģʽʱ��pKey[0]Ϊ��Կ��ŵ�����,������Ч
//												// ʹ��KEY_SOURCE_EXTģʽʱ��pKeyΪ16�ֽ���Կ
// ���ڲ���:    -
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ�
// ˵    ��:	�ú����Զ�ʶ���Զ�ʶ��M1 ��PLUS CPU����SL1��SL2��SL3
//				����ֻ��6�ֽ���Կ�����(��M1 S50/70��)���ȡ�����16�ֽ���Կ��ǰ6�ֽ���Ϊ��Կ
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall ZLG522S_MFAuth(HANDLE hCtrl, unsigned char ucAuthMode, unsigned char ucBNr, 
									   const unsigned char *pKey);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char ZLG522S_MFRead(HANDLE hCtrl, unsigned char ucBNr, unsigned char ucExt, 
//											 unsigned char *pBuf)
// ��������:   	MF������
// ��ڲ���:    HANDLE hCtrl					// ͨѶ����
//				unsigned char ucBNr				// ������ʼ���
//				unsigned char ucExt				// ���Ŀ���(ȡֵ��Χ1��3)
// ���ڲ���:    unsigned char *pBuf				// ����������
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ�
// ˵    ��:	��ģ��ΪZLG522Sʱ���ú���ֻ֧��M1 S50/70����PLUS SL1�Ŀ�,��ucExt�̶�Ϊ1
//				��ģ��ΪZLG522SPʱ���ú����Զ�ʶ���Զ�ʶ��M1 ��PLUS CPU����SL1��SL2��SL3
//				����MF1��PLUS SL1�Ŀ������б������Ŀ���Ҫ��ͬһ�����ڣ���������
//				����PLUS SL2/SL3�Ŀ��������������Ŀ����ͬһ������(��Ϊ������������1ʱ�����Զ��������ÿ�)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall ZLG522S_MFRead(HANDLE hCtrl, unsigned char ucBNr, 
									   unsigned char ucExt, unsigned char *pBuf);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char ZLG522S_MFWrite(HANDLE hCtrl, unsigned char ucBNr, unsigned char ucExt, 
//											  const unsigned char *pBuf)
// ��������:   	MF�����д
// ��ڲ���:    HANDLE hCtrl					// ͨѶ����
//				unsigned char ucBNr				// д����ʼ���
//				unsigned char ucExt				// д�Ŀ���(ȡֵ��Χ1��3)
// ���ڲ���:    unsigned char *pBuf				// д�������
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ�
// ˵    ��:	��ģ��ΪZLG522Sʱ���ú���ֻ֧��M1 S50/70����PLUS SL1�Ŀ�
//				��ģ��ΪZLG522SPʱ���ú����Զ�ʶ���Զ�ʶ��M1 ��PLUS CPU����SL1��SL2��SL3
//				����MF1��PLUS SL1�Ŀ������б������Ŀ���Ҫ��ͬһ�����ڣ���������
//				����PLUS SL2/SL3�Ŀ��������������Ŀ����ͬһ������(��Ϊ������������1ʱ�����Զ��������ÿ�)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall ZLG522S_MFWrite(HANDLE hCtrl, unsigned char ucBNr, unsigned char ucExt, 
										const unsigned char *pBuf);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char ZLG522S_MFReadValue(HANDLE hCtrl, unsigned char ucBNr, long *pValue)
// ��������:   	MF����ȡֵ��(��չ����)
// ��ڲ���:    HANDLE hCtrl					// ͨѶ����
//				unsigned char ucBNr				// ���Ŀ��
// ���ڲ���:    long *pValue					// ������ֵ
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ�
// ˵    ��:	��ģ��ΪZLG522Sʱ���ú���ֻ֧��M1 S50/70����PLUS SL1�Ŀ�
//				��ģ��ΪZLG522SPʱ���ú����Զ�ʶ���Զ�ʶ��M1 ��PLUS CPU����SL1��SL2��SL3
//				PLUS S/X SL2 �� PLUS S SL3��֧��ֵ��
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall ZLG522S_MFReadValue(HANDLE hCtrl, unsigned char ucBNr, long *pValue);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char ZLG522S_MFWriteValue(HANDLE hCtrl, unsigned char ucBNr, long lValue)
// ��������:   	MF��д��ֵ
// ��ڲ���:    HANDLE hCtrl					// ͨѶ����
//				unsigned char ucBNr				// д�Ŀ��
//				long lValue						// д���ֵ
// ���ڲ���:    -
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ�
// ˵    ��:	��ģ��ΪZLG522Sʱ���ú���ֻ֧��M1 S50/70����PLUS SL1�Ŀ�
//				��ģ��ΪZLG522SPʱ���ú����Զ�ʶ���Զ�ʶ��M1 ��PLUS CPU����SL1��SL2��SL3
//				PLUS S/X SL2 �� PLUS S SL3��֧��ֵ��
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall ZLG522S_MFWriteValue(HANDLE hCtrl, unsigned char ucBNr, long lValue);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char ZLG522S_MFValueOper(HANDLE hCtrl, unsigned char ucOperMode, unsigned char ucSBNr, 
//												  unsigned char ucDBNr, long lValue)
// ��������:    Mifareֵ��������(PLUS CPU����SL2��ֵ����)
// ��ڲ���:    HANDLE hCtrl					// ͨѶ����
//				unsigned char ucOperMode		// ֵ����ģʽ
//													MIFARE_INCREMENT ��Դ���ֵ����ָ����ֵ��ŵ�Ŀ�Ŀ�
//													MIFARE_DECREMENT ��Դ���ֵ��ȥָ����ֵ��ŵ�Ŀ�Ŀ�
//													MIFARE_RESTORE   ��Դ���ֵ���Ƶ�Ŀ�Ŀ�
//				unsigned char ucSBNr			// ������Դ���ַ��
//				unsigned char ucDBNr			// ֵ������Ŀ�ؿ�
//				long lValue						// �Ǹ����з�������
// ���ڲ���:    -
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ�
// ˵    ��:	��ģ��ΪZLG522Sʱ���ú���ֻ֧��M1 S50/70����PLUS SL1�Ŀ�
//				��ģ��ΪZLG522SPʱ���ú����Զ�ʶ���Զ�ʶ��M1 ��PLUS CPU����SL1��SL2��SL3
//				MIFARE_RESTORE ����ģʽ�ǽ�ָ����ֵ�鸴�Ƶ�Ŀ�Ŀ飬һ������ֵ������ı��ݴ���(��ģʽ�£�
//				����ֵ��Ч��Ϊ����ֵ����)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall ZLG522S_MFValueOper(HANDLE hCtrl, unsigned char ucOperMode, unsigned char ucSBNr, 
											unsigned char ucDBNr, long lValue);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char ZLG522S_MF0ULWrite(HANDLE hCtrl, unsigned char ucBNr, const unsigned char *pBuf)
// ��������:    MF0 ULд����
// ��ڲ���:    HANDLE hCtrl					// ͨѶ����
//				unsigned char ucBNr				// д��Ŀ��ַ��
//				unsigned char *pBuf				// 4�ֽ�д���ݻ�������ַ��
// ���ڲ���:    -
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ� 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall ZLG522S_MF0ULWrite(HANDLE hCtrl, unsigned char ucBNr, const unsigned char *pBuf);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char ZLG522S_MF1Auth(HANDLE hCtrl, unsigned char ucAuthMode, unsigned char ucBNr, 
//											  const unsigned char *pKey)
// ��������:   	MF1����Կ��֤
// ��ڲ���:    HANDLE hCtrl					// ͨѶ����
//				unsigned char ucAuthMode		// ��֤ģʽ 
//												//	KEY_TYPE_A | KEY_SOURCE_EXT	�ⲿ�������ԿA��֤
//												//	KEY_TYPE_A | KEY_SOURCE_E2	�ڲ�E2����ԿA��֤
//												//	KEY_TYPE_B | KEY_SOURCE_EXT	�ⲿ�������ԿB��֤
//												//	KEY_TYPE_B | KEY_SOURCE_E2	�ڲ�E2����ԿB��֤
//				unsigned char ucBNr				// ��֤�Ŀ��
//				unsigned char *pKey				// ʹ��KEY_SOURCE_E2ģʽʱ��pKey[0]Ϊ��Կ��ŵ�����,������Ч
//												// ʹ��KEY_SOURCE_EXTģʽʱ��pKeyΪ6�ֽ���Կ
// ���ڲ���:    -
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ�
// ˵    ��:	�ú���ֻ֧��M1 S50/70����PLUS SL1�Ŀ�
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall ZLG522S_MF1Auth(HANDLE hCtrl, unsigned char ucAuthMode, unsigned char ucBNr, 
									   const unsigned char *pKey);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char ZLG522S_MFRead(HANDLE hCtrl, unsigned char ucBNr, unsigned char *pBuf)
// ��������:   	MF1����
// ��ڲ���:    HANDLE hCtrl					// ͨѶ����
//				unsigned char ucBNr				// ���Ŀ��
// ���ڲ���:    unsigned char *pBuf				// ����������
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ�
// ˵    ��:	�ú���ֻ֧��M1 S50/70����PLUS SL1�Ŀ�
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall ZLG522S_MF1Read(HANDLE hCtrl, unsigned char ucBNr, unsigned char *pBuf);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char ZLG522S_MF1Write(HANDLE hCtrl, unsigned char ucBNr, const unsigned char *pBuf)
// ��������:   	MF1��д
// ��ڲ���:    HANDLE hCtrl					// ͨѶ����
//				unsigned char ucBNr				// д�Ŀ��
// ���ڲ���:    unsigned char *pBuf				// д�������
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ�
// ˵    ��:	�ú���ֻ֧��M1 S50/70����PLUS SL1�Ŀ�
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall ZLG522S_MF1Write(HANDLE hCtrl, unsigned char ucBNr, const unsigned char *pBuf);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char ZLG522S_MF1ReadValue(HANDLE hCtrl, unsigned char ucBNr, long *pValue)
// ��������:   	MF1����ȡֵ��
// ��ڲ���:    HANDLE hCtrl					// ͨѶ����
//				unsigned char ucBNr				// ���Ŀ��
// ���ڲ���:    long *pValue					// ������ֵ
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ�
// ˵    ��:	�ú���ֻ֧��M1 S50/70����PLUS SL1�Ŀ�
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall ZLG522S_MF1ReadValue(HANDLE hCtrl, unsigned char ucBNr, long *pValue);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char ZLG522S_MFWriteValue(HANDLE hCtrl, unsigned char ucBNr, long lValue)
// ��������:   	MF1��д��ֵ
// ��ڲ���:    HANDLE hCtrl					// ͨѶ����
//				unsigned char ucBNr				// д�Ŀ��
//				long lValue						// д���ֵ
// ���ڲ���:    -
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ�
// ˵    ��:	��ģ��ΪZLG522Sʱ���ú���ֻ֧��M1 S50/70����PLUS SL1�Ŀ�
//				��ģ��ΪZLG522SPʱ���ú����Զ�ʶ���Զ�ʶ��M1 ��PLUS CPU����SL1��SL2��SL3
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall ZLG522S_MF1WriteValue(HANDLE hCtrl, unsigned char ucBNr, long lValue);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char ZLG522S_MF1ValueOper(HANDLE hCtrl, unsigned char ucOperMode, unsigned char ucSBNr, 
//												  unsigned char ucDBNr, long lValue)
// ��������:    Mifareֵ��������
// ��ڲ���:    HANDLE hCtrl					// ͨѶ����
//				unsigned char ucOperMode		// ֵ����ģʽ
//													MIFARE_INCREMENT ��Դ���ֵ����ָ����ֵ��ŵ�Ŀ�Ŀ�
//													MIFARE_DECREMENT ��Դ���ֵ��ȥָ����ֵ��ŵ�Ŀ�Ŀ�
//													MIFARE_RESTORE   ��Դ���ֵ���Ƶ�Ŀ�Ŀ�
//				unsigned char ucSBNr			// ������Դ���ַ��
//				unsigned char ucDBNr			// ֵ������Ŀ�ؿ�
//				long lValue						// �Ǹ����з�������
// ���ڲ���:    -
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ�
// ˵    ��:	MIFARE_RESTORE ����ģʽ�ǽ�ָ����ֵ�鸴�Ƶ�Ŀ�Ŀ飬һ������ֵ������ı��ݴ���(��ģʽ�£�
//				����ֵ��Ч��Ϊ����ֵ����)
//				�ú���ֻ֧��M1 S50/70����PLUS SL1�Ŀ�
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall ZLG522S_MF1ValueOper(HANDLE hCtrl, unsigned char ucOperMode, unsigned char ucSBNr, 
											unsigned char ucDBNr, long lValue);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char ZLG522S_SetAutoDetectPar(HANDLE hCtrl, const AUTO_DETECT_PAR *pAutoDetectPar)
// ��������:    ����A�Ϳ��Զ�������
// ��ڲ���:    HANDLE hCtrl					// ͨѶ����
//				AUTO_DETECT_PAR *pAutoDetectPar	// A�Ϳ��Զ�������
// ���ڲ���:    -
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ�
// ˵    ��:	ZLG522Sֻ֧�ֶ�һ������
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall ZLG522S_SetAutoDetectPar(HANDLE hCtrl, const AUTO_DETECT_PAR *pAutoDetectPar);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char ZLG522S_GetAutoDetectRes(HANDLE hCtrl, unsigned char ucMode, 
//													   unsigned char *pTxMode, AUTO_DETECT_RES *pAutoDetectRes)
// ��������:    ��ȡA�Ϳ��Զ����Ľ�������ڼ�⵽���������ϴ�����ģʽ��
// ��ڲ���:    HANDLE hCtrl					// ͨѶ����
//				unsigned char ucMode			// ucMode.0		= 0  -- ȡ���Զ���⣻	1 -- �������
// ���ڲ���:    unsigned char *pTxMode			// pTxMode.1:0	= 01 -- TX1����
//												//				= 10 -- TX2����
//												//				= 11 -- TX1��TX2ͬʱ����
//				AUTO_DETECT_RES *pAutoDetectRes	// ��Ƭ��Ϣ
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ���	
//				STATUS_NO_TARGET -- δ��⵽��
//				STATUS_NY_IMPLEMENTED -- ��֧�ָ����û�������Զ���������
//				����ֵ -- ʧ�ܡ�
// ˵    ��:	�ú�����ZLG522S_ReadAutoDetectRes()�ǻ���ģ�����ͬʱ����
//				ZLG500S��ZLG522S��ZLG522SPϵ��ģ��֧��
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall ZLG522S_GetAutoDetectRes(HANDLE hCtrl, unsigned char ucMode, unsigned char *pTxMode,
												 AUTO_DETECT_RES *pAutoDetectRes);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char ReadAutoDetectRes(HANDLE hCtrl, unsigned int uiTimeOver, 
//												unsigned char *pTxMode, AUTO_DETECT_RES *pAutoDetectRes);
// ��������:    ��ȡA�Ϳ��Զ����Ľ�������ڼ�⵽�������������ϴ�ģʽ��
// ��ڲ���:    HANDLE hCtrl					// ͨѶ����
//				unsigned int uiTimeOver			// ucMode.0		= 0  -- ȡ���Զ���⣻	1 -- �������
// ���ڲ���:    unsigned char *pTxMode			// pTxMode.1:0	= 01 -- TX1����
//												//				= 10 -- TX2����
//												//				= 11 -- TX1��TX2ͬʱ����
//				AUTO_DETECT_RES *pAutoDetectRes	// ��Ƭ��Ϣ
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ���
//				STATUS_COMM_ERROR -- δ��⵽��/ͨѶ����	
//				����ֵ -- ʧ�ܡ�
// ˵    ��:	�ú�����ZLG522S_GetAutoDetectRes()�ǻ���ģ�����ͬʱ����
//				ZLG500S��ZLG522S��ZLG522SPϵ��ģ��֧��
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall ZLG522S_ReadAutoDetectRes(HANDLE hCtrl, unsigned int uiTimeOver, 
												  unsigned char *pTxMode, AUTO_DETECT_RES *pAutoDetectRes);
//===========================================================================================================
// ISO14443 
//===========================================================================================================
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char ZLG522S_PiccRequest(HANDLE hCtrl, unsigned char ucReqCode,unsigned char *pATQ)
// ��������:    ����(ISO14443-3A)
// ��ڲ���:    HANDLE hCtrl					// ͨѶ����
//				unsigned char ucReqCode			// �������	ISO14443_3A_REQIDL	0x26	IDLE
//												// 			ISO14443_3A_REQALL	0x52	ALL
// ���ڲ���:    unsigned char *pATQ				// �����Ӧ��,2�ֽ�
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ�
// ˵    ��:	ZLG500S��ZLG522Sû�иú�������MFRequest()������ʵ�� 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall ZLG522S_PiccRequest(HANDLE hCtrl, unsigned char ucReqCode, unsigned char *pATQ);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char ZLG522S_PiccAnticoll(HANDLE hCtrl,  unsigned char unMode, unsigned char ucSelCode,  
//													unsigned char ucBitCnt, unsigned char *pUID)
// ��������:    λ��ʽ����ײ(ISO14443-3A)
// ��ڲ���:    HANDLE hCtrl					// ͨѶ����
//				unsigned char unMode			// ����������Ϊ1��
//				unsigned char ucSelCode			// SELECT_LEVEL_1--����ײ�ȼ�1
//												// SELECT_LEVEL_2--����ײ�ȼ�2
//												// SELECT_LEVEL_3--����ײ�ȼ�3
//				unsigned char ucBitCnt			// ��֪���кŵ�λ��
// ���ڲ���:    unsigned char *pUID				// �������кŻ�������ַ��4�ֽڡ�
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ�
// ˵    ��:	ZLG500S��ZLG522Sû�иú�������MFAnticoll()������ʵ��    
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall ZLG522S_PiccAnticoll(HANDLE hCtrl, unsigned char unMode, unsigned char ucSelCode, 
											 unsigned char ucBitCnt, unsigned char *pUID);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char ZLG522S_PiccSelect(HANDLE hCtrl, unsigned char ucSelCode, const unsigned char *pUID, 
//												 unsigned char *pSAK)
// ��������:    ѡ��(ISO14443-3A)
// ��ڲ���:    HANDLE hCtrl					// ͨѶ����
//				unsigned char ucSelCode			// ����ײ����	SELECT_LEVEL_1����1��
//												// 				SELECT_LEVEL_2����2��
//												// 				SELECT_LEVEL_3����3��
//				unsigned char *pUID				// 4�ֽ����кš�
// ���ڲ���:    unsigned char *pSAK				// ѡ��Ӧ��
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ�
// ˵    ��:	ZLG500S��ZLG522Sû�иú�������MFSelect()������ʵ��   
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall ZLG522S_PiccSelect(HANDLE hCtrl, unsigned char ucSelCode, const unsigned char *pUID, 
										   unsigned char *pSAK);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char ZLG522S_PiccHaltA(HANDLE hCtrl)
// ��������:   	������ΪHALT״̬(ISO14443-3A)
// ��ڲ���:    HANDLE hCtrl					// ͨѶ����
// ���ڲ���:    -
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ�
// ˵    ��:	ZLG500S��ZLG522Sû�иú�������MFHalt()������ʵ�� 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall ZLG522S_PiccHaltA(HANDLE hCtrl);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char ZLG522S_PiccRequestATS(HANDLE hCtrl, unsigned char ucCID, unsigned char *pATS, 
//													 unsigned int nATSBufSize, unsigned int *pATSBytes)
// ��������:    ����ѡ����Ӧ��
// ��ڲ���:    HANDLE hCtrl					// ͨѶ����
//				unsigned char ucCID				// ��Ѱַ��PICC�߼���� 0x00~0x0E
//				unsigned int nATSBufSize		// ATS��������С		
// ���ڲ���:    unsigned char *pATS				// Ӧ����ѡ��ATS��
//				unsigned int *pATSBytes			// ATS���ֽ���
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ�
// ˵    ��:	ZLG500S��ZLG522Sû�иú�������ExChangeBlock()������ʵ��
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall ZLG522S_PiccRequestATS(HANDLE hCtrl, unsigned char ucCID, unsigned char *pATS, 
											   unsigned int nATSBufSize, unsigned int *pATSBytes);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char ZLG522S_PiccPPSRequest(HANDLE hCtrl, unsigned char ucDSI_DRI)
// ��������:    Э��Ͳ���ѡ������
// �������:    HANDLE hCtrl					// ͨѶ����
//				unsigned char ucDSI_DRI			// b3:2 -- PCD  -> PICC��λ�ʱ���(ֻ֧��0)
//												// b1:0 -- PICC -> PCD ��λ�ʱ���(ֻ֧��0)
// �������:    -
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ�
// ˵    ��:	ZLG500S��ZLG522Sû�иú�������ExChangeBlock()������ʵ��
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall ZLG522S_PiccPPSRequest(HANDLE hCtrl, unsigned char ucDSI_DRI);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char ZLG522S_PiccDeSelect(HANDLE hCtrl)
// ��������:    ������Ŀ�����
// �������:    HANDLE hCtrl					// ͨѶ����		
// �������:    -
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ�
// ˵    ��:	ZLG500S��ZLG522Sû�иú�������ExChangeBlock()������ʵ��
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall ZLG522S_PiccDeSelect(HANDLE hCtrl);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char ZLG522S_PiccTPCL(HANDLE hCtrl, const unsigned char *pSBuf, unsigned int nSBytes, 
//									   unsigned char *pRBuf, unsigned int nRBufSize, unsigned int *pRBytes)
// ��������:    ʵ�ַǽӴ�ʽIC������Э��(T = CL)����ISO14443-4 
// �������:    HANDLE hCtrl					// ͨѶ����
//				unsigned char *pSBuf			// ���͵�����	
//				unsigned int  nSBytes			// ���͵��ֽ���
//				unsigned int nRBufSize			// ���ջ�������С	
// �������:    unsigned char *pRBuf			// ���յ�����
//				unsigned int *pRBytes			// ���յ��ֽ���
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ�
// ˵    ��:	ZLG500S��ZLG522Sû�иú�������ExChangeBlock()������ʵ��
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall ZLG522S_PiccTPCL(HANDLE hCtrl, const unsigned char *pSBuf, unsigned int nSBytes, 
										 unsigned char *pRBuf, unsigned int nRBufSize, unsigned int *pRBytes);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char ZLG522S_PiccActivateTypeA(HANDLE hCtrl, unsigned char ucMode,  
//														unsigned char ucReqCode, ACTIVEPARAA *pAInfo)
// ��������:   	A�Ϳ�����
// ��ڲ���:    HANDLE hCtrl					// ͨѶ����
//				unsigned char ucMode			// ����������Ϊ1
//				unsigned char ucReqCode			// �������
// ���ڲ���:    ACTIVEPARAA *pAInfo				// ISO14443 A��������Ϣ
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ�
// ˵    ��:	ZLG500S��ZLG522Sû�иú�������MFActivate()������ʵ��  
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall ZLG522S_PiccActivateTypeA(HANDLE hCtrl, unsigned char ucMode, 
												  unsigned char ucReqCode, ACTIVEPARAA *pAInfo);
//===========================================================================================================
// PLUS CPU
//===========================================================================================================
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char ZLG522S_SL0WritePerso(HANDLE hCtrl, unsigned int uiBNr, const unsigned char *pBuf)
// ��������:    SL0 ���˻���������
// ��ڲ���:    HANDLE hCtrl					// ͨѶ����
//				unsigned int uiBNr				// ���ַ
//				unsigned char *pBuf				// д�������
// ���ڲ���:    -
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ�
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall ZLG522S_SL0WritePerso(HANDLE hCtrl, unsigned int uiBNr, const unsigned char *pBuf);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char ZLG522S_SL0CommitPerso(HANDLE hCtrl)
// ��������:    SL0 �ύ���˻�
// ��ڲ���:    HANDLE hCtrl					// ͨѶ����
// ���ڲ���:    -
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ�
// ˵    ��:	��SL0��PLUS CPU����ֻ���޸������µ�ַ����ִ��"SL0�ύ���˻�"���
//				0x9000(������Կ)
//				0x9001(���ÿ���Կ)
//				0x9002(SL2������Կ��ֻ��֧��SL2�Ŀ����и���Կ)
//				0x9003(SL3������Կ��ֻ��֧��SL3�Ŀ����и���Կ)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall ZLG522S_SL0CommitPerso(HANDLE hCtrl);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char ZLG522S_SL1Auth(HANDLE hCtrl, unsigned char ucAuthMode, unsigned char ucBNr, 
//											 const unsigned char *pKey)
// ��������:    SL1��֤��Կ
// ��ڲ���:    HANDLE hCtrl					// ͨѶ����
//				unsigned char ucAuthMode		// ��֤ģʽ 
//													KEY_TYPE_A | KEY_SOURCE_EXT	�ⲿ�������ԿA��֤
//													KEY_TYPE_A | KEY_SOURCE_E2	�ڲ�E2����ԿA��֤
//													KEY_TYPE_B | KEY_SOURCE_EXT	�ⲿ�������ԿB��֤
//													KEY_TYPE_B | KEY_SOURCE_E2	�ڲ�E2����ԿB��֤
//				unsigned char ucBNr				// ��֤�Ŀ��	MF1 S50����0��63
//												//				MF1 S70����0��255
//				unsigned char *pKey				// ʹ��KEY_SOURCE_E2ģʽʱ��pKey[0]Ϊ��Կ��ŵ�����,������Ч
//												// ʹ��KEY_SOURCE_EXTģʽʱ��pKeyΪ6�ֽ���Կ
// ���ڲ���:    -
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ�
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall ZLG522S_SL1Auth(HANDLE hCtrl, unsigned char ucAuthMode, unsigned char ucBNr, 
										const unsigned char *pKey);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char ZLG522S_SL1Read(HANDLE hCtrl, unsigned char ucBNr, unsigned char *pBuf)
// ��������:   	SL1�����ݿ�
// ��ڲ���:    HANDLE hCtrl					// ͨѶ����
//				unsigned char ucBNr				// ���Ŀ��
// ���ڲ���:    unsigned char *pBuf				// ����������
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ�
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall ZLG522S_SL1Read(HANDLE hCtrl, unsigned char ucBNr, unsigned char *pBuf);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char ZLG522S_SL1Write(HANDLE hCtrl, unsigned char ucBNr, const unsigned char *pBuf)
// ��������:   	SL1д���ݿ�
// ��ڲ���:    HANDLE hCtrl					// ͨѶ����
//				unsigned char ucBNr				// д�Ŀ��
//				unsigned char *pBuf				// д�������
// ���ڲ���:    -
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ�
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall ZLG522S_SL1Write(HANDLE hCtrl, unsigned char ucBNr, const unsigned char *pBuf);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char ZLG522S_SL1ReadValue(HANDLE hCtrl, unsigned char ucBNr, long *pValue)
// ��������:   	SL1����ȡֵ��
// ��ڲ���:    HANDLE hCtrl					// ͨѶ����
//				unsigned char ucBNr				// ���Ŀ��
// ���ڲ���:    long *pValue					// ������ֵ
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ�
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall ZLG522S_SL1ReadValue(HANDLE hCtrl, unsigned char ucBNr, long *pValue);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char ZLG522S_SL1WriteValue(HANDLE hCtrl, unsigned char ucBNr, long lValue)
// ��������:   	SL1��д��ֵ
// ��ڲ���:    HANDLE hCtrl					// ͨѶ����
//				unsigned char ucBNr				// д�Ŀ��
//				long *pValue					// д����ֵ
// ���ڲ���:    -
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ�
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall ZLG522S_SL1WriteValue(HANDLE hCtrl, unsigned char ucBNr, long lValue);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char ZLG522S_SL1ValueOper(HANDLE hCtrl, unsigned char ucOperMode, unsigned char ucSBNr, 
//													unsigned char ucDBNr, long lValue)
// ��������:    SL1ֵ����
// ��ڲ���:    HANDLE hCtrl					// ͨѶ����
//				unsigned char ucOperMode		// ֵ����ģʽ
//													MIFARE_INCREMENT ��Դ���ֵ����ָ����ֵ��ŵ�Ŀ�Ŀ�
//													MIFARE_DECREMENT ��Դ���ֵ��ȥָ����ֵ��ŵ�Ŀ�Ŀ�
//													MIFARE_RESTORE   ��Դ���ֵ���Ƶ�Ŀ�Ŀ�
//													MIFARE_TRANSFER	 ���������е����ݴ��䵽ָ���Ŀ�	
//				unsigned char ucSBNr			// ������Դ���ַ��
//				unsigned char ucDBNr			// ֵ������Ŀ�ؿ�
//				long lValue						// �Ǹ����з�������
// ���ڲ���:    -
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ�
// ˵    ��:	ģʽΪMIFARE_RESTOREʱ��������lValue����Ч��
//				ģʽΪMIFARE_TRANSFERʱ��������lValue����Ч �͡�ucSBNr����Ч��
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall  ZLG522S_SL1ValueOper(HANDLE hCtrl, unsigned char ucOperMode, unsigned char ucSBNr, 
											  unsigned char ucDBNr, long lValue);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char ZLG522S_SL2AESAuth(HANDLE hCtrl, unsigned char ucAuthMode, unsigned int uiKNr, 
//												 const unsigned char *pAESKey)
// ��������:    SL2��֤��Կ
// ��ڲ���:    HANDLE hCtrl					// ͨѶ����
//				unsigned char ucAuthMode		// ��֤ģʽ 
//													KEY_SOURCE_EXT	�ⲿ�������Կ��֤
//													KEY_SOURCE_E2	�ڲ�E2����Կ��֤
//				unsigned int uiKNr				// 2KByte�Ŀ���ȡֵ��Χ0x4000��0x403F
//												// 4KByte�Ŀ���ȡֵ��Χ0x4000��0x404F
//				unsigned char *pKey				// ʹ��KEY_SOURCE_E2ģʽʱ��pKey[0]Ϊ��Կ��ŵ�����,������Ч
//												// ʹ��KEY_SOURCE_EXTģʽʱ��pKeyΪ6�ֽ���Կ
// ���ڲ���:    -
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ�
// ˵    ��:	uiKNr��ȡֵ��ֹ0x4000��0x404F����ϸ���������PLUS CPU��������
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall ZLG522S_SL2AESAuth(HANDLE hCtrl, unsigned char ucAuthMode, unsigned int uiKNr, 
										   const unsigned char *pAESKey);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char ZLG522S_SL2CryptoAuth(HANDLE hCtrl, unsigned char ucAuthMode, unsigned int uiKNr, 
//													const unsigned char *pKey)
// ��������:    SL2��֤��Կ
// ��ڲ���:    HANDLE hCtrl					// ͨѶ����
//				unsigned char ucAuthMode		// ��֤ģʽ 
//													KEY_TYPE_A | KEY_SOURCE_EXT	�ⲿ�������ԿA��֤
//													KEY_TYPE_A | KEY_SOURCE_E2	�ڲ�E2����ԿA��֤
//													KEY_TYPE_B | KEY_SOURCE_EXT	�ⲿ�������ԿB��֤
//													KEY_TYPE_B | KEY_SOURCE_E2	�ڲ�E2����ԿB��֤
//				unsigned char ucBNr			// 2KByte�Ŀ���ȡֵ��Χ0��127
//												// 4KByte�Ŀ���ȡֵ��Χ0��255
//				unsigned char *pKey				// ʹ��KEY_SOURCE_E2ģʽʱ��pKey[0]Ϊ��Կ��ŵ�����,������Ч
//												// ʹ��KEY_SOURCE_EXTģʽʱ��pKeyΪ6�ֽ���Կ
// ���ڲ���:    -
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ�
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall ZLG522S_SL2CryptoAuth(HANDLE hCtrl, unsigned char ucAuthMode, unsigned char ucBNr, 
											  const unsigned char *pKey);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char ZLG522S_SL2MultiBlockRead(HANDLE hCtrl, unsigned char ucBNr, unsigned char ucExt, 
//														unsigned char *pBuf)
// ��������:   	SL2����
// ��ڲ���:    HANDLE hCtrl					// ͨѶ����
//				unsigned char ucBNr				// ������ʼ���
//				unsigned char ucExt				// ���Ŀ���(ȡֵ��Χ1��3)
// ���ڲ���:    unsigned char *pBuf				// ����������(ucExt * 16)
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ�
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall ZLG522S_SL2MultiBlockRead(HANDLE hCtrl, unsigned char ucBNr, unsigned char ucExt, 
												  unsigned char *pBuf);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char ZLG522S_SL2MultiBlockWrite(HANDLE hCtrl, unsigned char ucBNr, 
//														 unsigned char ucExt, const unsigned char *pBuf)
// ��������:   	SL2 ���д
// ��ڲ���:    HANDLE hCtrl					// ͨѶ����
//				unsigned char ucBNr				// д����ʼ���
//				unsigned char ucExt				// д�Ŀ���(ȡֵ��Χ1��3)
//				unsigned char *pBuf				// д�������
// ���ڲ���:    -
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ�
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall ZLG522S_SL2MultiBlockWrite(HANDLE hCtrl, unsigned char ucBNr, unsigned char ucExt, 
												   const unsigned char *pBuf);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char ZLG522S_SL3FirstAuth(HANDLE hCtrl,  unsigned char ucAuthMode, unsigned int uiKNr, 
//													const unsigned char *pAESKey)
// ��������:    SL3 �״���֤
// ��ڲ���:    HANDLE hCtrl					// ͨѶ����
//				unsigned char ucAuthMode		// ��֤ģʽ 
//													KEY_SOURCE_EXT	�ⲿ�������Կ��֤
//													KEY_SOURCE_E2	�ڲ�E2����Կ��֤
//				unsigned int uiKNr				// ��Կ��ַ
//				unsigned char *pKey				// ʹ��KEY_SOURCE_E2ģʽʱ��pKey[0]Ϊ��Կ��ŵ�����,������Ч
//												// ʹ��KEY_SOURCE_EXTģʽʱ��pKeyΪ6�ֽ���Կ
// ���ڲ���:    -
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ�
// ˵    ��:	uiKNr��ȡֵ��ֹ0x4000��0x404F����ϸ���������PLUS CPU��������
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall ZLG522S_SL3FirstAuth(HANDLE hCtrl, unsigned char ucAuthMode, unsigned int uiKNr, 
											 const unsigned char *pAESKey);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char ZLG522S_SL3FollowingAuth(HANDLE hCtrl,  unsigned char ucAuthMode,
//													   unsigned int uiKNr, const unsigned char *pAESKey)
// ��������:    SL3 ������֤
// ��ڲ���:    HANDLE hCtrl					// ͨѶ����
//				unsigned char ucAuthMode		// ��֤ģʽ 
//													KEY_SOURCE_EXT	�ⲿ�������Կ��֤
//													KEY_SOURCE_E2	�ڲ�E2����Կ��֤
//				unsigned int uiKNr				// ��Կ��ַ
//				unsigned char *pKey				// ʹ��KEY_SOURCE_E2ģʽʱ��pKey[0]Ϊ��Կ��ŵ�����,������Ч
//												// ʹ��KEY_SOURCE_EXTģʽʱ��pKeyΪ16�ֽ���Կ
// ���ڲ���:    -
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ�
// ˵    ��:	���ݿ��������Կ��ַΪ0x4000��0x404Fż����ַΪ��ԿA,������ַΪ��ԿB
//				uiKNr��ȡֵ��ֹ0x4000��0x404F����ϸ���������PLUS CPU��������
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall ZLG522S_SL3FollowingAuth(HANDLE hCtrl, unsigned char ucAuthMode, unsigned int uiKNr, 
												 const unsigned char *pAESKey);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char ZLG522S_SL3ResetAuth(HANDLE hCtrl)
// ��������:    SL3 ��λ��֤
// ��ڲ���:    HANDLE hCtrl					// ͨѶ����
// ���ڲ���:    -
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ�
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall ZLG522S_SL3ResetAuth(HANDLE hCtrl);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char ZLG522S_SL3Read(HANDLE hCtrl, unsigned char ucMode, unsigned int uiBNr, 
//											  unsigned char ucExt, unsigned char *pBuf)
// ��������:   	SL3 �����ݿ�
// ��ڲ���:    HANDLE hCtrl					// ͨѶ����
//				unsigned char ucMode			// ����ģʽ	0x31 -- ������MAC���������ģ���Ӧ��MAC
//												//			0x33 -- ������MAC���������ģ���Ӧ��MAC
//				unsigned int uiBNr				// ������ʼ���
//				unsigned char ucExt				// ���Ŀ���
// ���ڲ���:    unsigned char *pBuf				// ����������
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ�
// ˵    ��:	ģʽ�е�����/������ָRF���е����ݴ����Ƿ���ܣ���д����PC֮�䴫�������ʼ��������
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall ZLG522S_SL3Read(HANDLE hCtrl, unsigned char ucMode, unsigned int uiBNr, 
										unsigned char ucExt, unsigned char *pBuf);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char ZLG522S_SL3Write(HANDLE hCtrl, unsigned char ucMode, unsigned int uiBNr, 
//												unsigned char ucExt, const unsigned char *pBuf)
// ��������:   	SL3 д
// ��ڲ���:    HANDLE hCtrl					// ͨѶ����
//				unsigned char ucMode			// д��ģʽ	0xA1 -- ������MAC���������ģ���Ӧ��MAC��
//															0xA3 -- ������MAC���������ģ���Ӧ��MAC��
//				unsigned int uiBNr				// д����ʼ���
//				unsigned char ucExt				// д�Ŀ���(ȡֵ��Χ1��3)
//				unsigned char *pBuf				// д�������
// ���ڲ���:    -
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ�
// ˵    ��:	ģʽ�е�����/������ָRF���е����ݴ����Ƿ���ܣ���д����PC֮�䴫�������ʼ��������
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall ZLG522S_SL3Write(HANDLE hCtrl, unsigned char ucMode, unsigned int uiBNr, 
										 unsigned char ucExt, const unsigned char *pBuf);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char ZLG522S_SL3ReadValue(HANDLE hCtrl, unsigned char ucMode, 
//													unsigned int uiBNr, long *pValue)
// ��������:   	SL3����ȡֵ��
// ��ڲ���:    HANDLE hCtrl					// ͨѶ����
//				unsigned char ucMode			// ����ģʽ	0x31 -- ������MAC���������ģ���Ӧ��MAC
//												//			0x33 -- ������MAC���������ģ���Ӧ��MAC
//				unsigned int uiBNr				// ���Ŀ��
// ���ڲ���:    long *pValue					// ������ֵ
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ�
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall ZLG522S_SL3ReadValue(HANDLE hCtrl, unsigned char ucMode, 
											 unsigned int uiBNr, long *pValue);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char ZLG522S_SL3WriteValue(HANDLE hCtrl, unsigned char ucMode, 
//													unsigned int uiBNr, long lValue)
// ��������:   	SL3��д��ֵ
// ��ڲ���:    HANDLE hCtrl					// ͨѶ����
//				unsigned char ucMode			// д��ģʽ	0xA1 -- ������MAC���������ģ���Ӧ��MAC��
//															0xA3 -- ������MAC���������ģ���Ӧ��MAC��
//				unsigned int uiBNr				// д�Ŀ��
//				long *pValue					// д����ֵ
// ���ڲ���:    -
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ�
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall ZLG522S_SL3WriteValue(HANDLE hCtrl, unsigned char ucMode, 
											  unsigned int uiBNr, long lValue);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char ZLG522S_SL3ValueOper(HANDLE hCtrl, unsigned char ucMode, unsigned int uiSBNr, 
//													unsigned int uiDBNr, long lValue)
// ��������:    SL3ֵ����
// ��ڲ���:    HANDLE hCtrl					// ͨѶ����
//				unsigned char ucOperMode		// ֵ����ģʽ
//													SL3_INCREMENT	��Դ���ֵ����ָ����ֵ��ŵ�Ŀ�Ŀ�
//													SL3_DECREMENT	��Դ���ֵ��ȥָ����ֵ��ŵ�Ŀ�Ŀ�
//													SL3_RESTORE		��Դ���ֵ���Ƶ�Ŀ�Ŀ�
//													SL3_TRANSFER	���������е����ݴ��䵽ָ���Ŀ�	
//				unsigned int uiSBNr				// ������Դ���ַ(ֻ�������ݿ�)
//				unsigned int uiDBNr				// ֵ������Ŀ�ؿ�(ֻ�������ݿ�)
//				long lValue						// �Ǹ����з�������
// ���ڲ���:    -
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ�
// ˵    ��:	ģʽΪSL3_RESTOREʱ��������lValue����Ч��
//				ģʽΪSL3_TRANSFERʱ��������lValue����Ч �͡�ucSBNr����Ч��
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall ZLG522S_SL3ValueOper(HANDLE hCtrl, unsigned char ucOperMode,  unsigned int uiSBNr, 
											 unsigned int uiDBNr, long lValue);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char ZLG522S_SL3ProximityCheck(HANDLE hCtrl, unsigned char ucKeyMode, 
//														const unsigned char *pProxCheckKey, 
//														unsigned int *pTime)
// ��������:    �м̹������
// �������:    HANDLE hCtrl					// ͨѶ����
//				unsigned char ucKeyMode			// 0 -- ʹ��PICC��֤ʱ�ĻỰKey, 
//												// 1 -- ʹ��ProximityCheckKey
//				unsigned char *pProxCheckKey	// ProximityCheckKey(��Կ��ַ:0xA001),����ucKeyMode=1ʱ��Ч
// �������:    unsigned int *pTime				// ����ִ��ʱ��(us)
// �� �� ֵ:    ����ִ�н��
// ˵    ��:	PPS1�̶�Ϊ0x00���ó���֧�ֱ䲨����
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall ZLG522S_SL3ProximityCheck(HANDLE hCtrl, unsigned char ucKeyMode, 
												  const unsigned char *pProxCheckKey, unsigned int *pTime);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char ZLG522S_SL3VCSupport(HANDLE hCtrl, const unsigned char *pIID)
// ��������:    ѡ��Ӧ��(���⿨)
// �������:    HANDLE hCtrl					// ͨѶ����
//				unsigned char *pIID				// Ӧ�ñ�ʶ(Installation Identifier,0xB001)
// �������:    -
// �� �� ֵ:    ����ִ�н��
// ˵    ��:	PLUS S��֧�ָ�����
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall ZLG522S_SL3VCSupport(HANDLE hCtrl, const unsigned char *pIID);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char ZLG522S_SL3VCSupportLast(HANDLE hCtrl, const unsigned char *pIID, 
//													   const unsigned char *pENCKey, 
//													   const unsigned char *pMACKey, unsigned char *pCardInfo)
// ��������:    ѡ��Ӧ��(���⿨������ʵ��UID����Ϣ)
// �������:    HANDLE hCtrl					// ͨѶ����
//				unsigned char *pIID				// Ӧ�ñ�ʶ(Installation Identifier,0xB001)
//				unsigned char *pENCKey			// ���ݼ�����Կ(��Կ��ַ:0xA080)
//				unsigned char *pMACKey			// MAC������Կ (��Կ��ַ:0xA081)
// �������:    unsigned char *pCardInfo		// ����16�ֽ����� ��ѡ��Ƭʹ��
// �� �� ֵ:    ����ִ�н��
// ˵    ��:	��IID�����ϣ��򷵻ص���ϢΪ16�ֽڵ����������IID��Ч������Ϣ����
//				pCardInfo[1:2] -- PiccCap2
//				pCardInfo[0] = 0x83		4�ֽڿ���	pCardInfo[3:6] -- 4�ֽڿ���
//				pCardInfo[0] = 0x03		7�ֽڿ���	pCardInfo[3:9] -- 7�ֽڿ���
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall ZLG522S_SL3VCSupportLast(HANDLE hCtrl, const unsigned char *pIID, 
												 const unsigned char *pENCKey, const unsigned char *pMACKey, 
												 unsigned char *pCardInfo);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char ZLG522S_SL3VCSelect(HANDLE hCtrl, unsigned char *pCardInfo, 
//												  unsigned char *pMACKey)
// ��������:    ѡ��(���⿨)
// �������:    HANDLE hCtrl					// ͨѶ����
//				unsigned char *pCardInfo		// SupportLast����16�ֽ����� ��ѡ��Ƭʹ��
//              unsigned char *pMACKey			// ����MAC����Կ(��Կ��ַ: 0xA000)
// �������:    -             
// �� �� ֵ:    ����ִ�н��	
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall ZLG522S_SL3VCSelect(HANDLE hCtrl, const unsigned char *pCardInfo, 
											const unsigned char *pMACKey);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char ZLG522S_SL3VCDeselect(HANDLE hCtrl)
// ��������:    ȡ�����⿨ѡ��
// �������:    HANDLE hCtrl					// ͨѶ����
// �������:    -             
// �� �� ֵ:    ����ִ�н��	
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall ZLG522S_SL3VCDeselect(HANDLE hCtrl);

//===========================================================================================================
#ifdef __cplusplus
	}
#endif

#endif				// __ZLG522S_DEV_CTRL_H
