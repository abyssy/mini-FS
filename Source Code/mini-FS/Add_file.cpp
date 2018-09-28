#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include"../mini-FS/structure.h"
#include"../mini-FS/fuc_annou.h"

void add(char *sn)
{
	extern struct super_block sup_b;
	char pname[9] = { 0 };
	char pexname[4] = { 0 };
	int ii = 0, jj = 0;
	for (ii = 0; ii < strlen(sn); ii++)
	{
		if (sn[ii] == '.')
			break;
		pname[ii] = sn[ii];
	}
	pname[ii] = '\0';
	ii++;
	for (jj = ii; jj < strlen(sn); jj++)
	{
		pexname[jj - ii] = sn[jj];
	}
	pexname[jj - ii] = '\0';
	/*名字复制*/

	FCB *fcb = sup_b.Full_fcb;
	while (1)
	{
		//在目录中寻找是否存在创造后同名的文件
		if (fcb == NULL)
			break;
		if ((strcmp(pname, fcb->Name) == 0) && (strcmp(pexname, fcb->Exname) == 0))
		{
			printf("the file name is used! please delete origin file or change the file name.\n");
			return;
		}
		if (fcb->Fcb_next == NULL)
			break;
		fcb = fcb->Fcb_next;
	}

	fcb = sup_b.Full_fcb;
	while (1)
	{
		if (fcb == NULL)
			break;
		if (fcb->Fcb_next == NULL)
			break;
		fcb = fcb->Fcb_next;
	}//fcb指向新建文件的FCB的上一个FCB
	if (fcb == NULL)
	{
		sup_b.Full_fcb = (FCB *)malloc(sizeof(FCB) * 1);
		strcpy(sup_b.Full_fcb->Name, pname);
		strcpy(sup_b.Full_fcb->Exname, pexname);
		time(&(sup_b.Full_fcb->filetime));
		sup_b.Full_fcb->filesize = 1;
		sup_b.Full_fcb->Fcb_next = NULL;
		sup_b.Full_fcb->Fcb_parent = NULL;
		memset(sup_b.Full_fcb->Filelocal, 0, sizeof(int));
		fcb = sup_b.Full_fcb;
	}
	else
	{
		fcb->Fcb_next = (FCB *)malloc(sizeof(FCB) * 1);
		strcpy(fcb->Fcb_next->Name, pname);
		strcpy(fcb->Fcb_next->Exname, pexname);
		time(&(fcb->Fcb_next->filetime));
		fcb->Fcb_next->filesize = 1;
		fcb->Fcb_next->Fcb_next = NULL;
		fcb->Fcb_next->Fcb_parent = fcb;
		fcb = fcb->Fcb_next;
		memset(fcb->Filelocal, 0, sizeof(int));
	}
	sup_b.Full_fcb_num++;
	int b_no = 0, k = 0, i = 0, j = 0;
	for (i = 0; i < 32640; i++) //搜索Bitmap查找空白块
	{
		if (sup_b.Bitmap[i] == 255) //Bitmap该字节代表的文件块都已被占用
			continue;
		for (j = 7; j >= 0; j--)
		{
			if ((((sup_b.Bitmap[i] >> j) | 0) % 2) == 0) //unsigned char右移后左侧补全0
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
	sup_b.Free_block_num -= 1;
	fcb->Filelocal[0] = b_no;
	printf("现在需要编辑这个文件吗？\n[y/n]");
	char ch = 0;
	scanf("%c", &ch);
	getchar();
	if (ch == 'Y' || ch == 'y')
		rewrite(sn);
}