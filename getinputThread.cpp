#include "GlobalConstantAndStruct.h"
#include "getinputThread.h"

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

void add_(int num)
{
	if (req_ == TARGET)
	{
		target[target__] = num + '0';
		++target__;
		target[target__] = '\0';
		target_ = target_ * 10 + num;
	}
	else if (req_ == CLOCKWISE)
	{
		clockwise[clockwise__] = num + '0';
		++clockwise__;
		clockwise[clockwise__] = '\0';
		clockwise_ = clockwise_ * 10 + num;
	}
	else if (req_ == COUNTERCLOCKWISE)
	{
		counterclockwise[counterclockwise__] = num + '0';
		++counterclockwise__;
		counterclockwise[counterclockwise__] = '\0';
		counterclockwise_ = counterclockwise_ * 10 + num;
	}

}

void getinput(void)
{
	ExMessage msg;
	while (true)
	{
		if (peekmessage(&msg, EM_MOUSE))
		{
			switch (msg.message)
			{
			case WM_LBUTTONDOWN:
				if (msg.x >= 820 && msg.x <= 920 && msg.y >= 335 && msg.y <= 385)//T后按钮
				{
					req_ = TARGET;
					blink_c = false, blink_u = false;
					blink_t = true;
					blink_ = 0;
				}
				else if (msg.x >= 820 && msg.x <= 920 && msg.y >= 395 && msg.y <= 445)//C后按钮
				{
					req_ = CLOCKWISE;
					blink_t = false, blink_u = false;
					blink_c = true;
					blink_ = 0;
				}
				else if (msg.x >= 820 && msg.x <= 920 && msg.y >= 455 && msg.y <= 505)
				{
					req_ = COUNTERCLOCKWISE;
					blink_c = false, blink_t = false;
					blink_u = true;
					blink_ = 0;
				}
				else if (msg.x >= 740 && msg.x <= 790 && msg.y >= 335 && msg.y <= 385)
				{
					target__ = 0;
					target[0] = '\0';
					if (target_ >= 1 && target_ <= totalstation && req.target[target_] == 0)
					{
						req.target[target_] = 1;
						req_list.push(std::make_pair('T', target_));
						row3[6 + target_] = '1';
					}
					else if (target_ < 1 || target_ > totalstation)
					{
						HWND hnd1 = GetHWnd();
						SetWindowText(hnd1, "提示");
						MessageBox(hnd1, "请输入正确数据", "提示", MB_OKCANCEL);
					}
					target_ = 0;
				}
				else if (msg.x >= 740 && msg.x <= 790 && msg.y >= 395 && msg.y <= 445)
				{
					clockwise__ = 0;
					clockwise[0] = '\0';
					if (clockwise_ >= 1 && clockwise_ <= totalstation && req.clockwise[clockwise_] == 0)
					{
						req.clockwise[clockwise_] = 1;
						req_list.push(std::make_pair('C', clockwise_));
						row4[9 + clockwise_] = '1';
					}
					else if (clockwise_ < 1 || clockwise_ > totalstation)
					{
						HWND hnd2 = GetHWnd();
						SetWindowText(hnd2, "提示");
						MessageBox(hnd2, "请输入正确数据", "提示", MB_OKCANCEL);
					}
					clockwise_ = 0;
				}
				else if (msg.x >= 740 && msg.x <= 790 && msg.y >= 455 && msg.y <= 505)
				{
					counterclockwise__ = 0;
					counterclockwise[0] = '\0';
					if (counterclockwise_ >= 1 && counterclockwise_ <= totalstation && req.counterclockwise[counterclockwise_] == 0)
					{
						req.counterclockwise[counterclockwise_] = 1;
						req_list.push(std::make_pair('U', counterclockwise_));
						row5[16 + counterclockwise_] = '1';
					}
					else if (counterclockwise_ < 1 || counterclockwise_ > totalstation)
					{
						HWND hnd3 = GetHWnd();
						SetWindowText(hnd3, "提示");
						MessageBox(hnd3, "请输入正确数据", "提示", MB_OKCANCEL);
					}
					counterclockwise_ = 0;
				}
				else if (msg.x >= 740 && msg.x <= 790 && msg.y >= 515 && msg.y <= 565)
				{
					row1[0] = 'E', row1[1] = 'N', row1[2] = 'D', row1[3] = '\0';
					row2[0] = '\0';
					row3[0] = '\0';
					row4[0] = '\0';
					row5[0] = '\0';
					Sleep(1000);
					end = true;
				}
				break;
			default:
				break;
			}
		}
		else if (_kbhit() && req_ != 0)
		{
			char key = _getch();
			switch (key)
			{
			case 8:
			{
				if (req_ == TARGET)
				{
					if (target__ > 0)
					{
						target_ /= 10;
						--target__;
						target[target__] = '\0';
					}
				}
				else if (req_ == CLOCKWISE)
				{
					if (clockwise__ > 0)
					{
						clockwise_ /= 10;
						--clockwise__;
						clockwise[clockwise__] = '\0';
					}
				}
				else if (req_ == COUNTERCLOCKWISE)
				{
					if (counterclockwise__ > 0)
					{
						counterclockwise_ /= 10;
						--counterclockwise__;
						counterclockwise[counterclockwise__] = '\0';
					}
				}
				break;
			}
			case '0':
			{
				add_(0);
				break;
			}
			case '1':
			{
				add_(1);
				break;
			}
			case '2':
			{
				add_(2);
				break;
			}
			case '3':
			{
				add_(3);
				break;
			}
			case '4':
			{
				add_(4);
				break;
			}
			case '5':
			{
				add_(5);
				break;
			}
			case '6':
			{
				add_(6);
				break;
			}
			case '7':
			{
				add_(7);
				break;
			}
			case '8':
			{
				add_(8);
				break;
			}
			case '9':
			{
				add_(9);
				break;
			}
			}
		}
	}
}
