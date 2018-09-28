#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"../mini-FS/structure.h"
#include"../mini-FS/fuc_annou.h"

int order_input(char *o1, char *o2, char *o3)
{
	char order[201];
	int i = 0, j = 0, o_len = 0, o1_len = 0, o2_len = 0, o3_len = 0, flag = 0;
	memset(order, 0, sizeof(order));
	for (i = 0; ; i++)
	{
		if (i > 200)
		{
			printf("命令格式不正确：长度过长  请重新输入");
			return -1;
		}
		char c;
		c = getchar();
		if (c == '\n')
		{
			order[i] = '\0';
			break;
		}
		order[i] = c;
	}
	o_len = i; //数组order的长度此时为i
	for (i = 0; i < o_len; i++)  //读取命令的系统命令符
	{
		if (order[i] == ' ' || order[i] == '\0')
			break;
		o1[i] = order[i];
	}
	o1[i] = '\0';
	int a = i;
	o1_len = a;
	//printf("%s", o1);
	if (order[i] == '\0')
	{
		o2[0] = '\0';
		o3[0] = '\0';
		return 0;
	}
	for (i = o1_len + 1; i < o_len; i++)
	{
		if (order[i] == ' ' || order[i] == '\0')
			break;
		o2[i - o1_len - 1] = order[i];
	}
	o2[i - o1_len - 1] = '\0';
	o2_len = i - o1_len - 1;
	//printf("%s", o2);
	if (order[i] == '\0')
	{
		o3[0] = '\0';
		return 0;
	}
	for (i = o1_len + o2_len + 2; i < o_len; i++)
	{
		if (order[i] == ' ' || order[i] == '\0')
			break;
		o3[i - o1_len - o2_len - 2] = order[i];
	}
	o3[i - o1_len - o2_len - 2] = '\0';
	o3_len = i - o1_len - o2_len - 2;
	//printf("%s", o3);
	return 0;
}
