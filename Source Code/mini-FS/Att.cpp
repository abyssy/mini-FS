#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include"../mini-FS/structure.h"
#include"../mini-FS/fuc_annou.h"

void Att(char *fileName)//此处fileName为order2
{
	extern struct super_block sup_b;
	FCB *fcb = sup_b.Full_fcb;//记录文件控制块的指针
	char newName[9];		//fileName的文件名
	char newExname[4];		//fileName的拓展名
	int i, j = 0, flag = 0;

/********************处理fileName**************/
	for(i = 0; fileName[i] != '\0'; i++)
	{
		if(fileName[i] == '.')
		{
			flag = 1;
			newName[j] = '\0';
			j = 0;
			continue;
		}
		if(flag == 0)
		{
			newName[j++] = fileName[i];
			continue;
		}
		if(flag == 1)
		{
			newExname[j++] = fileName[i];
			continue;
		}
	}
	newExname[j] = '\0';
/********************处理fileName**************/

/********************遍历FCB**************/
	if (fcb == NULL)
	{
		printf("该文件不存在\n");
		return;
	}
	int flag1 = 0;//标志是否查找到该文件
	while(1)
	{
		if((strcmp(newName, fcb->Name) == 0)&&(strcmp(newExname, fcb->Exname) == 0))//找到该文件
		{
			printf("Name:          %s\n", fcb->Name);
			printf("Exname:        %s\n", fcb->Exname);
			printf("Time:          ");
			struct tm *p;
			p = gmtime(&(fcb->filetime));
			printf("%d年%d月%d日%d时%d分%d秒\n", 1900 + p->tm_year, 1 + p->tm_mon, p->tm_mday, 8 + p->tm_hour, p->tm_min, p->tm_sec);

			printf("filesize:      %d\n", fcb->filesize);
			flag1 = 1;
			break;
		}
		if(fcb->Fcb_next == NULL)	break;
		fcb = fcb->Fcb_next;
	}
/********************遍历FCB**************/
	if(flag1 == 0)	printf("该文件不存在\n");
	return;
}