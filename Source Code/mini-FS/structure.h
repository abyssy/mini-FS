#ifndef STRUCTURE_H
#define STRUCTURE_H

#include<time.h>

const int BMmaxx = 32640;//32640*8为文件内容块数
const int FCB_COUNT = 49152; //文件控制块个数

struct FCB {
	char Name[9];		//文件名
	char Exname[4];		//拓展名
	time_t filetime;	//创建时间 8B
	int filesize;		//文件大小
	FCB *Fcb_next;		//指向下一个文件控制块
	FCB *Fcb_parent;	//指向上一个文件控制块
	int Filelocal[7];	//文件内容占用的物理块编号
};	//文件控制块 目前64B

struct super_block {
	FILE *FS;						//指向1G空间的起始位置
	char FS_path[100];				//记录空间的绝对路径
	int flag;						//判断文件系统是否使用过，方便初始化。
	int Fs_size;					//文件系统大小
	int Block_size;					//数据块的大小
	int Free_block_num;				//空闲块的数量 （实时更改）
	unsigned int BM_first_free;		//指向位示图中第一个空白块
	unsigned char Bitmap[BMmaxx];	//位示图，方便查看文件块的占用/空闲情况 （实时更改）
	FCB *Free_fcb;					//指向第一个空闲文件控制块
	int Free_fcb_num;				// （实时更改）
	FCB *Full_fcb;					//指向第一个占用文件控制块
	int Full_fcb_num;				// （实时更改）
};	//超级块



#endif //STRUCTURE_H