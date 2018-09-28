#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"../mini-FS/structure.h"
#include"../mini-FS/fuc_annou.h"

void delete_new(char sFName[])
{
	extern struct super_block sup_b;
	FILE *fp = NULL;
	FCB * fcb = NULL;
	char name[9]; //文件名
	char exName[4]; //扩展名

	fcb = sup_b.Full_fcb;
	if (fcb == NULL)
	{
		printf("this file does not exist!!\n");
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
	FCB * fcbPa = NULL;

	while (1)
	{
		//在目录中寻找文件
		if (fcb == NULL)
			break;
		if ((strcmp(name, fcb->Name) == 0) && (strcmp(exName, fcb->Exname) == 0))
		{
			//文件存在
			flag = 1;

			//确认是否删除
			printf("Are you sure to delete this file? Please enter Y/N\n");
			char sure;
			scanf("%c%*c", &sure);
			if (sure == 'Y' || sure == 'y')
				;
			else
			{
				flag = 2;
				break;
			}
			//从目录中删掉文件
			if (fcb->Fcb_parent != NULL)
			{
				//如果该目录前还有目录
				fcbPa = fcb->Fcb_parent;
				fcbPa->Fcb_next = fcb->Fcb_next;
				if (fcb->Fcb_next != NULL)
				{
					//如果该目录后还有目录
					fcb->Fcb_next->Fcb_parent = fcbPa;
				}
			}
			else
			{
				//如果该目录上一个是sup_b.Full_fcb
				sup_b.Full_fcb = fcb->Fcb_next;
				if (fcb->Fcb_next != NULL)
				{
					//如果该目录后还有目录
					fcb->Fcb_next->Fcb_parent = NULL;
				}
			}
			//移动到free_fcb链表中
			fcb->Fcb_next = sup_b.Free_fcb;
			fcb->Fcb_parent = NULL;
			sup_b.Free_fcb = fcb;
			if (fcb->Fcb_next != NULL)
			{
				fcb->Fcb_next->Fcb_parent = fcb;
			}

			sup_b.Full_fcb_num--;
			sup_b.Free_fcb_num ++; //1.注释掉这一行 //2.添加回来了这一行

			//下面的代码移动到了empty_bin里，只有清空回收站的时候才会把磁盘剩余空间增加
			//sup_b.Free_block_num += fcb->filesize / (4 * 1024);
			//if (fcb->filesize % (4 * 1024) != 0)
				//sup_b.Free_block_num ++;

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


	if (flag == 0) //文件不存在
		printf("this file does not exist!!\n");
	else if (flag == 1) //删除成功
		printf("delete file successfully!!\n");
	else if (flag == 2) //误删
		printf("your file still exists!!\n");

	return;
}