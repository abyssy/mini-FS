#include<stdio.h>
#include<string.h>
#include"../mini-FS/structure.h"
#include"../mini-FS/fuc_annou.h"

void rewrite(char *sn)
{
	extern struct super_block sup_b;
	if (sup_b.Full_fcb == NULL)
	{
		printf("�����ڸ��ļ�!\n");
		return;
	}
	char name[9] = { 0 };
	char exname[4] = { 0 };
	int ii = 0, jj = 0;
	for (ii = 0; ii < strlen(sn); ii++)
	{
		if (sn[ii] == '.')
			break;
		name[ii] = sn[ii];
	}
	name[ii] = '\0';
	ii++;
	for (jj = ii; jj < strlen(sn); jj++)
	{
		exname[jj - ii] = sn[jj];
	}
	exname[jj - ii] = '\0';
	/*�����ļ��������*/
	FCB *fcb = sup_b.Full_fcb;
	int flag1 = 0;
	for (int i = 0; i < sup_b.Full_fcb_num; i++)
	{
		if ((strcmp(name, fcb->Name) == 0) && (strcmp(exname, fcb->Exname) == 0)) //fcbָ����ļ���FCB
		{
			flag1 = 1;
			break;
		}
		fcb = fcb->Fcb_next;
	}
	if (flag1 == 0)
	{
		printf("�����ڸ��ļ�!\n");
		return;
	}
	/*�ҵ���ӦFCB*/
	printf("���ļ���СΪ%d.\n", fcb->filesize);
	int sn_size = fcb->filesize;
	int free_b_need = sn_size / (4 * 1024);
	int left_B = sn_size % (4 * 1024);
	if (left_B > 0)
		free_b_need++;
	if (sn_size > 28672 && sn_size <= 29360128) //�ļ�s1����28KBС��28MB�ټ�7��������
		free_b_need += 7;
	if (sn_size < 28672)
	{
		int filelocal[7] = { 0 };
		int i = 0;
		for (i = 0; i < free_b_need; i++)
			filelocal[i] = fcb->Filelocal[i];
		printf("����Ҫ�Ӹ��ļ���һ���ַ�֮��ʼ�޸ģ�\n");
		int sta = 0;
		scanf("%d", &sta);
		getchar();
		printf("�������ı���\n");
		int Nob = sta / (4 * 1024);
		int re = sta % (4 * 1024);
		if (re > 0 || sta == 0)
			Nob++;
		FILE *fp = fopen(sup_b.FS_path, "rb+");
		fseek(fp, 4 * 1024 * 1024 + (fcb->Filelocal[Nob - 1] - 1) * 1024 * 4 + re, SEEK_SET);
		char str[4096] = { 0 };
		scanf("%s", str);
		getchar();
		if (sta - (Nob - 1) * 4 * 1024 + strlen(str) <= 4096)
		{
			//printf("len = %d\n", strlen(str));
			//printf("%s\n", str);
			fwrite(str, sizeof(char), strlen(str), fp);
			fseek(fp, 0, SEEK_CUR);
		}
		else //������ǡ�ÿ�Խ28KB�Ľ���
		{
			char str1[4096] = { 0 };
			for (i = 0; i < 4096 - re; i++)
				str1[i] = str[i];
			fwrite(str, sizeof(char), strlen(str), fp);
			fseek(fp, 0, SEEK_CUR);
			memset(str1, 0, sizeof(str1));
			for (; i < strlen(str); i++)
				str1[i - (4096 - re)] = str[i];
			/*�ڶ��θ������*/
			int b_no = 0, k = 0, j = 0;
			for (i = 0; i < 32640; i++) //����Bitmap���ҿհ׿�
			{
				if (sup_b.Bitmap[i] == 255) //Bitmap���ֽڴ�����ļ��鶼�ѱ�ռ��
					continue;
				for (j = 7; j >= 0; j--)
				{
					if ((((sup_b.Bitmap[i] >> j) | 0) % 2) == 0) //unsigned char���ƺ���ಹȫ0
					{
						b_no = i * 8 + 8 - j;
						k++;
						sup_b.Bitmap[i] = sup_b.Bitmap[i] ^ (1 << j);
						if (sup_b.Bitmap[i] == 255)
							break;
						if (k == 1)
							break;
					}
				}
				if (k == 1)
					break;
			}

			fcb->Filelocal[free_b_need] = b_no;
			fwrite(str, sizeof(char), strlen(str) - (4096 - re), fp);
			fseek(fp, 0, SEEK_CUR);
		}
		if (sta + strlen(str) > fcb->filesize)
			fcb->filesize = strlen(str) + sta;
		fclose(fp);
	}
	else if (sn_size > 28672 && sn_size <= 29360128)
	{
		printf("����Ҫ�Ӹ��ļ���һ���ַ�֮��ʼ�޸ģ�\n");
		int sta = 0;
		scanf("%d", &sta);
		getchar();
		printf("�������ı���\n");
		int Nos = sta / (4 * 1024 * 1024);
		if (sta % (4 * 1024 * 1024) || sta == 0)
			Nos++;
		int Nob = (sta - (Nos - 1) * 4 * 1024 * 1024) / (4 * 1024);
		int re = (sta - (Nos - 1) * 4 * 1024 * 1024) % (4 * 1024);
		if (re > 0 || sta == 0)
			Nob++;
		int blo[1024] = { 0 };
		char str[4096] = { 0 };
		scanf("%s", str);
		getchar();
		FILE *fp = fopen(sup_b.FS_path, "rb+");
		fseek(fp, 4 * 1024 * 1024 + (fcb->Filelocal[Nos - 1] - 1) * 4 * 1024, SEEK_SET);
		fread(blo, sizeof(int), 1024, fp);
		fseek(fp, 4 * 1024 * 1024 + (blo[Nob - 1] - 1) * 4 * 1024 + re, SEEK_SET);
		fwrite(str, sizeof(char), strlen(str), fp);
		fclose(fp);
	}
}