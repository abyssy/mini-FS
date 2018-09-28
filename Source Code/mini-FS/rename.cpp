#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include"../mini-FS/structure.h"
#include"../mini-FS/fuc_annou.h"

void rename(char sFName[], char new_sFName[])
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

	//处理新文件名和扩展名
	i = 0;
	j = 0;
	flag = 0;
	for (i = 0; new_sFName[i] != '\0'; i++)
	{
		if (new_sFName[i] == '.')
		{
			flag = 1;
			new_name[j] = '\0';
			j = 0;
			continue;
		}
		if (flag == 0)
		{
			new_name[j] = new_sFName[i];
			j++;
		}
		else if (flag == 1)
		{
			new_exName[j] = new_sFName[i];
			j++;
		}
	}
	new_exName[j] = '\0';

	flag = 0;
	FCB * fcbPa = NULL;

	int tmp, zijie, wei, cnt;
	int addr[1024];

	while (1)
	{
		//在目录中寻找是否存在改名后同名的文件
		if (fcb == NULL)
			break;
		if ((strcmp(new_name, fcb->Name) == 0) && (strcmp(new_exName, fcb->Exname) == 0))
		{
			printf("the file name is used! please delete origin file or change the file name.\n");
			return;
		}
		if (fcb->Fcb_next == NULL)
			break;
		fcb = fcb->Fcb_next;
	}

	flag = 0;
	fcb = sup_b.Full_fcb;
	while (1)
	{
		//在目录中寻找文件
		if (fcb == NULL)
			break;
		if ((strcmp(name, fcb->Name) == 0) && (strcmp(exName, fcb->Exname) == 0))
		{
			//文件存在
			flag = 1;
			strcpy(fcb->Name, new_name);
			strcpy(fcb->Exname, new_exName);
			printf("rename file successfully!!\n");
			break;
		}
		if (fcb->Fcb_next == NULL)
			break;
		fcb = fcb->Fcb_next;
	}
	if (flag == 0) //文件不存在
		printf("this file does not exist!!\n");

	return;
}