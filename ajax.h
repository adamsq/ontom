#ifndef _AJAX_DEF_INCLUDED_H
#define _AJAX_DEF_INCLUDED_H

#define XML_IO_SIZE   4*1024  // 4K

struct ajax_xml_struct {
	// ��̬���ɵ�XML�ļ����ȣ� �Ϊ31�ֽ�
	char xml_name[32];
	// ���ɵ�XML�ļ�����
	unsigned int  xml_len;
	// XML�����������Ϊ4K
	char iobuff[XML_IO_SIZE];
};

// ����xml�ļ�
int ajax_gen_xml(struct ajax_xml_struct *thiz);

#endif /*_AJAX_DEF_INCLUDED_H*/