#��/bin/sh
echo "��ʼ��װ������....."
stty /dev/ttyO4 ispeed 19200 ospeed 19200
./uartwrite /dev/ttyO4 07 01 48 01 00 b0 03
echo "��������װ���"
