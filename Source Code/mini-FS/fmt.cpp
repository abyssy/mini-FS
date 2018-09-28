#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"../mini-FS/structure.h"
#include"../mini-FS/fuc_annou.h"

void fmt()
{
	extern struct super_block sup_b;
	FILE *fp = NULL;
	if (strcmp(sup_b.FS_path, "d:\\mini-FS.dat") != 0)
	{
		printf("please create or mount system!\n");
		return;
	}
	if ((fp = fopen(sup_b.FS_path, "rb+")) == NULL)
	{
		printf("format failed!!\n");
		return;
	}

	/********** ³õÊ¼»¯³¬¼¶¿é **********/
	//fseek(sup_b.FS, 1024 * 1024, 0);
	sup_b.Fs_size = 1024 * 1024 * 1024;
	sup_b.Block_size = 4 * 1024;
	sup_b.Free_block_num = 1024 / 4 * 1020;
	sup_b.Full_fcb_num = 0;
	sup_b.Free_fcb_num = 0;
	sup_b.Full_fcb = NULL;
	sup_b.Free_fcb = NULL;
	unsigned char tmp = 0;
	int i = 0;
	for (i = 0; i < 32640; i++)
		sup_b.Bitmap[i] = 0;
	unsigned int BM_first_free = 0;
	fwrite(&sup_b, sizeof(sup_b), 1, fp);

	fseek(fp, 0, 0);
	fread(&sup_b, sizeof(sup_b), 1, fp);

	printf("format successfully!!\n");
	fclose(fp);
	return;
}