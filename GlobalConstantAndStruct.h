#pragma once

#define GRAY RGB(200, 200, 200)
#define PI 3.14159265359

#define FCFS 1
#define SSTF 2
#define SCAN 3

#define TARGET 1
#define CLOCKWISE 2
#define COUNTERCLOCKWISE 3
#define STOP 1
#define END -1

#define FREE 1
#define RUN 2
#define STOP_TO_SEVER 3
#define STOP_BY 4

typedef struct REQUEST//�洢��������
{
	int target[25];//�������ڲ�����
	int clockwise[25];//˳ʱ������
	int counterclockwise[25];//��ʱ������
}REQUEST;

typedef struct POINT_//�����λ������
{
	int x;
	int y;
}POINT_;

typedef struct BUS//�������������
{
	int state;//����������״̬
	int direction;//���������з���
	int position;//����������λ��
	int nextposition;
	int targetstation;//������Ŀ��վ��
}BUS;