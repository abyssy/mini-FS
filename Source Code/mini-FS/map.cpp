#include<stdio.h>
#include<string.h>
#include"../mini-FS/structure.h"
#include"../mini-FS/fuc_annou.h"

void map(char *sn)
{
	extern struct super_block sup_b;
	//printf("%d %d\n", sup_b.Bitmap[0], sup_b.Bitmap[1]);
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
	int i = 0;
	int sn_size = fcb->filesize;
	int free_b_need = sn_size / (4 * 1024);
	int left_B = sn_size % (4 * 1024);
	if (left_B > 0)
		free_b_need++;
	if (sn_size > 28672 && sn_size <= 29360128) //文件s1大于28KB小于28MB再加7个索引块
		free_b_need += 7;
	if (sn_size <= 28672)
	{
		printf("改文件占用的块号为：");
		for (i = 0; i < free_b_need; i++)
		{
			printf("%d ", fcb->Filelocal[i]);
		}
		printf("\n");
	}
	else if (sn_size > 28672 && sn_size <= 29360128)
	{
		int fneed = free_b_need - 7;
		int nol = fneed / 1024;
		int noly = fneed % 1024;
		if (noly > 0)
			nol++;
		int j = 0;
		FILE *fp = fopen(sup_b.FS_path, "rb");
		printf("改文件占用的块号为：");
		for (i = 0; i < 7; i++)
		{
			printf("%d ", fcb->Filelocal[i]);
		}
		for (i = 0; i < nol; i++)
		{
			int suoyin[1024] = { 0 };
			if (i == nol - 1)
			{
				fseek(fp, 4 * 1024 * 1024 + (fcb->Filelocal[i] - 1) * 4 * 1024, SEEK_SET);
				fread(suoyin, sizeof(int), 1024, fp);
				//printf("%d %d\n", sup_b.Bitmap[0], sup_b.Bitmap[1]);
				for (j = 0; j < fneed % 1024; j++)
					printf("%d ", suoyin[j]);
			}
			else
			{
				fseek(fp, 4 * 1024 * 1024 + (fcb->Filelocal[i] - 1) * 4 * 1024, SEEK_SET);
				fread(suoyin, sizeof(int), 1024, fp);
				for (j = 0; j < 1024; j++)
					printf("%d ", suoyin[j]);
			}
		}
		printf("\n");
		fclose(fp);
	}
}