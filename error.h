#ifndef _ERROR_H_
#define _ERROR_H_

// û�д���
#define ERR_OK    0x00000000
// �д���
#define ERR_ERR   0x80000000
// �������ʧ��
#define ERR_WRONG_PARAM  (ERR_ERR|0x00000001)
// ����ִ��ʧ��
#define ERR_FUN_FAILE    (ERR_ERR|0x00000002)

#endif /*_ERROR_H_*/