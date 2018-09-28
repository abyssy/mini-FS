#include<stdio.h>
#include<string.h>
#include"../mini-FS/structure.h"
#include"../mini-FS/fuc_annou.h"

void dl(char sFName[])
{
	extern struct super_block sup_b;
	//printf("%d %d\n", sup_b.Bitmap[0], sup_b.Bitmap[1]);
	FILE *fp = NULL;
	FCB * fcb = NULL;
	char name[9]; //文件名
	char exName[4]; //扩展名

	fcb = sup_b.Full_fcb;

	//处理文件名和扩展名
	int i = 0;
	int j = 0;
	int flag = 0;
	for (i = 0; sFName[i] != '\0'; i ++)
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
			j ++;
		}
		else if (flag == 1)
		{
			exName[j] = sFName[i];
			j ++;
		}
	}
	exName[j] = '\0';

	flag = 0;
	FCB * fcbPa = NULL;

	int tmp, zijie, wei, cnt;
	int addr[1024];

	while(1)
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
			sup_b.Free_fcb = fcb;
			char sure;
			scanf("%c%*c", &sure);
			if (sure == 'Y' || sure == 'y')
				sup_b.Free_fcb = NULL;
			else
			{
				flag = 2;
				sup_b.Free_fcb = NULL;
				break;
			}

			int file_b[1024] = {0};

			//把位图中文件位置置0
			memset(file_b, 0, sizeof(file_b));
			if (fcb->filesize <= 7 * 4 * 1024) //小于等于28KB
			{
				fp = fopen(sup_b.FS_path, "rb+");
				cnt = fcb->filesize / (4 * 1024);
				if (fcb->filesize % (4 * 1024) != 0)
					cnt ++;
				for (j = 0; j < cnt; j ++)
				{
					tmp = fcb -> Filelocal[j] - 1;
					zijie = tmp / 8;
					wei = tmp % 8;
					// printf("tmp: %d  zijie: %d  wei: %d\n", tmp, zijie, wei);
					sup_b.Bitmap[zijie] = sup_b.Bitmap[zijie] & (~(1 << (7 - wei))); 
					//把硬盘中文件内容置0
					fseek(fp, 4 * 1024 * 1024 + tmp * 4 * 1024, 0);
					fwrite(file_b, sizeof(int), 1024, fp);
				}
				fclose(fp);
			}
			else //大于28KB
			{
				cnt = fcb->filesize / (4 * 1024); //需要改位图属性的块的数量
				if (fcb->filesize % (4 * 1024) != 0)
					cnt ++;
				for (j = 0; j < 7; j ++)
				{
					//把索引块的位图置0
					tmp = fcb -> Filelocal[j] - 1;
					zijie = tmp / 8;
					wei = tmp % 8;
					sup_b.Bitmap[zijie] &= ~(1 << (7 - wei));

					memset(addr, 0, sizeof(addr));
					fp = fopen(sup_b.FS_path, "rb+");
					fseek(fp, 4 * 1024 * 1024 + tmp * 4 * 1024, 0);
					fread(addr, sizeof(int), 1024, fp);

					i = 0;
					memset(file_b, 0, sizeof(file_b));
					while(1)
					{
						if (cnt && addr[i] != 0) //如果还没有改完
						{
							zijie = (addr[i] - 1) / 8;
							wei = (addr[i] - 1) % 8;

							sup_b.Bitmap[zijie] = sup_b.Bitmap[zijie] & (~(1 << (7 - wei)));
							cnt --;
							i ++;

							//把磁盘块置0
							fseek(fp, 4*1024*1024 + (addr[i] - 1) * 4 * 1024, 0);
							fwrite(file_b, sizeof(int), 1024, fp);
						}
						else
							break;
					}
				}
				fclose(fp);
			}

			//从目录中删掉文件
			if (fcb->Fcb_parent != NULL)
			{
				//如果该目录前还有目录
				fcbPa = fcb -> Fcb_parent;
				fcbPa -> Fcb_next = fcb->Fcb_next;
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
			fcb -> Fcb_next = NULL;
			fcb -> Fcb_parent = NULL;
			sup_b.Full_fcb_num --;
			//sup_b.Free_fcb_num ++; //注释掉这一行
			sup_b.Free_block_num += fcb->filesize / (4 * 1024);
			if (fcb->filesize % (4 * 1024) != 0)
				sup_b.Free_block_num ++;
			break;
		}
		if (fcb -> Fcb_next == NULL)
			break;
		fcb = fcb -> Fcb_next;
	}
	//printf("%d %d\n", sup_b.Bitmap[0], sup_b.Bitmap[1]);
	if (flag == 0) //文件不存在
		printf("this file does not exist!!\n");
	else if (flag == 1) //删除成功
		printf("delete file successfully!!\n");
	else if (flag == 2) //误删后恢复成功
		printf("restore file successfully!!\n");
	return;
}