#!/bin/bash
echo "ֱ�����׮��װ����..."

echo "��װҳ���ļ�."
cp -R UI_html/* /install/srv/www/

echo "��װ�����ļ�."
cp ontom.db /install/usr/zeus/
cp ontom.cfg /install/usr/zeus/
cp user.cfg /install/usr/zeus/

echo "��װ����-sqlite3"
cp 

echo "��װ������-�����"
cp browser/browser /install/usr/zeus/
echo "��װ������-�����"
cp `readlink me` /install/usr/zeus/