#include "JoinManager.h"
#include "GlobalInfo.h"
#include "User.h"
#include "Enum.h"
#include "Struct.h"

JoinThread::JoinThread()
{
	m_event = INVALID_HANDLE_VALUE;
	Initialize();
}

JoinThread::~JoinThread()
{
	CloseHandle(m_event);
}

void JoinThread::Initialize()
{
	m_event = CreateEvent(NULL, TRUE, FALSE, NULL);
}

int JoinThread::Run()
{
	while (WaitForSingleObject(m_event, 500) != 0)
	{
		CS(GLOBALINFO->m_UserCS);

		for (auto userInfo : GLOBALINFO->m_UserList)
		{
			if (userInfo->GetState() != UserState::READY)
				continue;

			auto UserRatingInfo = GetUserRatingInfo(userInfo->GetRating());

			userInfo->SetState(UserState::MATCHING);

			auto work = GLOBALINFO->CreateWorkInfo();
			work->PackingWorkInfo(userInfo, UserRatingInfo, timeGetTime());

			{
				CS(GLOBALINFO->m_MatchCS);
				GLOBALINFO->m_MatchList.push_back(work);
			}
		}
	}

	return 0;
}

void JoinThread::PackingUserMatchInfo(WorkInfo* work )
{

}

RatingInfo JoinThread::GetUserRatingInfo(DWORD rating)
{
	RatingInfo result;
	int iRatingMax = 0;
	int iRatingMin = 0;

	if (rating - iMatch_Rating_Default < 0)
	{
		iRatingMin = 0;
	}
	else
	{
		iRatingMin = rating - iMatch_Rating_Default;
	}

	iRatingMax = rating + iMatch_Rating_Default;

	result.MaxRating = iRatingMax;
	result.MinRating = iRatingMin;

	return result;
}

JoinManager::JoinManager()
{
	joinThread = NULL;
}

JoinManager::~JoinManager()
{
	delete joinThread;
	joinThread = NULL;
}

void JoinManager::Initialize()
{
	joinThread = new JoinThread();
	join_thread = thread(&JoinThread::Run, joinThread);
}

void JoinManager::Run()
{
	join_thread.join();
}