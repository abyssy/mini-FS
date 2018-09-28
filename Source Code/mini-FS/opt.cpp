#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include"../mini-FS/structure.h"
#include"../mini-FS/fuc_annou.h"

void opt(char *sn)
{
	extern struct super_block sup_b;
	if (sup_b.Full_fcb == NULL)
	{
		printf("不存在该文件!\n");
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
	/*复制文件名称完成*/
	FCB *fcb = sup_b.Full_fcb;
	int flag1 = 0;
	for (int i = 0; i < sup_b.Full_fcb_num; i++)
	{
		if ((strcmp(name, fcb->Name) == 0) && (strcmp(exname, fcb->Exname) == 0)) //fcb指向该文件的FCB
		{
			flag1 = 1;
			break;
		}
		fcb = fcb->Fcb_next;
	}
	if (flag1 == 0)
	{
		printf("不存在该文件!\n");
		return;
	}
	/*找到对应FCB*/
	int sn_size = fcb->filesize;
	int free_b_need = sn_size / (4 * 1024);
	int left_B = sn_size % (4 * 1024);
	if (left_B > 0)
		free_b_need++;
	if (sn_size > 28672 && sn_size <= 29360128) //文件s1大于28KB小于28MB再加7个索引块
		free_b_need += 7;
	int *ofile = (int *)malloc(sizeof(int) * free_b_need);
	int i = 0;
	for (i = 0; i < free_b_need; i++)
		ofile[i] = fcb->Filelocal[i];
	for (i = 0; i < free_b_need; i++)
	{
		int cha = abs(ofile[i] - ofile[i + 1]);
		if (cha > 1)
		{
			i++;
			break;
		}
	}
	int *exch = NULL;
	int cha = ofile[i] - ofile[i - 1] - 1;
	if (cha >= i)
	{
		exch = (int *)malloc(sizeof(int) * i);
		int ii = 0;
		memset(exch, 0, sizeof(int));
		for (ii = 0; ii < i; ii++)
			exch[ii] = ofile[i] - ii - 1;
		FCB *ffcb = sup_b.Full_fcb;
		int ff = 0, j = 0;
		FILE *fp = fopen(sup_b.FS_path, "rb+");
		while (1)
		{
			if (j == i)
			{
				break;
			}
			if (ff > i)
			{
				printf("opt操作条件不满足\n");
				return;
			}
			if (ffcb == NULL)
			{
				ffcb = sup_b.Full_fcb;
				ff++;
			}
			for (ii = 0; ii < 7; ii++)
			{
				if (ffcb->Filelocal[ii] == exch[j])
				{
					char str[4096], strr[4096];
					int temp = 0;
					fseek(fp, 4 * 1024 * 1024 + (ffcb->Filelocal[ii] - 1) * 4 * 1024, SEEK_SET);
					fread(str, sizeof(char), 4096, fp);
					fseek(fp, 4 * 1024 * 1024 + (fcb->Filelocal[j] - 1) * 4 * 1024, SEEK_SET);
					fread(strr, sizeof(char), 4096, fp);
					fseek(fp, 4 * 1024 * 1024 + (ffcb->Filelocal[ii] - 1) * 4 * 1024, SEEK_SET);
					fwrite(strr, sizeof(char), 4096, fp);
					fseek(fp, 0, SEEK_CUR);
					temp = fcb->Filelocal[j];
					fcb->Filelocal[j] = ffcb->Filelocal[ii];
					fseek(fp, 4 * 1024 * 1024 + (temp - 1) * 4 * 1024, SEEK_SET);
					fwrite(str, sizeof(char), 4096, fp);
					fseek(fp, 0, SEEK_CUR);
					ffcb->Filelocal[ii] = temp;
					j++;
					if (j == cha)
					{
						break;
					}
				}
			}
			ffcb = ffcb->Fcb_next;
		}
		fclose(fp);
	}
	else
	{
		exch = (int *)malloc(sizeof(int) * i);
		int ii = 0;
		memset(exch, 0, sizeof(int));
		for (ii = 0; ii < cha; ii++)
			exch[ii] = ofile[i] - ii - 1;
		FCB *ffcb = sup_b.Full_fcb;
		int ff = 0, j = 0;
		FILE *fp = fopen(sup_b.FS_path, "rb+");
		while (1)
		{
			if (j == cha)
			{
				break;
			}
			if (ff > cha)
			{
				printf("opt操作条件不满足\n");
				return;
			}
			if (ffcb == NULL)
			{
				ffcb = sup_b.Full_fcb;
				ff++;
			}
			for (ii = 0; ii < 7; ii++)
			{
				if (ffcb->Filelocal[ii] == exch[j])
				{
					char str[4096], strr[4096];
					int temp = 0;
					fseek(fp, 4 * 1024 * 1024 + (ffcb->Filelocal[ii] - 1) * 4 * 1024, SEEK_SET);
					fread(str, sizeof(char), 4096, fp);
					fseek(fp, 4 * 1024 * 1024 + (fcb->Filelocal[j] - 1) * 4 * 1024, SEEK_SET);
					fread(strr, sizeof(char), 4096, fp);
					fseek(fp, 4 * 1024 * 1024 + (ffcb->Filelocal[ii] - 1) * 4 * 1024, SEEK_SET);
					fwrite(strr, sizeof(char), 4096, fp);
					fseek(fp, 0, SEEK_CUR);
					temp = fcb->Filelocal[j];
					fcb->Filelocal[j] = ffcb->Filelocal[ii];
					fseek(fp, 4 * 1024 * 1024 + (temp - 1) * 4 * 1024, SEEK_SET);
					fwrite(str, sizeof(char), 4096, fp);
					fseek(fp, 0, SEEK_CUR);
					ffcb->Filelocal[ii] = temp;
					j++;
					if (j == cha)
					{
						break;
					}
				}
			}
			ffcb = ffcb->Fcb_next;
		}
		fclose(fp);
	}
}