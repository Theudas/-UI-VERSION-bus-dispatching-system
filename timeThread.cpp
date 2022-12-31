#include "GlobalConstantAndStruct.h"
#include "timeThread.h"

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

void time_(void)
{
	int i = 1;
	while (end == false)
	{
		if (bus.state == FREE)
		{
			Sleep(1000);
			++i;
			if (i == 5)
			{
				i = 1;
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
		}
		else
		{
			i = 1;
		}
	}
	return;
}
