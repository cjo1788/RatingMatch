#pragma once
#include <iostream>
#include <stdlib.h>
#include <WinSock2.h>
#include <list>
#include <time.h>
#include <thread>
#include <queue>
#include <map>

using namespace std;

#pragma comment(lib, "winmm.lib")

#pragma region const 사용
const int iRating_Max = 3000;
const int iRating_Min = 1000;
const int iMatch_Rating_Default = 10;
const int iMatching_Min_Time = 3000;
const int iMatch_Room_Count = 100;
const int iFree_Match_Count = 1000;
#pragma endregion

#pragma region define 사용
//#define DF_RATING_MAX	3000
//#define DF_RATING_MIN	1000
//#define DF_MATCH_RATING_DEFAULT 10
//#define DF_MATCHING_MIN_TIME	3000
//#define DF_MAX_ROOM_COUNT		100
//#define DF_FREE_MATCH_COUNT		1000
#pragma endregion

#pragma region enum 사용
//enum {
//	RATING_MAX = 3000,
//	RATING_MIN = 1000,
//	MATCH_RATING_DEFAULT = 10,
//	MATCHING_MIN_TIME = 3000,
//	MATCH_ROOM_COUNT = 100,
//	FREE_MATCH_COUNT = 1000
//};
#pragma endregion

