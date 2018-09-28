#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"../mini-FS/structure.h"
#include"../mini-FS/fuc_annou.h"

//flag == 0 �û��Լ�ѡ����ջ���վ
//flag == 1 ��close������ջ���վ
void empty_bin(int flag)
{
	extern struct super_block sup_b;
	FILE *fp = NULL;
	FCB * fcb = NULL;

	//������û��Լ�ѡ����ջ���վ
	if (flag == 0)
	{
		printf("Are you sure you want to empty you recycle bin? Deleted files will not be restored\n");
		printf("Please enter Y/N\n");
		char sure;
		scanf("%c%*c", &sure);
		if (sure == 'Y' || sure == 'y')
			;
		else
		{
			printf("No operating on the recycle bin\n");
			return;
		}
	}

	/*****************************/
	/*****************************/
	/*****************************/
	/*
		printf("bitmap:\n");
	for (int i = 0; i < 10; i++)
		printf("%d ", sup_b.Bitmap[i]);
	printf("\n");
	*/
	/*****************************/
	/*****************************/
	/*****************************/


	fcb = sup_b.Free_fcb;
	while (1)
	{
		//�������
		if (fcb == NULL)
			break;

		int file_b[1024] = { 0 };
		int addr[1024] = { 0 };
		int cnt, tmp, zijie, wei;
		int i = 0, j = 0;

		//��λͼ���ļ�λ����0
		memset(file_b, 0, sizeof(file_b));
		if (fcb->filesize <= 7 * 4 * 1024) //С�ڵ���28KB
		{
			fp = fopen(sup_b.FS_path, "rb+");

			//�����ж��ٴ��̿鱻�õ�
			cnt = fcb->filesize / (4 * 1024);
			if (fcb->filesize % (4 * 1024) != 0)
				cnt++;

			for (j = 0; j < cnt; j++)
			{
				tmp = fcb->Filelocal[j] - 1;
				zijie = tmp / 8;
				wei = tmp % 8;
				// printf("tmp: %d  zijie: %d  wei: %d\n", tmp, zijie, wei);

				//�Ѵ��̿��Ӧλͼ��0
				sup_b.Bitmap[zijie] = sup_b.Bitmap[zijie] & (~(1 << (7 - wei)));

				//��Ӳ�����ļ�������0
				fseek(fp, 4 * 1024 * 1024 + tmp * 4 * 1024, 0);
				fwrite(file_b, sizeof(int), 1024, fp);
			}
			fclose(fp);
		}
		else //����28KB
		{
			cnt = fcb->filesize / (4 * 1024); //�õ��Ĵ��̿������
			if (fcb->filesize % (4 * 1024) != 0)
				cnt++;

			for (j = 0; j < 7; j++)
			{
				//���������λͼ��0
				tmp = fcb->Filelocal[j] - 1;
				zijie = tmp / 8;
				wei = tmp % 8;
				sup_b.Bitmap[zijie] &= ~(1 << (7 - wei));

				memset(addr, 0, sizeof(addr));
				fp = fopen(sup_b.FS_path, "rb+");
				fseek(fp, 4 * 1024 * 1024 + tmp * 4 * 1024, 0);
				fread(addr, sizeof(int), 1024, fp);

				i = 0;
				memset(file_b, 0, sizeof(file_b));
				while (1)
				{
					if (cnt && addr[i] != 0) //�����û�и���
					{
						zijie = (addr[i] - 1) / 8;
						wei = (addr[i] - 1) % 8;

						//�Ѵ��̿��Ӧλͼ��0
						sup_b.Bitmap[zijie] = sup_b.Bitmap[zijie] & (~(1 << (7 - wei)));
						cnt--;
						i++;

						//�Ѵ��̿�洢������0
						fseek(fp, 4 * 1024 * 1024 + (addr[i] - 1) * 4 * 1024, 0);
						fwrite(file_b, sizeof(int), 1024, fp);
					}
					else
						break;
				}
			}
			fclose(fp);
		}
		//�޸Ĵ���ʣ��ռ�
		sup_b.Free_block_num += fcb->filesize / (4 * 1024);
		if (fcb->filesize % (4 * 1024) != 0)
			sup_b.Free_block_num ++;

		if (fcb->Fcb_next == NULL)
			break;
		fcb = fcb->Fcb_next;
	}
	sup_b.Free_fcb = NULL;
	printf("empty the recycle bin successfully!!\n");


	/*****************************/
	/*****************************/
	/*****************************/
	/*
	printf("bitmap:\n");
	for (int i = 0; i < 10; i++)
	printf("%d ", sup_b.Bitmap[i]);
	printf("\n");
	*/
	/*****************************/
	/*****************************/
	/*****************************/


	return;
}