#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "ajax.h"
#include "error.h"

int ajax_version_xml_proc(struct ajax_xml_struct *);

struct xml_generator {
	// xml �ļ���
	char * xml_name;
	// xml ���ɹ��̵�ַ
	int (*xml_gen_proc)(struct ajax_xml_struct *);
}xmls[]={
	{"version.xml", ajax_version_xml_proc},
	{"", NULL}
};

// ����xml�ļ�
int ajax_gen_xml(struct ajax_xml_struct *thiz)
{
	if ( thiz == NULL ) return ERR_WRONG_PARAM;
	else {
		struct xml_generator *cursor = xmls;

		for ( ; xmls->xml_gen_proc ; ) {
			if ( ! strstr(thiz->xml_name, xmls->xml_name) ) continue;
			return xmls->xml_gen_proc(thiz);
		}

		return ERR_FUN_FAILE;
	}
}

// ����version.xml
int ajax_version_xml_proc(struct ajax_xml_struct *thiz)
{
	return ERR_OK;
}