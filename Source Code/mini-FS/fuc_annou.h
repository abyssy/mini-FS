#ifndef FUC_ANNOU_H
#define FUC_ANNOU_H

/*��������*/
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
���ܣ�
create��Ϊmini-FS����1G�ռ�
mount����ʼ������ռ�
fmt����ʼ��
dr����ʾ�ļ�������չ�����
cp�������ļ�
dl��ɾ���ļ�
tp����ʾ�ı���������
more����ҳ��ʾ�ı�����
att����ʾ�ļ�����
help��������Ϣ
close���˳�ϵͳ
*/

#endif