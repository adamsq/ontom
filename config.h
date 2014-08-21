#ifndef CONFIG_INCLUDED_H_
#define CONFIG_INCLUDED_H_

//{{{{{{{{{{{{{
#define _DEBUG  1

// ��������ģ��ĵ���
#if _DEBUG >= 1
	#define CONFIG_DEBUG_CONFIG  1
#else
	#define CONFIG_DEBUG_CONFIG  0
#endif

//}}}}}}}}}}}}}

#ifndef bool
	#define bool unsigned char
#endif
#ifndef true
	#define true (bool)(1)
#endif
#ifndef false
	#define false (bool)(0)
#endif
#ifndef yes
	#define yes true
#endif
#ifndef no
	#define no false
#endif

// ��������ֵ��󳤶�Ϊ127
#define CONFIG_MAX_VALUE   128
struct config_struct {
	// ����������
	char *config_name;

	// ����������, 
	// string: �ַ���������
	#define C_STRING   0x00
	// int�� �������ݣ������ɸ�
	#define C_INT      0x01
	// float������������
	#define C_FLOAT    0x02
	// bool������������
	#define C_BOOL     0x03
	char config_type;

	// �����Ƿ����������ļ�
	// ��Щϵͳֵ��Ĭ�ϲ���Ҫ��������
	bool config_exsit;

	// ��������״̬,
	// 0 -- ��ʼ״̬����Ч
	#define C_INVALID   0
	// 1 -- ��Ч�������Ѿ����롢д����Ĭ��ֵ
	#define C_VALID     1
	// 2 -- �Ѿ��޸ģ�δ����
	#define C_MODIFY    2
	char config_status;

	// ��������ֵ
	union{
		float f; // float
		int   i; // int
		int   b; // bool
		int   n; // N/A
	}cuv;

	// ������ֵ��ͳһʹ���ַ��洢��ʹ��ʱ������ת��
	char config_value[CONFIG_MAX_VALUE];
};

/*�������ݳ�ʼ��, �������ļ�������������*/
int config_initlize(const char *cfgfile);
/*ͬ����������, ����������д�������ļ�*/
int config_writeout(const char *cfgfile);
/*��ȡ��������*/
const char *config_read(const char *name);
/*�޸���������, ֻ���޸�����ֵ�������޸���������.*/
const char *config_write(const char *name, const char *value);
#if CONFIG_DEBUG_CONFIG >= 1
/*��ӡ��������*/
void config_print();
#else
#define config_print
#endif /*CONFIG_DEBUG_CONFIG*/

#define CONFIG_DOMAIN_BEGIN static struct config_struct configs[]={
#define CONFIG_DOMAIN_END {"", 0, 0, 0, {0}} };

#endif /*CONFIG_INCLUDED_H_*/