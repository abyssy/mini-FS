#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"../mini-FS/structure.h"
#include"../mini-FS/fuc_annou.h"

void deal(char *o1, char *o2, char *o3)
{
	if (strcmp(o1, "create") == 0)
	{
		if ((strlen(o2) != 0) || (strlen(o3) != 0))
		{
			printf("命令格式错误,请重新输入。\n");
			return;
		}
		else
			create();
	}
	else if (strcmp(o1, "mount") == 0)
	{
		if ((strlen(o2) != 0) || (strlen(o3) != 0))
		{
			printf("命令格式错误,请重新输入。\n");
			return;
		}
		else
			mount();
	}
	else if (strcmp(o1, "fmt") == 0)
	{
		if ((strlen(o2) != 0) || (strlen(o3) != 0))
		{
			printf("命令格式错误,请重新输入。\n");
			return;
		}
		else
			fmt();
	}
	else if (strcmp(o1, "cp") == 0)
	{
		if (strlen(o2) == 0 || strlen(o3) == 0)
		{
			printf("命令格式错误,请重新输入。\n");
			return;
		}
		else
			cp(o2, o3);
	}
	else if (strcmp(o1, "att") == 0)
	{
		if (strlen(o2) == 0 || strlen(o3) != 0)
		{
			printf("命令格式错误,请重新输入。\n");
			return;
		}
		else
			Att(o2);
	}
	else if (strcmp(o1, "dl") == 0)
	{
		if (strlen(o2) == 0 || strlen(o3) != 0)
		{
			printf("命令格式错误,请重新输入。\n");
			return;
		}
		else
			dl(o2);
	}
	else if (strcmp(o1, "del") == 0)
	{
		if (strlen(o2) == 0 || strlen(o3) != 0)
		{
			printf("命令格式错误,请重新输入。\n");
			return;
		}
		else
			delete_new(o2);
	}
	else if (strcmp(o1, "dr") == 0)
	{
		if (strlen(o2) != 0 || strlen(o3) != 0)
		{
			printf("命令格式错误,请重新输入。\n");
			return;
		}
		else
			dr();
	}
	else if (strcmp(o1, "rst") == 0)
	{
		if (strlen(o2) == 0 || strlen(o3) != 0)
		{
			printf("命令格式错误,请重新输入。\n");
			return;
		}
		else
			rst(o2);
	}
	else if (strcmp(o1, "emptybin") == 0)
	{
		if (strlen(o2) != 0 || strlen(o3) != 0)
		{
			printf("命令格式错误,请重新输入。\n");
			return;
		}
		else
			empty_bin(0);
	}
	else if (strcmp(o1, "map") == 0)
	{
		if (strlen(o2) == 0 || strlen(o3) != 0)
		{
			printf("命令格式错误,请重新输入。\n");
			return;
		}
		else
			map(o2);
	}
	else if (strcmp(o1, "rewrite") == 0)
	{
		if (strlen(o2) == 0 || strlen(o3) != 0)
		{
			printf("命令格式错误,请重新输入。\n");
			return;
		}
		else
			rewrite(o2);
	}
	else if (strcmp(o1, "opt") == 0)
	{
		if (strlen(o2) == 0 || strlen(o3) != 0)
		{
			printf("命令格式错误,请重新输入。\n");
			return;
		}
		else
			opt(o2);
	}
	else if (strcmp(o1, "add") == 0)
	{
		if (strlen(o2) == 0 || strlen(o3) != 0)
		{
			printf("命令格式错误,请重新输入。\n");
			return;
		}
		else
			add(o2);
	}
	else if (strcmp(o1, "rename") == 0)
	{
	if (strlen(o2) == 0 || strlen(o3) == 0)
	{
		printf("命令格式错误,请重新输入。\n");
		return;
	}
	else
		rename(o2, o3);
	}
	else if (strcmp(o1, "tp") == 0)
	{
	if (strlen(o2) == 0 || strlen(o3) != 0)
	{
		printf("命令格式错误,请重新输入。\n");
		return;
	}
	else
		tp(o2);
	}
	else if (strcmp(o1, "help") == 0)
	{
		if (strlen(o3) != 0)
		{
			printf("命令格式错误,请重新输入。\n");
			return;
		}
		else
			help(o2);
	}
}