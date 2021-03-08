#pragma once
#include <iostream>
#include <stdlib.h>
#include <WinSock2.h>
#include <list>
#include <time.h>
#include <thread>
#include <queue>

using namespace std;

#pragma comment(lib, "winmm.lib")

#define DF_RATING_MAX	3000
#define DF_RATING_MIN	1000
#define DF_MATCH_RATING_DEFAULT 10
#define DF_MATCHING_MIN_TIME	3000
#define DF_MAX_ROOM_COUNT		100
#define DF_FREE_MATCH_COUNT		1000