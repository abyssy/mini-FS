#ifndef STRUCTURE_H
#define STRUCTURE_H

#include<time.h>

const int BMmaxx = 32640;//32640*8Ϊ�ļ����ݿ���
const int FCB_COUNT = 49152; //�ļ����ƿ����

struct FCB {
	char Name[9];		//�ļ���
	char Exname[4];		//��չ��
	time_t filetime;	//����ʱ�� 8B
	int filesize;		//�ļ���С
	FCB *Fcb_next;		//ָ����һ���ļ����ƿ�
	FCB *Fcb_parent;	//ָ����һ���ļ����ƿ�
	int Filelocal[7];	//�ļ�����ռ�õ��������
};	//�ļ����ƿ� Ŀǰ64B

struct super_block {
	FILE *FS;						//ָ��1G�ռ����ʼλ��
	char FS_path[100];				//��¼�ռ�ľ���·��
	int flag;						//�ж��ļ�ϵͳ�Ƿ�ʹ�ù��������ʼ����
	int Fs_size;					//�ļ�ϵͳ��С
	int Block_size;					//���ݿ�Ĵ�С
	int Free_block_num;				//���п������ ��ʵʱ���ģ�
	unsigned int BM_first_free;		//ָ��λʾͼ�е�һ���հ׿�
	unsigned char Bitmap[BMmaxx];	//λʾͼ������鿴�ļ����ռ��/������� ��ʵʱ���ģ�
	FCB *Free_fcb;					//ָ���һ�������ļ����ƿ�
	int Free_fcb_num;				// ��ʵʱ���ģ�
	FCB *Full_fcb;					//ָ���һ��ռ���ļ����ƿ�
	int Full_fcb_num;				// ��ʵʱ���ģ�
};	//������



#endif //STRUCTURE_H