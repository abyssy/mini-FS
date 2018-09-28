#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include"../mini-FS/structure.h"
#include"../mini-FS/fuc_annou.h"

void tp(char sFName[])
{
	extern struct super_block sup_b;
	//printf("%d %d\n", sup_b.Bitmap[0], sup_b.Bitmap[1]);
	FILE *fp = NULL;
	FCB * fcb = NULL;
	char name[9]; //文件名
	char exName[4]; //扩展名

	char new_name[9];//新文件名
	char new_exName[4];//新扩展名

	fcb = sup_b.Full_fcb;
	if (fcb == NULL)
	{
		printf("this file does not exist!!\n");
		return;
	}

	//处理文件名和扩展名
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
	while (1)
	{
		if (fcb == NULL)
			break;
		if ((strcmp(name, fcb->Name) == 0) && (strcmp(exName, fcb->Exname) == 0))//找到该文件，fcb指向了这个文件的FCB
		{
			flag = 1;
			break;
		}
		if (fcb->Fcb_next == NULL)
			break;
		fcb = fcb->Fcb_next;
	}
	if (flag == 0)
	{
		printf("the file does not exist!!\n");
		return;
	}
	/*找到文件后*/
	int s1_size = fcb->filesize;
	int free_b_need = s1_size / (4 * 1024);
	int left_b = s1_size % (4 * 1024);
	if (left_b > 0)
		free_b_need++;
	//文件s1大于28kb小于28mb再释放7个索引块
	if (s1_size > 28672 && s1_size <= 29360128)
		free_b_need += 7;
	/*判断文件大小结束*/

	i = 0, j = 0;
	int k = 0;
	if (s1_size <= 28672)
	{
		fp = fopen(sup_b.FS_path, "r");
		for (i = 0; i < free_b_need; i++)
		{
			char str[4096] = { 0 };
			if (i == free_b_need - 1)
			{
				fseek(fp, 4 * 1024 * 1024 + (fcb->Filelocal[i] - 1) * 4 * 1024, SEEK_SET);
				fread(str, sizeof(char), 4096, fp);
				int n = s1_size - i * 4 * 1024;
				str[n] = '\0';
				for(j = 0; j < n; j ++)
					printf("%c", str[j]);
				printf("\n");
			}
			else
			{
				fseek(fp, 4 * 1024 * 1024 + (fcb->Filelocal[i] - 1) * 4 * 1024, SEEK_SET);
				fread(str, sizeof(char), 4096, fp);
				for (j = 0; j < 4096; j ++)
					printf("%c", str[j]);
				printf("\n");
			}
		}
		fclose(fp);
	}
	else if (s1_size > 28672 && s1_size <= 29360128)
	{
		int cped_num = s1_size / (4 * 1024);
		int no_suoyin = cped_num / 1024;
		if ((cped_num % 1024) != 0)
			no_suoyin++;
		fp = fopen(sup_b.FS_path, "rb");
		for (i = 0; i < no_suoyin; i++)
		{
			char str[4096] = { 0 };
			int suoyin[1024] = { 0 };
			if ((i == no_suoyin - 1) && ((no_suoyin % 1024) != 0))
			{
				fseek(fp, 4 * 1024 * 1024 + (fcb->Filelocal[i] - 1) * 4 * 1024, SEEK_SET);
				fread(suoyin, sizeof(int), 1024, fp);
			}
			else
			{
				fseek(fp, 4 * 1024 * 1024 + (fcb->Filelocal[i] - 1) * 4 * 1024, SEEK_SET);
				fread(suoyin, sizeof(int), 1024, fp);
			}
			for (j = 0; j < 1024; j++)
			{
				if ((i * 1024 + j + 8) == free_b_need)
				{
					fseek(fp, 4 * 1024 * 1024 + (suoyin[j] - 1) * 4 * 1024, SEEK_SET);
					fread(str, sizeof(char), 4096, fp);
					int n = s1_size % (4 * 1024);
					str[n] = '\0';
					for (k = 0; k < n; k++)
						printf("%c", str[k]);
					printf("\n");
					j++;
					break;
				}
				else
				{
					fseek(fp, 4 * 1024 * 1024 + (suoyin[j] - 1) * 4 * 1024, SEEK_SET);
					fread(str, sizeof(char), 4096, fp);
					for (j = 0; j < 4096; j++)
						printf("%c", str[j]);
					printf("\n");
				}
			}
			if ((i * 1024 + j + 8) == free_b_need + 1)
				break;
		}
		fclose(fp);
	}
	return;
}