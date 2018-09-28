#ifndef FUC_ANNOU_H
#define FUC_ANNOU_H

/*函数申明*/
int order_input(char *o1, char *o2, char *o3);

void deal(char *o1, char *o2, char *o3);

void create();

void mount();

void fmt();

void help(char *order);

void close();

void cp(char *s1, char *s2);

void Att(char *fileName);

void dl(char sFName[]);

void dr();

void map(char *sn);

void rewrite(char *sn);

void delete_new(char sFName[]);

void empty_bin(int flag);

void rst(char sFName[]);

void opt(char *sn);

void add(char *sn);

void tp(char sFName[]);

void rename(char sFName[], char new_sFNmae[]);
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

#endif