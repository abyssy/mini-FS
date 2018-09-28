#include<stdio.h>
#include<string.h>
#include<time.h>
#include"../mini-FS/structure.h"
#include"../mini-FS/fuc_annou.h"


void dr()
{
	extern struct super_block sup_b;
	FILE *fp = NULL;
	FCB *fcb = sup_b.Full_fcb;//记录文件控制块的指针
	if(fcb == NULL)
	{
		printf("no files\n");
		return;
	}
/**************打印文件目录***********/
	fp = fopen(sup_b.FS_path, "rb");
	while(1)
	{
		//printTime(fcb->filetime);
		printf("%s", fcb->Name);
		printf(".");
		printf("%s\n", fcb->Exname);
		printf("%d Byte\n", fcb->filesize);
		struct tm *p;
		p = gmtime(&(fcb->filetime));
		printf("%d年%d月%d日%d时%d分%d秒\n", 1900 + p->tm_year, 1 + p->tm_mon, p->tm_mday, 8 + p->tm_hour, p->tm_min, p->tm_sec);
		printf("\n");
		if(fcb->Fcb_next == NULL)	
			break;
		fcb = fcb->Fcb_next;
	}
	fclose(fp);

	return;
}