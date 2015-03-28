#!/bin/sh
SQLITE3
DB=ontom.dh

SYSTYPE=`$SQLITE3 $DB "SELECT current_value FROM settings WHERE key='system_type'"`
MODULE=`$SQLITE3 $DB SELECT current_value FROM settings WHERE key='module_kind'`

# ����ϵͳѡ�������Ƿ��г�������ͨ��ģ��
if [ $SYSTYPE == 0 ]; then
	# һ��ʽ
	echo "ϵͳ����: һ��ʽ����"
	echo "�رճ�������ù���...."
	$SQLITE3 $DB "UPDATE RS485_config set disable='true' where id='00000004'"
	$SQLITE3 $DB "UPDATE RS485_config set disable='true' where id='00000005'"
	$SQLITE3 $DB "UPDATE RS485_config set disable='true' where id='00000006'"
else
	# ����ʽ
	# Ŀǰֻ֧�ֹ�˾�Բ�ģ��ķ���ʽ���׮
	echo "ϵͳ����: ����ʽ����"
	echo "�򿪳�������ù���...."
	$SQLITE3 $DB "UPDATE RS485_config set disable='false' where id='00000004'"
	$SQLITE3 $DB "UPDATE RS485_config set disable='false' where id='00000005'"
	$SQLITE3 $DB "UPDATE RS485_config set disable='false' where id='00000006'"

	# ģ�����֧��AN10750
	echo "ϵͳǿ��ѡ��ģ��: AN10750"
	$SQLITE3 $DB "UPDATE settings set current_value='0' where key='module_kind'"
	
	echo "����ʽϵͳԤ�������."
fi

# �ٴ�ѡ��ϵͳ�����õ�ģ���ͺ�
MODULE=`$SQLITE3 $DB SELECT current_value FROM settings WHERE key='module_kind'`

# ����ģ���ͺ�ѡ��Ҫʹ�õĴ���ģ��
if [ $MODULE == 0 ];then
	# AN10750 ģ��
	echo "ϵͳ����AN10750ģ�飬ѡ�����Э��ת����.."
	$SQLITE3 $DB "UPDATE RS485_config set disable='false' where id='00000007'"
	$SQLITE3 $DB "UPDATE RS485_config set disable='false' where id='00000008'"
	
	echo "�رճ�ͻ�Ĵ���ģ��..."
	echo "   �ر�     Ӣ���ģ��Э��ת����ģ��"
	$SQLITE3 $DB "UPDATE RS485_config set disable='true' where id='00000009'"
	$SQLITE3 $DB "UPDATE RS485_config set disable='true' where id='0000000A'"
elif [ $MODULE == 4];then
	# Ӣ��� EVR400-7500
	echo "ϵͳ����Ӣ��� EVR400-7500ģ�飬ѡ��Э��ת����.."
	$SQLITE3 $DB "UPDATE RS485_config set disable='false' where id='00000009'"
	$SQLITE3 $DB "UPDATE RS485_config set disable='false' where id='0000000A'"
	
	echo "�رճ�ͻ�Ĵ���ģ��..."
	echo "   �ر�     ����Э��ת����ģ��"
	$SQLITE3 $DB "UPDATE RS485_config set disable='true' where id='00000007'"
	$SQLITE3 $DB "UPDATE RS485_config set disable='true' where id='00000008'"
else;
	echo "Ԥ����ϵͳ��������"
	exit 1;
fi

echo "ϵͳԤ�������..."
exit 0;