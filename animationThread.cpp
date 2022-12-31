#include "GlobalConstantAndStruct.h"
#include "animationThread.h"

extern int totalstation, distance, strategy;
extern int tot_pos;
extern char row1[30], row2[30], row3[30], row4[30], row5[30];
extern char target[20], clockwise[20], counterclockwise[20];
extern int lane_width, lane_height;
extern int x1_lane, y1_lane, x2_lane, y2_lane;
extern int x_bus, y_bus;
extern int req_;
extern int target_, clockwise_, counterclockwise_;
extern int target__, clockwise__, counterclockwise__;
extern bool blink_t, blink_c, blink_u;
extern int blink_;
extern POINT_ pos[100];
extern REQUEST req;
extern BUS bus;
extern std::queue<std::pair<char, int>> req_list;
extern IMAGE bus_, stop1, stop2, stop3, stop4;
extern bool end;
extern int line_num;
extern int time__;

void calculate_lane(void)
{
	tot_pos = totalstation * distance;
	int tot_pixel = tot_pos * 50;//tot_pixel一定是2倍数
	if (tot_pixel % 4)
	{
		lane_width = tot_pixel / 4 + 1;
		lane_height = tot_pixel / 4;
	}
	else
	{
		lane_width = lane_height = tot_pixel / 4;
	}
}

void draw_point(void)
{
	setfillcolor(BLUE);
	int x = x1_lane, y = y1_lane;
	int state = 1, num = 0;
	while (true)
	{
		switch (state)
		{
		case 1:
			if (num % distance == 0)
			{
				settextstyle(15, 0, "宋体");
				outtextxy(x - 10, y - 60, num / distance + 1 + '0');
				putimage(x - 10, y - 46 - 1, &stop1);
			}
			fillcircle(x, y, 4);
			pos[num].x = x, pos[num].y = y;
			++num;
			if (x + 50 <= x2_lane)
			{
				x += 50;
				if (x == x2_lane)
				{
					state = 2;
				}
			}
			else
			{
				y += (x2_lane - x);
				x = x2_lane;
				state = 2;
			}
			break;
		case 2:
			if (num % distance == 0)
			{
				settextstyle(15, 0, "宋体");
				outtextxy(x + 55, y - 10, num / distance + 1 + '0');
				putimage(x + 1, y - 10, &stop2);
			}
			fillcircle(x, y, 4);
			pos[num].x = x, pos[num].y = y;
			++num;
			if (y + 50 <= y2_lane)
			{
				y += 50;
				if (y == y2_lane)
				{
					state = 3;
				}
			}
			else
			{
				x -= (y2_lane - y);
				y = y2_lane;
				state = 3;
			}
			break;
		case 3:
			if (num % distance == 0)
			{
				settextstyle(15, 0, "宋体");
				outtextxy(x, y + 50, num / distance + 1 + '0');
				putimage(x - 10, y + 1, &stop3);
			}
			fillcircle(x, y, 4);
			pos[num].x = x, pos[num].y = y;
			++num;
			if (x - 50 >= x1_lane)
			{
				x -= 50;
				if (x == x1_lane)
				{
					state = 4;
				}
			}
			else
			{
				y -= (x - x1_lane);
				x = x1_lane;
				state = 4;
			}
			break;
		case 4:
			if (num % distance == 0)
			{
				settextstyle(15, 0, "宋体");
				if (num / distance + 1 >= 10)
				{
					char tmp[5];
					tmp[0] = (num / distance + 1) / 10 + '0';
					tmp[1] = (num / distance + 1) % 10 + '0';
					tmp[2] = '\0';
					outtextxy(x - 55, y + 10, tmp);
				}
				else
				{
					outtextxy(x - 55, y + 10, num / distance + 1 + '0');
				}

				putimage(x - 46 - 1, y - 10, &stop4);
			}
			fillcircle(x, y, 4);
			pos[num].x = x, pos[num].y = y;
			++num;
			if (y - 50 > y1_lane)
			{
				y -= 50;
			}
			else
			{
				y = y1_lane;
				break;
			}
		}
		if (state == 4 && y == y1_lane)
		{
			break;
		}
	}
}

void animation(void)
{
	IMAGE button_T, button_C, button_U, button_END;
	initgraph(1200, 600);
	loadimage(&button_T, "button_T.jpg", 50, 50);
	loadimage(&button_C, "button_C.jpg", 50, 50);
	loadimage(&button_U, "button_U.jpg", 50, 50);
	loadimage(&button_END, "button_END.jpg", 50, 50);
	loadimage(&bus_, "bus.png", 30, 30);
	loadimage(&stop1, "stop1.png", 20, 46);
	loadimage(&stop2, "stop2.png", 46, 20);
	loadimage(&stop3, "stop3.png", 20, 46);
	loadimage(&stop4, "stop4.png", 46, 20);
	calculate_lane();
	x1_lane = 350 - lane_width / 2, y1_lane = 300 - lane_height / 2;
	x2_lane = x1_lane + lane_width, y2_lane = y1_lane + lane_height;
	x_bus = x1_lane - 15, y_bus = y1_lane - 15;
	row3[7 + totalstation] = '\0', row4[10 + totalstation] = '\0', row5[17 + totalstation] = '\0';
	BeginBatchDraw();
	while (true)
	{
		//设置背景
		setbkcolor(WHITE);
		cleardevice();
		//动画区
		setlinecolor(BLACK);
		setlinestyle(PS_SOLID, 2);
		rectangle(x1_lane, y1_lane, x2_lane, y2_lane);
		draw_point();
		//输出公交车
		putimage(x_bus, y_bus, &bus_);
		//输出区
		setlinestyle(PS_DASH, 2);
		roundrect(720, 25, 1150, 290, 20, 20);
		settextstyle(30, 0, "宋体");
		settextcolor(BLACK);
		setbkmode(TRANSPARENT);
		int height = 45;
		outtextxy(740, 50, row1);
		outtextxy(740, 50 + height, row2);
		outtextxy(740, 50 + 2 * height, row3);
		outtextxy(740, 50 + 3 * height, row4);
		outtextxy(740, 50 + 4 * height, row5);
		//输入区
		roundrect(720, 310, 1150, 575, 20, 20);
		putimage(740, 335, &button_T);
		putimage(740, 395, &button_C);
		putimage(740, 455, &button_U);
		putimage(740, 515, &button_END);
		setfillcolor(GRAY);
		solidrectangle(820, 335, 920, 385);
		solidrectangle(820, 395, 920, 445);
		solidrectangle(820, 455, 920, 505);
		outtextxy(823, 340 + 5, target);
		outtextxy(823, 400 + 5, clockwise);
		outtextxy(823, 460 + 5, counterclockwise);
		if (blink_t)
		{
			if (blink_ % 1000 < 500)//光标亮
			{
				++blink_;
				setfillcolor(BLACK);
				solidrectangle(825 + target__ * 15, 340, 827 + target__ * 15, 380);
			}
			else
			{
				++blink_;
				setfillcolor(GRAY);
				solidrectangle(825 + target__ * 15, 340, 827 + target__ * 15, 380);
			}
		}
		else if (blink_c)
		{
			if (blink_ % 1000 < 500)//光标亮
			{
				++blink_;
				setfillcolor(BLACK);
				solidrectangle(825 + clockwise__ * 15, 400, 827 + clockwise__ * 15, 440);
			}
			else
			{
				++blink_;
				setfillcolor(GRAY);
				solidrectangle(825 + clockwise__ * 15, 400, 827 + clockwise__ * 15, 440);
			}
		}
		else if (blink_u)
		{
			if (blink_ % 1000 < 500)//光标亮
			{
				++blink_;
				setfillcolor(BLACK);
				solidrectangle(825 + counterclockwise__ * 15, 460, 827 + counterclockwise__ * 15, 500);
			}
			else
			{
				++blink_;
				setfillcolor(GRAY);
				solidrectangle(825 + counterclockwise__ * 15, 460, 827 + counterclockwise__ * 15, 500);
			}
		}
		FlushBatchDraw();
	}
	closegraph();
}
