#include<stdio.h>
#include<string.h>
#include"../mini-FS/structure.h"
#include"../mini-FS/fuc_annou.h"

void help(char *order)
{
	if(order[0] == '\0')
	{
		printf("关于某个命令的详细信息，请按格式输入：help 命令名称\n");
		printf("create		创建一个大小为1G的空间\nmount		挂载空间\nfmt		格式化当前 SName 空间\ndr		显示空间中的文件目录：dir\ncp		复制文件：copy\ndl		删除空间中的文件：del\n");
		printf("del             将文件放入垃圾箱\n");
		printf("opt             使文件分块连续\n");
		printf("tp              显示文件内容\n");
		printf("add             新建文件\n");
		printf("rewrite         修改文件\n");
		printf("rename          对文件进行重命名\n");
		printf("rst             恢复本次使用中被删除的文件：restore\n");
		printf("empty_bin       清空您的回收站，被删除的文件将无法被恢复\n");
		printf("map		显示空间中的文件所占块号：map\natt		显示空间文件属性\nhelp		帮助信息\nclose		退出空间，返回操作系统\n");
		return;
	}
	else if(strcmp(order, "create") == 0)
    {
        printf("格式:create\n生成大小为1G的存储空间\n");
        return;
    }
	else if (strcmp(order, "mount") == 0)
	{
		printf("格式:mount\n挂载空间，完成所有准备工作\n");
		return;
	}
	else if(strcmp(order, "fmt") == 0)
	{
		printf("格式:fmt\n初始化当前打开的 SName 空间，建立初始结构\n");
		return;
	}
	else if(strcmp(order, "dr") == 0)
	{
		printf("格式：dr [sFName]\n显示文件目录\n");
		return;
	}
	else if(strcmp(order, "cp") == 0)
	{
		printf("格式：cp wFName  sFName\n      cp sFName  wFName\n      cp sFName1 sFName2\n");
		printf("拷贝文件\n");
		return;
	}
	else if(strcmp(order, "dl") == 0)
	{
		printf("格式：dl sFName\n删除文件所有数据\n");
		return;
	}
	else if (strcmp(order, "del") == 0)
	{
		printf("格式：del sFName\n删除文件到回收站\n");
		return;
	}
	else if (strcmp(order, "emptybin") == 0)
	{
		printf("格式：emptybin\n清空回收站\n");
		return;
	}
	else if (strcmp(order, "rst") == 0)
	{
		printf("格式：rst sFName\n恢复文件，从回收站中\n");
		return;
	}
	else if (strcmp(order, "opt") == 0)
	{
		printf("格式：opt sFName\n使文件占用的块连续\n");
		return;
	}
	else if(strcmp(order, "map") == 0)
	{
		printf("格式：map sFName\n显示文件占用的块号\n");
		return;
	}
	//else if(strcmp(order, "more") == 0)
	//{
	//	printf("格式：more sFName\n只能分页显示单个文件的内容\n");
	//	return;
	//}
	else if(strcmp(order, "att") == 0)
	{
		printf("格式：att sFName\n文件属性包括：文件名、扩展名、文件创建日期和时间、文件大小等\n");
		return;
	}
	else if(strcmp(order, "help") == 0)
	{
		printf("格式：help [命令名]\n获取某个命令的详细信息\n");
		return;
	}
	else if (strcmp(order, "add") == 0)
	{
		printf("格式：add [文件名]\n新建文件\n");
		return;
	}
	else if (strcmp(order, "tp") == 0)
	{
		printf("格式：tp [文件名]\n显示文件内容\n");
	}
	else if (strcmp(order, "rename") == 0)
	{
		printf("格式：rename [文件名] [新文件名]\n把名字为[文件名]的文件，更改文件名为[新文件名]\n");
	}
	else if(strcmp(order, "close") == 0)
	{
		printf("格式：close [命令名]\n保存信息，关闭 mini-FS 系统，退出\n");
		return;
	}
	else if (strcmp(order, "rewrite") == 0)
	{
		printf("格式：rewrite [文件名]\n修改该文件的内容\n");
		return;
	}
	else
	{
		printf("输入命令格式错误，请重新输入\n");
		return;
	}
}
