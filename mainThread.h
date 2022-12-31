#pragma once

#include <iostream>
#include <windows.h>
#include <queue>
#include <utility>
#include <easyx.h>
#include <conio.h>

#include "animationThread.cpp"
#include "getinputThread.cpp"
#include "timeThread.cpp"

inline void init(void);
inline void readfile(void);
inline void fcfs(void);
inline void sstf(void);
inline void scan(void);
inline int find_next_position(int cur);
inline void directionjudge(void);
inline void run_pixel(void);
inline void findtarget_sstf(void);
inline int judgeby_sstf(void);
inline void findtarget_scan(void);
inline int judgeby_scan(void);