#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"../mini-FS/structure.h"
#include"../mini-FS/fuc_annou.h"


struct super_block sup_b;

int main(int argc, char *argv[])
{
	/*�ڴ��������ĳ������ʼ��*/
	sup_b.FS = NULL;
	strcpy(sup_b.FS_path, "d:\\"); //��Ϊ����\��ֻ�����һ��\�����ں��������а�\����
	sup_b.flag = 0;
	sup_b.Fs_size = 1024 * 1024 * 1024;
	sup_b.Block_size = 4 * 1024;
	sup_b.Free_block_num = 1024 / 4 * 1020;
	sup_b.BM_first_free = 0;
	for (int i = 0; i < BMmaxx; i++)
		sup_b.Bitmap[i] = 0;
	sup_b.Free_fcb = NULL;
	int Full_fcb_num = 0;
	int Free_fcb_num = 0;
	sup_b.Full_fcb = NULL;
	unsigned int BM_first_free = 0;
	/*�ڴ��������ĳ������ʼ��*/

	char order1[201];
	char order2[201];
	char order3[201];
	printf("�ļ�ϵͳ������\n");
	printf("--------------------\n");
	while (1) {
		memset(order1, 0, sizeof(order1));
		memset(order2, 0, sizeof(order2));
		memset(order3, 0, sizeof(order3));
		printf("\n###naokuoteng###>");
		if (order_input(order1, order2, order3) == -1)
			continue;
		if (strcmp(order1, "close") == 0)
		{
			if ((strlen(order2) != 0) || (strlen(order3) != 0))
			{
				printf("�����ʽ����,���������롣\n");
				continue;
			}
			else
				close();
			break;
		}
		deal(order1, order2, order3);
	}

	return 0;
}
/*
���ܣ�
create��Ϊmini-FS����1G�ռ�
mount����ʼ������ռ�
fmt����ʼ��
dr����ʾ�ļ�������չ�����
cp�������ļ�
dl��ɾ���ļ�
tp����ʾ�ı���������
more����ҳ��ʾ�ı�����
att����ʾ�ļ�����
help��������Ϣ
close���˳�ϵͳ
*/