#!/bin/bash

prefix="install"
target="zeus.update.tar"
copydir="/media/sf_E_DRIVE/www/"
installdb="install.db"
VERSION=`svn info $0|grep Revision|grep -o '[0-9]\+'`

config='no'
drivers='no'
ext='no'
html='no'
library='no'
profile='no'
qtlib='no'
script='no'
zeus='no'

while getopts acdlpqstuvzhP:T:W:C: param; do
	# commands
	case "$param" in
	a|-all) # ��װȫ��
		config='yes'
		drivers='yes'
		ext='yes'
		html='yes'
		library='yes'
		profile='yes'
		qtlib='yes'
		script='yes'
		zeus='yes'
	;;
	c|-config)
		config='yes';
	;;
	d|-drivers) # ����װ����
		drivers='yes';
	;;
	e|-ext) # ����װ���
		ext='yes';
	;;
	l|-library) # ����װ���ļ�
		library='yes';
	;;
	q|-qtlib) # ����װQT���ļ�
		qtlib='yes';
	;;
	s|-script) # ����װ�ű�
		script='yes';
	;;
	t|-tools) # ��װ���߼��ű�
		tools='yes';
	;;
	u|-html) # ����װhtml�ļ�
		html='yes';
	;;
	u|-zeus) # ��װ����
		zeus='yes';
	;;
	v|-version)
		echo "installer of zeus version: $VERSION"
		exit 0;
	;;
	h|-help)
		echo -e "���׮���ϵͳ�ļ���װ����\r\n"\
			"Usage:\r\n"\
			"    install {COMMANDS}... {OPTIONS}...\r\n"\
			"COMMANDS:\r\n"\
			"	-a ==> all: ��װȫ��ϵͳ�ļ�(�������⣬UI�������, ����);\r\n"\
			"	-c ==> config: ��װ�����ļ����������ݿ�;\r\n"\
			"	-d ==> drivers: ��װBMS��������;\r\n"\
			"	-l ==> library: ��װϵͳ���ļ�\r\n"\
			"	-p ==> plugins: ��װϵͳ���\r\n"\
			"	-q ==> qtlibrary: ��װQT���ļ�;\r\n"\
			"	-t ==> tools: ��װ�ű����߼�;\r\n"\
			"	-u ==> html: ��װ��ʾ��html�ļ�;\r\n"\
			"	-h ==> help: ��ʾ����������Ϣ;\r\n"\
			"	-v ==> version: ��ʾ��װ���İ汾\r\n"\
			"OPTIONS:\r\n"\
			"	-C ==> copydir: ָ�����Ŀ¼(Ĭ��: $copydir)\r\n"\
			"	-P ==> prefix: ָ����װĿ¼(Ĭ��: `pwd`/install/);\r\n"\
			"	-T ==> target: ָ�����Ŀ���ļ���(Ĭ��: zeus.update.tar);\r\n"\
			"	-W ==> workdir: ָ����ǰ����Ŀ¼(Ĭ��: `pwd`)\r\n"\
			"AUTHOR:\r\n"\
			"	LiJie <cuplision@163.com> 2015/05/05 09:00"
		exit 0;
	;;
	# options
	P|-prefix)
		prefix=$OPTARG
	;;
	T|-target)
		target=$OPTARG
	;;
	W|-workdir)
		WORKDIR=$OPTARG
	;;
	C|-copydir)
		copydir=$OPTARG
	;;
	*)
		echo "�޷�ʶ��Ĳ��� $param=$OPTARG, ʹ��install -h �鿴����."
		exit 1;
	;;
	esac
done
shift $(( OPTIND - 1 ));

if [ ${#WORKDIR} -eq 0 ];then
	echo "û���ҵ���������WORKDIR��ʹ��Ĭ��Ŀ¼`pwd`, ��ʹ��-Wָ������Ŀ¼"
	WORKDIR=`pwd`
fi

if [ $config == "yes" ];then
	P=`sqlite3 $(installdb) "SELECT path FROM dirs WHERE class LIKE '%config%'"`
	if [ ${#P} -eq 0 ];then
		echo "û���ҵ���Ҫ��װ�������ļ�Ŀ¼, ����."
	else
		for d in $P;do
			printf "����Ŀ¼ $d"
			mkdir -p $prefix/$d
			if [ $? -eq 0 ];then
				echo "   �ɹ�."
			else
				echo "   ʧ�� ($?) !"
			fi
		done
		Fsrc=`sqlite3 $(installdb) "SELECT src FROM files WHERE class LIKE '%config%'"`
	fi
fi
