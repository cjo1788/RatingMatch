#pragma once

#include "Global.h"
#include "SingleTon.h"
#include "CriticalSection.h"
#include "User.h"
#include "Struct.h"

class User;
class Room;

struct WorkInfo
{
	User* m_User;
	DWORD m_MatchRatingMax;
	DWORD m_MatchRatingMin;
	DWORD m_dwMatchingTime;

	WorkInfo()
	{
		m_User = nullptr;
		m_MatchRatingMax = 0;
		m_MatchRatingMin = 0;
		m_dwMatchingTime = 0;
	}

	~WorkInfo()
	{
		m_User = nullptr;
		printf("WorkInfo º“∏Í¿⁄ »£√‚!!!\n");
		Sleep(10);
	}

	void Initialize()
	{

	}

	void PackingWorkInfo(User* user, RatingInfo ratinginfo, DWORD time)
	{
		m_User = user;
		m_MatchRatingMax = ratinginfo.MaxRating;
		m_MatchRatingMin = ratinginfo.MinRating;
		m_dwMatchingTime = time;
	}

	void AddRatingMax(DWORD dwValue)
	{
		m_MatchRatingMax += dwValue;
	}

	DWORD GetRatingMax()
	{
		return m_MatchRatingMax;
	}

	void DisRatingMin(DWORD dwValue)
	{
		m_MatchRatingMin -= dwValue;
	}

	DWORD GetRatingMin()
	{
		return m_MatchRatingMin;
	}


};

class GlobalInfo : public SingleTon<GlobalInfo>
{
public:
	GlobalInfo();
	virtual ~GlobalInfo();

	void Initialize();

	Room* CreateRoom();
	void InitRoom(Room* pRoom);

	WorkInfo* CreateWorkInfo();

	using return_itor = typename list<WorkInfo*>::iterator;
	return_itor InitWorkInfo(WorkInfo* work);

	void SortMatchList();
	void PrintMatchList();

	CS m_UserCS;
	list<User*> m_UserList;

	CS m_FreeMatchCS;
	list<WorkInfo*> m_FreeMatchList;

	CS m_MatchCS;
	list<WorkInfo*> m_MatchList;

	CS m_FreeRoomCS;
	list<Room*> m_FreeRoomList;

	CS m_UseRoomCS;
	list<Room*> m_UseRoomList;
};

#define GLOBALINFO GlobalInfo::getInstance()