#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"../mini-FS/structure.h"
#include"../mini-FS/fuc_annou.h"


void mount()
{
	extern struct super_block sup_b;
	sup_b.FS_path[3] = '\0';
	strcat(sup_b.FS_path, "mini-FS.dat\0");
	FILE *fp = fopen(sup_b.FS_path, "rb+");
	if (fp == NULL)
	{
		printf("mount failed!\n");
		return;
	}
	//fseek(sup_b.FS, 1024 * 1024, 0);
	int i = 0;
	fread(&sup_b, sizeof(struct super_block), 1, fp);
	if (sup_b.Full_fcb_num > 0)
	{
		FCB * fcb = NULL;
		FCB * fcb2 = (FCB *)malloc(sizeof(FCB));
		fseek(fp, 1024 * 1024, 0);//´ÓÇ°Íùºó¶Á
		fread(fcb2, sizeof(FCB), 1, fp);
		sup_b.Full_fcb = fcb2;
		fcb = fcb2;
		fcb2 = NULL;
		fcb->Fcb_next = NULL;
		fseek(fp, 1024 * 1024 + 64, SEEK_SET);
		for (i = 2; i <= sup_b.Full_fcb_num; i++)
		{
			fcb2 = (FCB *)malloc(sizeof(FCB));
			fread(fcb2, sizeof(FCB), 1, fp);
			fseek(fp, 1024 * 1024 + 64 * i, SEEK_SET);
			fcb->Fcb_next = fcb2;
			fcb2->Fcb_parent = fcb;
			fcb2->Fcb_next = NULL;
			fcb = fcb->Fcb_next;
			fcb2 = NULL;
		}
	}
	// fread(fcb2, sizeof(FCB), 1, fp);
	// fcb2 = (FCB *) malloc(sizeof(FCB));
	// superBlock.Free_fcb = fcb2;
	// fcb = fcb2;
	// fcb->Fcb_next = NULL;
	// // fseek(fp, 64 - sizeof(FCB), 1);
	// for (int i = 0; i < superBlock.Free_fcb_num - 1; i ++)
	// {
	// 	fcb2 = (FCB *)malloc(sizeof(FCB));
	// 	// fread(fcb2, sizeof(FCB), 1, fp);
	// 	// fseek(fp, 64 - sizeof(FCB), 1);
	// 	fcb->Fcb_next = fcb2;
	// 	fcb2->Fcb_parent = fcb;
	// 	fcb2->Fcb_next = NULL;
	// }
	printf("mount system successfully!!\n");
	fclose(fp);
	//printf("%d %d\n", sup_b.Bitmap[0], sup_b.Bitmap[1]);
	return;
}