#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"../mini-FS/structure.h"
#include"../mini-FS/fuc_annou.h"

void rst(char sFName[])
{
	extern struct super_block sup_b;
	FILE *fp = NULL;
	FCB * fcb = NULL;
	char name[9]; //�ļ���
	char exName[4]; //��չ��

	fcb = sup_b.Free_fcb;
	if (fcb == NULL)
	{
		printf("the file that needs to be restored does not exist!!\n");
		return;
	}


	/*****************************/
	/*****************************/
	/*****************************/
	/*
	printf("bitmap:\n");
	for (i = 0; i < 10; i++)
	printf("%d ", sup_b.Bitmap[i]);
	printf("\n");
	*/
	/*****************************/
	/*****************************/
	/*****************************/


	//�����ļ�������չ��
	int i = 0;
	int j = 0;
	int flag = 0;
	for (i = 0; sFName[i] != '\0'; i++)
	{
		if (sFName[i] == '.')
		{
			flag = 1;
			name[j] = '\0';
			j = 0;
			continue;
		}
		if (flag == 0)
		{
			name[j] = sFName[i];
			j++;
		}
		else if (flag == 1)
		{
			exName[j] = sFName[i];
			j++;
		}
	}
	exName[j] = '\0';

	flag = 0;
	FCB * fcbPa = NULL;

	while (1)
	{
		//��ɾ��Ŀ¼��Ѱ���ļ�
		if (fcb == NULL)
			break;
		if ((strcmp(name, fcb->Name) == 0) && (strcmp(exName, fcb->Exname) == 0))
		{
			//�ļ�����
			flag = 1;

			//��ɾ��Ŀ¼��ɾ���ļ�
			if (fcb->Fcb_parent != NULL)
			{
				//�����ɾ��Ŀ¼ǰ����ɾ��Ŀ¼
				fcbPa = fcb->Fcb_parent;
				fcbPa->Fcb_next = fcb->Fcb_next;
				if (fcb->Fcb_next != NULL)
				{
					//�����ɾ��Ŀ¼����ɾ��Ŀ¼
					fcb->Fcb_next->Fcb_parent = fcbPa;
				}
			}
			else
			{
				//�����ɾ��Ŀ¼��һ����sup_b.Free_fcb
				sup_b.Free_fcb = fcb->Fcb_next;
				if (fcb->Fcb_next != NULL)
				{
					//�����ɾ��Ŀ¼����Ŀ¼
					fcb->Fcb_next->Fcb_parent = NULL;
				}
			}
			//�ƶ���Full_fcb������
			fcb->Fcb_next = sup_b.Full_fcb;
			fcb->Fcb_parent = NULL;
			sup_b.Full_fcb = fcb;
			if (fcb->Fcb_next != NULL)
			{
				fcb->Fcb_next->Fcb_parent = fcb;
			}

			sup_b.Full_fcb_num ++;
			sup_b.Free_fcb_num --;
			sup_b.Free_block_num -= fcb->filesize / (4 * 1024);
			if (fcb->filesize % (4 * 1024) != 0)
				sup_b.Free_block_num --;
			break;
		}
		if (fcb->Fcb_next == NULL)
			break;
		fcb = fcb->Fcb_next;
	}

	/*****************************/
	/*****************************/
	/*****************************/
	/*
	printf("bitmap:\n");
	for (i = 0; i < 10; i++)
		printf("%d ", sup_b.Bitmap[i]);
	printf("\n");
	*/
	/*****************************/
	/*****************************/
	/*****************************/


	if (flag == 0) //�ļ�������
		printf("the file that needs to be restored does not exist!!\n");
	else if (flag == 1) //�ָ��ɹ�
		printf("restore file successfully!!\n");

	return;
}