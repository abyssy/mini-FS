#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"../mini-FS/structure.h"
#include"../mini-FS/fuc_annou.h"


struct super_block sup_b;

int main(int argc, char *argv[])
{
	/*内存中申明的超级块初始化*/
	sup_b.FS = NULL;
	strcpy(sup_b.FS_path, "d:\\"); //因为存在\且只需包含一个\所以在后续操作中把\消掉
	sup_b.flag = 0;
	sup_b.Fs_size = 1024 * 1024 * 1024;
	sup_b.Block_size = 4 * 1024;
	sup_b.Free_block_num = 1024 / 4 * 1020;
	sup_b.BM_first_free = 0;
	for (int i = 0; i < BMmaxx; i++)
		sup_b.Bitmap[i] = 0;
	sup_b.Free_fcb = NULL;
	int Full_fcb_num = 0;
	int Free_fcb_num = 0;
	sup_b.Full_fcb = NULL;
	unsigned int BM_first_free = 0;
	/*内存中申明的超级块初始化*/

	char order1[201];
	char order2[201];
	char order3[201];
	printf("文件系统已启动\n");
	printf("--------------------\n");
	while (1) {
		memset(order1, 0, sizeof(order1));
		memset(order2, 0, sizeof(order2));
		memset(order3, 0, sizeof(order3));
		printf("\n###naokuoteng###>");
		if (order_input(order1, order2, order3) == -1)
			continue;
		if (strcmp(order1, "close") == 0)
		{
			if ((strlen(order2) != 0) || (strlen(order3) != 0))
			{
				printf("命令格式错误,请重新输入。\n");
				continue;
			}
			else
				close();
			break;
		}
		deal(order1, order2, order3);
	}

	return 0;
}
/*
功能：
create：为mini-FS分配1G空间
mount：初始化这个空间
fmt：初始化
dr：显示文件名和拓展名结合
cp：复制文件
dl：删除文件
tp：显示文本所有内容
more：分页显示文本内容
att：显示文件属性
help：帮助信息
close：退出系统
*/