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

typedef struct REQUEST//存储现有请求
{
	int target[25];//公交车内部请求
	int clockwise[25];//顺时针请求
	int counterclockwise[25];//逆时针请求
}REQUEST;

typedef struct POINT_//轨道上位点坐标
{
	int x;
	int y;
}POINT_;

typedef struct BUS//公交车相关数据
{
	int state;//公交车运行状态
	int direction;//公交车运行方向
	int position;//公交车所在位置
	int nextposition;
	int targetstation;//公交车目标站点
}BUS;