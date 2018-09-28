#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include"../mini-FS/structure.h"
#include"../mini-FS/fuc_annou.h"

void cp(char *s1, char *s2)
{
	extern struct super_block sup_b;
	//printf("%d %d\n", sup_b.Bitmap[0], sup_b.Bitmap[1]);
	char pname[9] = { 0 };
	char pexname[4] = { 0 };
	int ii = 0, jj = 0;
	for (ii = 0; ii < strlen(s1); ii++)
	{
		if (s1[ii] == '.')
			break;
		pname[ii] = s1[ii];
	}
	pname[ii] = '\0';
	ii++;
	for (jj = ii; jj < strlen(s1); jj++)
	{
		pexname[jj - ii] = s1[jj];
	}
	pexname[jj - ii] = '\0';
	/*复制母文件名称完成*/
	printf("请选择拷贝方式：\n从windows中拷入mini-FS为 1\n从mini-FS拷入mini-FS为 2\n从mini - FS拷入windows为 3\n");
	printf("[1/2/3]   ");
	int flag = 0;
	scanf("%d", &flag);
	getchar();
	printf("\n");
	if (flag == 1)
	{
		FILE *ff = NULL, *fp = NULL;
		if (strcmp(pexname, "jpg") == 0 || strcmp(pexname, "png") == 0)
			ff = fopen(s1, "rb");
		else
			ff = fopen(s1, "r");
		if (ff == NULL)
		{
			printf("文件路径出错！\n");
			return;
		}
		fseek(ff, 0, SEEK_END);
		int s1_size = ftell(ff); //ftell返回文件的字节数
		fclose(ff);
		int free_b_need = s1_size / (4 * 1024);
		int left_B = s1_size % (4 * 1024);
		if (left_B > 0)
			free_b_need++;
		if (s1_size > 28672 && s1_size <= 29360128) //文件s1大于28KB小于28MB再加7个索引块
			free_b_need += 7;
		if (s1_size > 29360128) //文件s1大于28MB再加7175块索引块
		{
			printf("File is too big!\n");
			//free_b_need += 7175;
			return;
		}
		if (free_b_need > sup_b.Free_block_num)
		{
			printf("文件过大\n");
			return;
		}
		FCB *nfcb = sup_b.Full_fcb;
		if (nfcb == NULL) //拷入文件前，系统没有文件
		{
			nfcb = (FCB *)malloc(sizeof(FCB));
			sup_b.Full_fcb = nfcb;
			/*设置名称开始*/
			int i = 0, j = 0;
			for (i = 0; i < strlen(s2); i++)
			{
				if (s2[i] == '.')
					break;
				nfcb->Name[i] = s2[i];
			}
			nfcb->Name[i] = '\0';
			i++;
			for (j = i; j < strlen(s2); j++)
			{
				nfcb->Exname[j - i] = s2[j];
			}
			nfcb->Exname[j - i] = '\0';
			/*设置名称结束*/
			time(&(nfcb->filetime));
			nfcb->filesize = s1_size;
			nfcb->Fcb_next = NULL;
			nfcb->Fcb_parent = NULL;
			/*FCB信息初始化完成，只剩filelocal*/
			sup_b.Full_fcb_num++;
		}
		else //已有文件
		{
			for (int i = 1; i < sup_b.Full_fcb_num; i++)
				nfcb = nfcb->Fcb_next;
			nfcb->Fcb_next = (FCB *)malloc(sizeof(FCB));
			/*设置名称开始*/
			int i = 0, j = 0;
			for (i = 0; i < strlen(s2); i++)
			{
				if (s2[i] == '.')
					break;
				nfcb->Fcb_next->Name[i] = s2[i];
			}
			nfcb->Fcb_next->Name[i] = '\0';
			i++;
			for (j = i; j < strlen(s2); j++)
			{
				nfcb->Fcb_next->Exname[j - i] = s2[j];
			}
			nfcb->Fcb_next->Exname[j - i] = '\0';
			/*设置名称结束*/
			time(&(nfcb->Fcb_next->filetime));
			nfcb->Fcb_next->filesize = s1_size;
			nfcb->Fcb_next->Fcb_next = NULL;
			nfcb->Fcb_next->Fcb_parent = nfcb;
			/*FCB信息初始化完成，只剩filelocal*/
			nfcb = nfcb->Fcb_next;
			sup_b.Full_fcb_num++;
		}
		int *b_no = NULL;
		b_no = (int *)malloc(sizeof(int) * free_b_need);
		int i = 0, j = 0, k = 0;
		for (i = 0; i < 32640; i++) //搜索Bitmap查找空白块
		{
			if (sup_b.Bitmap[i] == 255) //Bitmap该字节代表的文件块都已被占用
				continue;
			for (j = 7; j >= 0; j--)
			{
				if ((((sup_b.Bitmap[i] >> j) | 0) % 2) == 0) //unsigned char右移后左侧补全0
				{
					b_no[k] = i * 8 + 8 - j;
					k++;
					sup_b.Bitmap[i] = sup_b.Bitmap[i] ^ (1 << j);
					if (sup_b.Bitmap[i] == 255)
						break;
					if (k == free_b_need)
						break;
				}
			}
			if (k == free_b_need)
				break;
		}
		sup_b.Free_block_num -= free_b_need;
		if (s1_size <= 28672) //s1小于28KB
		{
			if (strcmp(pexname, "jpg") == 0 || strcmp(pexname, "png") == 0)
				ff = fopen(s1, "rb");
			else
				ff = fopen(s1, "r");
			fp = fopen(sup_b.FS_path, "rb+");
			for (i = 0; i < free_b_need; i++)
			{
				char str[4096] = { 0 };
				if (i == free_b_need - 1)
				{
					fseek(ff, i*4096, SEEK_SET);//
					fread(str, sizeof(char), 4096, ff);
					fseek(fp, 4 * 1024 * 1024 + (b_no[i] - 1) * 4 * 1024, SEEK_SET);					
					//printf("%s\n", str);
					fwrite(str, sizeof(char), s1_size % (4 * 1024), fp);
					fseek(ff, 0, SEEK_CUR);
					nfcb->Filelocal[i] = b_no[i];
				}
				else
				{
					fseek(ff, i * 4096, SEEK_SET);//
					fread(str, sizeof(char), 4096, ff);
					fseek(fp, 4 * 1024 * 1024 + (b_no[i] - 1) * 4 * 1024, SEEK_SET);
					//printf("%s\n", str);
					fwrite(str, sizeof(char), 4096, fp);
					fseek(ff, 0, SEEK_CUR);
					nfcb->Filelocal[i] = b_no[i];
				}
			}
			fclose(ff);
			fclose(fp);
		}
		else if (s1_size > 28672 && s1_size <= 29360128)//大于28KB小于28MB
		{
			if (strcmp(pexname, "jpg") == 0 || strcmp(pexname, "png") == 0)
				ff = fopen(s1, "rb");
			else
				ff = fopen(s1, "r");
			fp = fopen(sup_b.FS_path, "rb+");
			for (i = 0; i < 7; i++)
				nfcb->Filelocal[i] = b_no[i];
			int i1 = 0, j1 = 0;
			for (i1 = 0; i1 < 7; i1++)
			{
				char str[4096] = { 0 };
				int suoyin[1024] = { 0 };
				for (j1 = 0; j1 < 1024; j1++)
				{
					if ((i1 * 1024 + j1 + 8) == free_b_need)
					{
						fseek(ff, (i1 * 1024 + j1) * 4096, SEEK_SET);//
						fread(str, sizeof(char), 4096, ff);
						//printf("%s", str);
						fseek(fp, 4 * 1024 * 1024 + (b_no[i1 * 1024 + j1 + 7] - 1) * 4 * 1024, SEEK_SET);
						fwrite(str, sizeof(char), s1_size % (4 * 1024), fp);
						fseek(ff, 0, SEEK_CUR);
						suoyin[j1] = b_no[i1 * 1024 + j1 + 7];
						j1++;
						break;
					}
					else
					{
						fseek(ff, (i1 * 1024 + j1) * 4096, SEEK_SET);//
						fread(str, sizeof(char), 4096, ff);
						//printf("%s", str);
						fseek(fp, 4 * 1024 * 1024 + (b_no[i1 * 1024 + j1 + 7] - 1) * 4 * 1024, SEEK_SET);
						fwrite(str, sizeof(char), 4096, fp);
						fseek(ff, 0, SEEK_CUR);
						suoyin[j1] = b_no[i1 * 1024 + j1 + 7];
					}
				}
				fseek(fp, 4 * 1024 * 1024 + (b_no[i1] - 1) * 4 * 1024, SEEK_SET);
				fwrite(suoyin, sizeof(int), j1, fp);
				fseek(ff, 0, SEEK_CUR);
				if ((i1 * 1024 + j1 + 8) == free_b_need + 1)
					break;
			}
			fclose(ff);
			fclose(fp);
		}
	}

	else if (flag == 2)
	{
		/*char pname[9] = { 0 };
		char pexname[4] = { 0 };
		int i = 0, j = 0;
		for (i = 0; i < strlen(s1); i++)
		{
			if (s1[i] == '.')
				break;
			pname[i] = s1[i];
		}
		pname[i] = '\0';
		i++;
		for (j = i; j < strlen(s1); j++)
		{
			pexname[j - i] = s1[j];
		}
		pexname[j - i] = '\0';
		//复制母文件名称完成
		*/
		int i = 0, j = 0;
		FCB *ffcb = sup_b.Full_fcb;
		int flag1 = 0;
		while (1)
		{
			if ((strcmp(pname, ffcb->Name) == 0) && (strcmp(pexname, ffcb->Exname) == 0))//找到该文件，ffcb指向了这个文件的FCB
			{
				flag1 = 1;
				break;
			}
			if (ffcb->Fcb_next == NULL)	break;
			ffcb = ffcb->Fcb_next;
		}
		if (flag1 == 0)
		{
			printf("该文件不存在\n");
			return;
		}
		/*找到文件后*/
		int s1_size = ffcb->filesize;
		int free_b_need = s1_size / (4 * 1024);
		int left_B = s1_size % (4 * 1024);
		if (left_B > 0)
			free_b_need++;
		if (s1_size > 28672 && s1_size <= 29360128) //文件s1大于28KB小于28MB再加7个索引块
			free_b_need += 7;
		if (s1_size > 29360128) //文件s1大于28MB再加7175块索引块
		{
			printf("file is too big\n");
			return;
			free_b_need += 7175;
		}
		if (free_b_need > sup_b.Free_block_num)
		{
			printf("文件过大\n");
			return;
		}
		/*判断文件大小结束*/
		FCB *nfcb = sup_b.Full_fcb;
		while (1)
		{
			if (nfcb->Fcb_next == NULL)
				break;
			nfcb = nfcb->Fcb_next;
		}//找到FULL中倒数第1块FCB
		nfcb->Fcb_next = (FCB *)malloc(sizeof(FCB));
		/*设置名称开始*/
		for (i = 0; i < strlen(s2); i++)
		{
			if (s2[i] == '.')
				break;
			nfcb->Fcb_next->Name[i] = s2[i];
		}
		nfcb->Fcb_next->Name[i] = '\0';
		i++;
		for (j = i; j < strlen(s2); j++)
		{
			nfcb->Fcb_next->Exname[j - i] = s2[j];
		}
		nfcb->Fcb_next->Exname[j - i] = '\0';
		/*设置名称结束*/
		time(&(nfcb->Fcb_next->filetime));
		nfcb->Fcb_next->filesize = s1_size;
		nfcb->Fcb_next->Fcb_next = NULL;
		nfcb->Fcb_next->Fcb_parent = nfcb;
		/*FCB信息初始化完成，只剩filelocal*/
		nfcb = nfcb->Fcb_next; //nfcb指向备份的FCB
		sup_b.Full_fcb_num++;

		int *b_no = NULL;
		b_no = (int *)malloc(sizeof(int) * free_b_need);
		int k = 0;
		for (i = 0; i < 32640; i++) //搜索Bitmap查找空白块
		{
			if (sup_b.Bitmap[i] == 255) //Bitmap该字节代表的文件块都已被占用
				continue;
			for (j = 7; j >= 0; j--)
			{
				if ((((sup_b.Bitmap[i] >> j) | 0) % 2) == 0) //unsigned char右移后左侧补全0
				{
					b_no[k] = i * 8 + 8 - j;
					k++;
					sup_b.Bitmap[i] = sup_b.Bitmap[i] ^ (1 << j);
					if (sup_b.Bitmap[i] == 255)
						break;
					if (k == free_b_need)
						break;
				}
			}
			if (k == free_b_need)
				break;
		}
		sup_b.Free_block_num -= free_b_need;
		/*寻找空白块结束*/

		/*复制文件内容*/
		FILE *fp = NULL;
		if (s1_size <= 28672) //s1小于28KB
		{
			char str[4096] = { 0 };
			fp = fopen(sup_b.FS_path, "rb+");
			for (i = 0; i < free_b_need; i++)
			{
				if (i == free_b_need - 1)
				{
					fseek(fp, 4 * 1024 * 1024 + (ffcb->Filelocal[i] - 1) * 4 * 1024, SEEK_SET);
					fread(str, sizeof(char), 4096, fp);
					fseek(fp, 4 * 1024 * 1024 + (b_no[i] - 1) * 4 * 1024, SEEK_SET);
					fwrite(str, sizeof(char), s1_size % (4 * 1024), fp);
					fseek(fp, 0, SEEK_CUR);
					nfcb->Filelocal[i] = b_no[i];
				}
				else
				{
					fseek(fp, 4 * 1024 * 1024 + (ffcb->Filelocal[i] - 1) * 4 * 1024, SEEK_SET);
					fread(str, sizeof(char), 4096, fp);
					fseek(fp, 4 * 1024 * 1024 + (b_no[i] - 1) * 4 * 1024, SEEK_SET);
					fwrite(str, sizeof(char), 4096, fp);
					fseek(fp, 0, SEEK_CUR);
					nfcb->Filelocal[i] = b_no[i];
				}
			}
			fclose(fp);
		}
		else if (s1_size > 28672 && s1_size <= 29360128)//大于28KB小于28MB
		{
			char str[4096] = { 0 };
			fp = fopen(sup_b.FS_path, "rb+");
			for (i = 0; i < 7; i++)
				nfcb->Filelocal[i] = b_no[i];
			int i1 = 0, j1 = 0;
			int cped_num = s1_size / (4 * 1024);
			int no_suoyin = cped_num / 1024;
			if ((no_suoyin % 1024) != 0)
				no_suoyin++;
			for (i1 = 0; i1 < no_suoyin; i1++)
			{
				int suoyin[1024] = { 0 }, suoyin2[1024] = { 0 };
				if ((i1 == no_suoyin - 1) && ((no_suoyin % 1024) != 0))
				{
					fseek(fp, 4 * 1024 * 1024 + (ffcb->Filelocal[i1] - 1) * 4 * 1024, SEEK_SET);
					fread(suoyin2, sizeof(int), 1024, fp);
				}
				else
				{
					fseek(fp, 4 * 1024 * 1024 + (ffcb->Filelocal[i1] - 1) * 4 * 1024, SEEK_SET);
					fread(suoyin2, sizeof(int), 1024, fp);
				}
				for (j1 = 0; j1 < 1024; j1++)
				{
					if ((i1 * 1024 + j1 + 8) == free_b_need)
					{
						fseek(fp, 4 * 1024 * 1024 + (suoyin2[j1] - 1) * 4 * 1024, SEEK_SET);
						fread(str, sizeof(char), 4096, fp);
						fseek(fp, 4 * 1024 * 1024 + (b_no[i1 * 1024 + j1 + 7] - 1) * 4 * 1024, SEEK_SET);
						fwrite(str, sizeof(char), s1_size % (4 * 1024), fp);
						fseek(fp, 0, SEEK_CUR);
						suoyin[j1] = b_no[i1 * 1024 + j1 + 7];
						j1++;
						break;
					}
					else
					{
						fseek(fp, 4 * 1024 * 1024 + (suoyin2[j1] - 1) * 4 * 1024, SEEK_SET);
						fread(str, sizeof(char), 4096, fp);
						fseek(fp, 4 * 1024 * 1024 + (b_no[i1 * 1024 + j1 + 7] - 1) * 4 * 1024, SEEK_SET);
						fwrite(str, sizeof(char), 4096, fp);
						fseek(fp, 0, SEEK_CUR);
						suoyin[j1] = b_no[i1 * 1024 + j1 + 7];
					}
				}
				fseek(fp, 4 * 1024 * 1024 + (b_no[i1] - 1) * 4 * 1024, SEEK_SET);
				fwrite(suoyin, sizeof(int), j1, fp);
				fseek(fp, 0, SEEK_CUR);
				if ((i1 * 1024 + j1 + 8) == free_b_need + 1)
					break;
			}
			fclose(fp);
		}
	}
	
	else if (flag == 3)
	{
		FCB *ffcb = sup_b.Full_fcb;
		int flag1 = 0;
		while (1)
		{
			if (ffcb == NULL)
				break;
			if ((strcmp(pname, ffcb->Name) == 0) && (strcmp(pexname, ffcb->Exname) == 0))//找到该文件，ffcb指向了这个文件的FCB
			{
				flag1 = 1;
				break;
			}
			if (ffcb->Fcb_next == NULL)
				break;
			ffcb = ffcb->Fcb_next;
		}
		if (flag1 == 0)
		{
			printf("该文件不存在\n");
			return;
		}
		/*找到文件后*/
		int s1_size = ffcb->filesize;
		int free_b_need = s1_size / (4 * 1024);
		int left_b = s1_size % (4 * 1024);
		if (left_b > 0)
			free_b_need++;
		if (s1_size > 28672 && s1_size <= 29360128) //文件s1大于28kb小于28mb再加7个索引块
			free_b_need += 7;
		if (s1_size > 29360128) //文件s1大于28mb再加7175块索引块
		{
			printf("file is too big\n");
			return;
			free_b_need += 7175;
		}
		//if (free_b_need > sup_b.Free_block_num)
		//{
		//	printf("文件过大\n");
		//	return;
		//}
		/*判断文件大小结束*/
		//int s1_size = ffcb->filesize;
		FILE *ff = NULL, *fp = NULL;
		//fp = fopen(sup_b.FS_path, "rb");
		ff = fopen(s2, "wb+");
		fseek(ff, s1_size, SEEK_SET);
		//putw(0, ff);
		//fseek(ff, 0, SEEK_SET);
		fclose(ff);
		int i = 0, j = 0;
		if (s1_size <= 28672)
		{
			fp = fopen(sup_b.FS_path, "rb");
			ff = fopen(s2, "rb+");
			for (i = 0; i < free_b_need; i++)
			{
				char str[4096] = { 0 };
				if (i == free_b_need - 1)
				{
					fseek(fp, 4 * 1024 * 1024 + (ffcb->Filelocal[i] - 1) * 4 * 1024, SEEK_SET);
					fread(str, sizeof(char), 4096, fp);
					fseek(ff, i * 4096, SEEK_SET);//
					fwrite(str, sizeof(char), s1_size % (4 * 1024), ff);
					fseek(ff, 0, SEEK_CUR);
					fseek(ff, 0, SEEK_CUR);
				}
				else
				{
					fseek(fp, 4 * 1024 * 1024 + (ffcb->Filelocal[i] - 1) * 4 * 1024, SEEK_SET);
					fread(str, sizeof(char), 4096, fp);
					fseek(ff, i * 4096, SEEK_SET);//
					fwrite(str, sizeof(char), 4096, ff);
					fseek(ff, 0, SEEK_CUR);
				}
			}
			fclose(fp);
			fclose(ff);
		}
		else if (s1_size > 28672 && s1_size <= 29360128)
		{
			int cped_num = s1_size / (4 * 1024);
			int no_suoyin = cped_num / 1024;
			if ((cped_num % 1024) != 0)
				no_suoyin++;
			fp = fopen(sup_b.FS_path, "rb");
			ff = fopen(s2, "rb+");
			for (i = 0; i < no_suoyin; i++)
			{
				char str[4096] = { 0 };
				int suoyin[1024] = { 0 };
				if ((i == no_suoyin - 1) && ((no_suoyin % 1024) != 0))
				{
					fseek(fp, 4 * 1024 * 1024 + (ffcb->Filelocal[i] - 1) * 4 * 1024, SEEK_SET);
					fread(suoyin, sizeof(int), 1024, fp);
				}
				else
				{
					fseek(fp, 4 * 1024 * 1024 + (ffcb->Filelocal[i] - 1) * 4 * 1024, SEEK_SET);
					fread(suoyin, sizeof(int), 1024, fp);
				}
				for (j = 0; j < 1024; j++)
				{
					if ((i * 1024 + j + 8) == free_b_need)
					{
						fseek(fp, 4 * 1024 * 1024 + (suoyin[j] - 1) * 4 * 1024, SEEK_SET);
						fread(str, sizeof(char), 4096, fp);
						//printf(":%s", str);
						fseek(ff, (i * 1024 + j) * 4096, SEEK_SET);//
						fwrite(str, sizeof(char), s1_size % (4 * 1024), ff);
						fseek(ff, 0, SEEK_CUR);
						j++;
						break;
					}
					else
					{
						fseek(fp, 4 * 1024 * 1024 + (suoyin[j] - 1) * 4 * 1024, SEEK_SET);
						fread(str, sizeof(char), 4096, fp);
						fseek(ff, (i * 1024 + j) * 4096, SEEK_SET);//
						fwrite(str, sizeof(char), 4096, ff);
						fseek(ff, 0, SEEK_CUR);
					}
				}
				if ((i * 1024 + j + 8) == free_b_need + 1)
					break;
			}
			fclose(fp);
			fclose(ff);
		}
	}
	//printf("%d %d\n", sup_b.Bitmap[0], sup_b.Bitmap[1]);
}