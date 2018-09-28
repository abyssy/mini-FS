#include<stdio.h>
#include<stdlib.h>
#include"../mini-FS/structure.h"
#include"../mini-FS/fuc_annou.h"

void close()
{
	extern struct super_block sup_b;
	//printf("%d %d\n", sup_b.Bitmap[0], sup_b.Bitmap[1]);
	FILE *fp = NULL;
	if ((fp = fopen(sup_b.FS_path, "rb+")) == NULL)
	{
		printf("非法命令：不存在相应空间。");
		return;
	}
	printf("正在关闭系统\n");
	//fseek(sup_b.FS, 0, SEEK_SET);
	/*测试代码*/
	//printf("%s", sup_b.FS_path);
	/*测试代码*/
	/*清空回收站*/
	empty_bin(1);
	fwrite(&sup_b, sizeof(struct super_block), 1, fp);
	fseek(fp, 1024 * 1024, SEEK_SET);
	for (int i = 1; i <= sup_b.Full_fcb_num; i++)
	{
		FCB *fff = sup_b.Full_fcb;
		sup_b.Full_fcb = fff->Fcb_next;
		fwrite(fff, sizeof(FCB), 1, fp);
		fseek(fp, 1024 * 1024 + 64 * i, SEEK_SET);
		free(fff);
	}
	//printf("%d %d\n", sup_b.Bitmap[0], sup_b.Bitmap[1]);
	fclose(fp);
}