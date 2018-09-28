#include<stdio.h>
#include<string.h>
#include"../mini-FS/structure.h"
#include"../mini-FS/fuc_annou.h"

void create()
{
	extern struct super_block sup_b;
	FILE *fp = NULL;
	sup_b.flag = 1;
	sup_b.FS_path[3] = '\0';
	strcat(sup_b.FS_path, "mini-FS.dat\0");
	if ((fp = fopen(sup_b.FS_path, "wb+")) == NULL)
	{
		printf("系统名称不合法。");
		return;
	}
	fseek(fp, 1024 * 1024 * 1024, 0);
	putw(0, fp);
	sup_b.FS = fp;
	fclose(fp);
	if ((fp = fopen(sup_b.FS_path, "rb+")) != NULL)
		printf("开辟空间操作完成。\n");
	//fseek(sup_b.FS, 0, SEEK_SET); //回到文件头
	/*char a[10] = { "abc" };
	fwrite(a, sizeof(a), 3, sup_b.FS); //测试代码*/
	/*测试代码*/
	//printf("%s", sup_b.FS_path);
	/*测试代码*/
	fwrite(&sup_b, sizeof(struct super_block), 1, fp);
	fclose(fp);
}