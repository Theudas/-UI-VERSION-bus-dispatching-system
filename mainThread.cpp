#define _CRT_SECURE_NO_WARNINGS
#include "GlobalConstantAndStruct.h"
#include "mainThread.h"

int totalstation = 5, distance = 2, strategy = FCFS;
int tot_pos;

//输出
char row1[30] = "TIME:0";
char row2[30] = "position:0";
char row3[30] = "target:0000000000";
char row4[30] = "clockwise:0000000000";
char row5[30] = "counterclockwise:0000000000";
char target[20] = { '\0' }, clockwise[20] = { '\0' }, counterclockwise[20] = { '\0' };

int lane_width, lane_height;//轨道宽度、高度
int x1_lane, y1_lane, x2_lane, y2_lane;//轨道四角坐标
int x_bus, y_bus;//公交车图片左上

int req_ = 0;//0为无指令，其余值为对应的指令类型
int target_ = 0, clockwise_ = 0, counterclockwise_ = 0;//记录指令发出的站点
int target__ = 0, clockwise__ = 0, counterclockwise__ = 0;//记录指令记录到哪一位

bool blink_t = false, blink_c = false, blink_u = false;//记录对应光标是否闪烁
int blink_ = 0;//对应光标亮还是灭

POINT_ pos[100];
REQUEST req;
BUS bus;
std::queue<std::pair<char, int>> req_list;
IMAGE bus_, stop1, stop2, stop3, stop4;

bool end = false;
int line_num = 1;
int time__ = 0;

int main(void)
{
	init();
	readfile();
	HANDLE hThread1, hThread2, hThread3;
	hThread1 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)animation, NULL, 0, NULL);
	CloseHandle(hThread1);
	hThread2 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)getinput, NULL, 0, NULL);
	CloseHandle(hThread2);
	hThread3 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)time_, NULL, 0, NULL);
	CloseHandle(hThread3);
	switch (strategy)
	{
	case FCFS:
		fcfs(); break;
	case SSTF:
		sstf(); break;
	case SCAN:
		scan(); break;
	default:
		return -1;
	}
	return 0;
}

void init(void)
{
	bus.state = FREE;
	bus.direction = STOP;
	bus.position = 0;
	bus.nextposition = 0;
	bus.targetstation = 0;
	for (int i = 0; i < 25; ++i)
	{
		req.target[i] = 0;
		req.clockwise[i] = 0;
		req.counterclockwise[i] = 0;
	}
	return;
}

void readfile(void)
{
	extern int strategy, totalstation, distance;//策略代号,总站数,每站距离
	FILE* fp = fopen("dict.dic", "r");//打开文件
	if (fp == NULL) {//判断是否打开文件成功，不成功则退出程序
		exit(0);
	}
	int ch, num;
	char s[50], op[6];//一些临时存储文件中读到内容的变量
	while (!feof(fp)) {
		ch = fgetc(fp);
		if (ch == '#') {//#开头表示该行为注释，跳过该行
			do {
				ch = fgetc(fp);
			} while (ch != '\n');
		}
		else {
			if (ch == 'T') {//T开头表示该行对总站点数进行配置
				fscanf(fp, "%s = %d\n", s, &num);
				totalstation = num;
			}
			else if (ch == 'S') {//S开头表示该行对调度策略进行配置
				fscanf(fp, "%s = %s\n", s, op);
				if (op[2] == 'F') {
					strategy = FCFS;
				}
				else if (op[2] == 'T') {
					strategy = SSTF;
				}
				else {
					strategy = SCAN;
				}
			}
			else if (ch == 'D') {//D开头表示该行对站点间距离进行配置
				fscanf(fp, "%s = %d\n", s, &num);
				distance = num;
			}
		}
	}
}

void fcfs(void)
{
	while (end == false)
	{
		Sleep(100);
		switch (bus.state)
		{
		case FREE:
			if (!req_list.empty())
			{
				bus.state = RUN;
				bus.targetstation = (req_list.front()).second;
				directionjudge();
				bus.nextposition = find_next_position(bus.position);
				run_pixel();
			}
			else
			{
				bus.state = FREE;
			}
			break;
		case RUN:
			if (x_bus == pos[bus.nextposition].x - 15 && y_bus == pos[bus.nextposition].y - 15)
			{
				++time__;
				if (time__ < 10)
				{
					row1[5] = '0' + time__;
					row1[6] = '\0';
				}
				else
				{
					row1[5] = '0' + time__ / 10;
					row1[6] = '0' + time__ % 10;
					row1[7] = '\0';
				}
				bus.position = bus.nextposition;
				if (bus.position < 10)
				{
					row2[9] = '0' + bus.position;
					row2[10] = '\0';
				}
				else
				{
					row2[9] = '0' + bus.position / 10;
					row2[10] = '0' + bus.position % 10;
					row2[11] = '\0';
				}
			}
			if (bus.position % distance == 0 && bus.position / distance + 1 == bus.targetstation)
			{
				bus.state = STOP_TO_SEVER;
			}
			else
			{
				bus.nextposition = find_next_position(bus.position);
				bus.state = RUN;
				run_pixel();
			}
			break;
		case STOP_TO_SEVER:
			bus.position = (bus.targetstation - 1) * distance;
			bus.nextposition = (bus.targetstation - 1) * distance;
			bus.direction = STOP;
			Sleep(5000);
			if ((req_list.front()).first == 'T')
			{
				row3[bus.targetstation + 6] = '0';
				req.target[bus.targetstation] = 0;
			}
			else if ((req_list.front()).first == 'C')
			{
				row4[bus.targetstation + 9] = '0';
				req.clockwise[bus.targetstation] = 0;
			}
			else
			{
				row5[bus.targetstation + 16] = '0';
				req.counterclockwise[bus.targetstation] = 0;
			}
			++time__;
			if (time__ < 10)
			{
				row1[5] = '0' + time__;
				row1[6] = '\0';
			}
			else
			{
				row1[5] = '0' + time__ / 10;
				row1[6] = '0' + time__ % 10;
				row1[7] = '\0';
			}
			bus.position = bus.nextposition;
			if (bus.position < 10)
			{
				row2[9] = '0' + bus.position;
				row2[10] = '\0';
			}
			else
			{
				row2[9] = '0' + bus.position / 10;
				row2[10] = '0' + bus.position % 10;
				row2[11] = '\0';
			}
			req_list.pop();
			if (!req_list.empty())
			{
				bus.state = RUN;
				bus.targetstation = (req_list.front()).second;
				directionjudge();
				bus.nextposition = find_next_position(bus.position);
				run_pixel();
			}
			else
			{
				bus.state = FREE;
			}
			break;
		}
	}
	return;
}

void sstf(void)
{
	while (end == false)
	{
		Sleep(100);
		switch (bus.state)
		{
		case FREE:
			if (!req_list.empty())
			{
				bus.state = RUN;
				findtarget_sstf();
				directionjudge();
				bus.nextposition = find_next_position(bus.position);
				run_pixel();
			}
			else
			{
				bus.state = FREE;
			}
			break;
		case RUN:
			if (x_bus == pos[bus.nextposition].x - 15 && y_bus == pos[bus.nextposition].y - 15)
			{
				++time__;
				if (time__ < 10)
				{
					row1[5] = '0' + time__;
					row1[6] = '\0';
				}
				else
				{
					row1[5] = '0' + time__ / 10;
					row1[6] = '0' + time__ % 10;
					row1[7] = '\0';
				}
				bus.position = bus.nextposition;
				if (bus.position < 10)
				{
					row2[9] = '0' + bus.position;
					row2[10] = '\0';
				}
				else
				{
					row2[9] = '0' + bus.position / 10;
					row2[10] = '0' + bus.position % 10;
					row2[11] = '\0';
				}
			}
			if (bus.position % distance == 0)
			{
				if (bus.position == (bus.targetstation - 1) * distance)
				{
					bus.state = STOP_TO_SEVER;
				}
				else if (judgeby_sstf())
				{
					bus.state = STOP_BY;
				}
				else
				{
					bus.nextposition = find_next_position(bus.position);
					bus.state = RUN;
					run_pixel();
				}
			}
			else
			{
				bus.nextposition = find_next_position(bus.position);
				bus.state = RUN;
				run_pixel();
			}
			break;
		case STOP_TO_SEVER:
			bus.position = (bus.targetstation - 1) * distance;
			bus.nextposition = (bus.targetstation - 1) * distance;
			bus.direction = STOP;
			Sleep(5000);
			if (req.target[bus.targetstation])
			{
				row3[bus.targetstation + 6] = '0';
				req.target[bus.targetstation] = 0;
				req_list.pop();
			}
			if (req.clockwise[bus.targetstation])
			{
				row4[bus.targetstation + 9] = '0';
				req.clockwise[bus.targetstation] = 0;
				req_list.pop();
			}
			if (req.counterclockwise[bus.targetstation])
			{
				row5[bus.targetstation + 16] = '0';
				req.counterclockwise[bus.targetstation] = 0;
				req_list.pop();
			}
			++time__;
			if (time__ < 10)
			{
				row1[5] = '0' + time__;
				row1[6] = '\0';
			}
			else
			{
				row1[5] = '0' + time__ / 10;
				row1[6] = '0' + time__ % 10;
				row1[7] = '\0';
			}
			//bus.position = bus.nextposition;
			if (bus.position < 10)
			{
				row2[9] = '0' + bus.position;
				row2[10] = '\0';
			}
			else
			{
				row2[9] = '0' + bus.position / 10;
				row2[10] = '0' + bus.position % 10;
				row2[11] = '\0';
			}
			if (!req_list.empty())
			{
				bus.state = RUN;
				findtarget_sstf();
				directionjudge();
				bus.nextposition = find_next_position(bus.position);
				run_pixel();
			}
			else
			{
				bus.state = FREE;
				bus.direction = STOP;
			}
			break;
		case STOP_BY:
			if (bus.direction == CLOCKWISE)
			{
				Sleep(5000);
				if (req.clockwise[bus.position / distance + 1])
				{
					row4[bus.position / distance + 1 + 9] = '0';
					req.clockwise[bus.position / distance + 1] = 0;
					req_list.pop();
				}
				if (req.target[bus.position / distance + 1])
				{
					row3[bus.position / distance + 1 + 6] = '0';
					req.target[bus.position / distance + 1] = 0;
					req_list.pop();
				}
			}
			else
			{
				Sleep(5000);
				if (req.counterclockwise[bus.position / distance + 1])
				{
					row5[bus.position / distance + 1 + 16] = '0';
					req.counterclockwise[bus.position / distance + 1] = 0;
					req_list.pop();
				}
				if (req.target[bus.position / distance + 1])
				{
					row3[bus.position / distance + 1 + 6] = '0';
					req.target[bus.position / distance + 1] = 0;
					req_list.pop();
				}
			}
			bus.state = RUN;
			bus.nextposition = find_next_position(bus.position);
			run_pixel();
			break;
		}
	}
	return;
}

void scan(void)
{
	while (end == false)
	{
		Sleep(100);
		switch (bus.state)
		{
		case FREE:
			if (!req_list.empty())
			{
				bus.state = RUN;
				findtarget_sstf();
				directionjudge();
				bus.nextposition = find_next_position(bus.position);
				run_pixel();
			}
			else
			{
				bus.state = FREE;
			}
			break;
		case RUN:
			if (x_bus == pos[bus.nextposition].x - 15 && y_bus == pos[bus.nextposition].y - 15)
			{
				++time__;
				if (time__ < 10)
				{
					row1[5] = '0' + time__;
					row1[6] = '\0';
				}
				else
				{
					row1[5] = '0' + time__ / 10;
					row1[6] = '0' + time__ % 10;
					row1[7] = '\0';
				}
				bus.position = bus.nextposition;
				if (bus.position < 10)
				{
					row2[9] = '0' + bus.position;
					row2[10] = '\0';
				}
				else
				{
					row2[9] = '0' + bus.position / 10;
					row2[10] = '0' + bus.position % 10;
					row2[11] = '\0';
				}
			}
			if (bus.position % distance == 0)
			{
				if (bus.position == (bus.targetstation - 1) * distance)
				{
					bus.state = STOP_TO_SEVER;
				}
				else if (judgeby_scan())
				{
					bus.state = STOP_BY;
				}
				else
				{
					bus.nextposition = find_next_position(bus.position);
					bus.state = RUN;
					run_pixel();
				}
			}
			else
			{
				bus.nextposition = find_next_position(bus.position);
				bus.state = RUN;
				run_pixel();
			}
			break;
		case STOP_TO_SEVER:
			bus.position = (bus.targetstation - 1) * distance;
			bus.nextposition = (bus.targetstation - 1) * distance;
			//bus.direction = STOP;
			Sleep(5000);
			if (req.target[bus.targetstation])
			{
				row3[bus.targetstation + 6] = '0';
				req.target[bus.targetstation] = 0;
				req_list.pop();
			}
			if (req.clockwise[bus.targetstation])
			{
				row4[bus.targetstation + 9] = '0';
				req.clockwise[bus.targetstation] = 0;
				req_list.pop();
			}
			if (req.counterclockwise[bus.targetstation])
			{
				row5[bus.targetstation + 16] = '0';
				req.counterclockwise[bus.targetstation] = 0;
				req_list.pop();
			}
			++time__;
			if (time__ < 10)
			{
				row1[5] = '0' + time__;
				row1[6] = '\0';
			}
			else
			{
				row1[5] = '0' + time__ / 10;
				row1[6] = '0' + time__ % 10;
				row1[7] = '\0';
			}
			//bus.position = bus.nextposition;
			if (bus.position < 10)
			{
				row2[9] = '0' + bus.position;
				row2[10] = '\0';
			}
			else
			{
				row2[9] = '0' + bus.position / 10;
				row2[10] = '0' + bus.position % 10;
				row2[11] = '\0';
			}
			if (!req_list.empty())
			{
				bus.state = RUN;
				findtarget_scan();
				directionjudge();
				bus.nextposition = find_next_position(bus.position);
				run_pixel();
			}
			else
			{
				bus.state = FREE;
				bus.direction = STOP;
			}
			break;
		case STOP_BY:
			Sleep(5000);
			if (req.clockwise[bus.position / distance + 1])
			{
				row4[bus.position / distance + 1 + 9] = '0';
				req.clockwise[bus.position / distance + 1] = 0;
				req_list.pop();
			}
			if (req.target[bus.position / distance + 1])
			{
				row3[bus.position / distance + 1 + 6] = '0';
				req.target[bus.position / distance + 1] = 0;
				req_list.pop();
			}
			if (req.counterclockwise[bus.position / distance + 1])
			{
				row5[bus.position / distance + 1 + 16] = '0';
				req.counterclockwise[bus.position / distance + 1] = 0;
				req_list.pop();
			}
			bus.state = RUN;
			bus.nextposition = find_next_position(bus.position);
			run_pixel();
			break;
		}
	}
	return;
}

int find_next_position(int cur)
{
	if (bus.direction == CLOCKWISE)
	{
		if (cur == tot_pos - 1)
		{
			return 0;
		}
		else
		{
			return cur + 1;
		}
	}
	else if (bus.direction == COUNTERCLOCKWISE)
	{
		if (cur == 0)
		{
			return tot_pos - 1;
		}
		else
		{
			return cur - 1;
		}
	}
	else
	{
		return cur;
	}

}

void directionjudge(void)
{
	extern BUS bus;
	extern int strategy, totalstation, distance;
	int cursta = bus.position / distance + 1;//当前站点
	int tarsta = bus.targetstation;//目标站点
	if (tarsta == cursta)
	{
		bus.direction = STOP;
	}
	else if (tarsta > cursta)
	{
		if ((tarsta - cursta) <= (totalstation / 2.0))//顺时针方向较近
		{
			bus.direction = CLOCKWISE;
		}
		else
		{
			bus.direction = COUNTERCLOCKWISE;
		}
	}
	else if (cursta > tarsta)
	{
		if ((cursta - tarsta) < (totalstation / 2.0))
		{
			bus.direction = COUNTERCLOCKWISE;
		}
		else
		{
			bus.direction = CLOCKWISE;
		}
	}
}

void run_pixel(void)
{
	if (bus.direction == CLOCKWISE)
	{
		switch (line_num)
		{
		case 1:
			if (x_bus == x2_lane - 15)
			{
				++y_bus;
				line_num = 2;
				break;
			}
			++x_bus;
			if (x_bus == x2_lane - 15)
			{
				line_num = 2;
			}
			break;
		case 2:
			if (y_bus == y2_lane - 15)
			{
				--x_bus;
				line_num = 3;
				break;
			}
			++y_bus;
			if (y_bus == y2_lane - 15)
			{
				line_num = 3;
			}
			break;
		case 3:
			if (x_bus == x1_lane - 15)
			{
				--y_bus;
				line_num = 4;
				break;
			}
			--x_bus;
			if (x_bus == x1_lane - 15)
			{
				line_num = 4;
			}
			break;
		case 4:
			if (y_bus == y1_lane - 15)
			{
				++x_bus;
				line_num = 1;
				break;
			}
			--y_bus;
			if (y_bus == y1_lane - 15)
			{
				line_num = 1;
			}
			break;
		}
	}
	else if (bus.direction == COUNTERCLOCKWISE)
	{
		switch (line_num)
		{
		case 1:
			if (x_bus == x1_lane - 15)
			{
				++y_bus;
				line_num = 4;
				break;
			}
			--x_bus;
			if (x_bus == x1_lane - 15)
			{
				line_num = 4;
			}
			break;
		case 2:
			if (y_bus == y1_lane - 15)
			{
				--x_bus;
				line_num = 1;
				break;
			}
			--y_bus;
			if (y_bus == y1_lane - 15)
			{
				line_num = 1;
			}
			break;
		case 3:
			if (x_bus == x2_lane - 15)
			{
				--y_bus;
				line_num = 2;
				break;
			}
			++x_bus;
			if (x_bus == x2_lane - 15)
			{
				line_num = 2;
			}
			break;
		case 4:
			if (y_bus == y2_lane - 15)
			{
				++x_bus;
				line_num = 3;
				break;
			}
			++y_bus;
			if (y_bus == y2_lane - 15)
			{
				line_num = 3;
			}
			break;
		}
	}
}

void findtarget_sstf(void)
{
	int cursta = bus.position / distance + 1;//当前站点
	int tmp1 = cursta, tmp2 = cursta, i = 0;
	if (req.clockwise[cursta] || req.counterclockwise[cursta] || req.target[cursta])
	{
		bus.targetstation = cursta;
		return;
	}
	for (i = 1; i <= totalstation; ++i)
	{
		if (tmp1 == totalstation)
		{
			tmp1 = 1;
		}
		else
		{
			++tmp1;
		}
		if (tmp2 == 1)
		{
			tmp2 = totalstation;
		}
		else
		{
			--tmp2;
		}
		if (req.clockwise[tmp1] || req.counterclockwise[tmp1] || req.target[tmp1])
		{
			bus.targetstation = tmp1;
			return;
		}
		else if (req.clockwise[tmp2] || req.counterclockwise[tmp2] || req.target[tmp2])
		{
			bus.targetstation = tmp2;
			return;
		}
	}
}

int judgeby_sstf(void)
{
	int dir = bus.direction, sta = bus.position / distance + 1;
	if (dir == CLOCKWISE)
	{
		return ((req.clockwise[sta]) | (req.target[sta]));
	}
	else
	{
		return ((req.counterclockwise[sta]) | (req.target[sta]));
	}
}

void findtarget_scan(void)
{
	int trasta = bus.position / distance + 1;
	while (1)
	{
		if (bus.direction == CLOCKWISE)
		{
			if (trasta == totalstation)
			{
				trasta = 1;
			}
			else
			{
				++trasta;
			}
		}
		else
		{
			if (trasta == 1)
			{
				trasta = totalstation;
			}
			else
			{
				--trasta;
			}
		}
		if ((req.clockwise[trasta] | req.counterclockwise[trasta]) | req.target[trasta])
		{
			bus.targetstation = trasta;
			return;
		}
	}
}

int judgeby_scan(void)
{
	int sta = bus.position / distance + 1;
	return (req.clockwise[sta] | req.counterclockwise[sta]) | req.target[sta];
}

