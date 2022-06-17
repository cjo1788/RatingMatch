#include "InGameManager.h"
#include "Room.h"
#include "Enum.h"

InGameThread::InGameThread()
{
	m_event = INVALID_HANDLE_VALUE;
	Initialize();
}

InGameThread::~InGameThread()
{
	CloseHandle(m_event);
}

void InGameThread::Initialize()
{
	m_event = CreateEvent(NULL, TRUE, FALSE, NULL);
}

int InGameThread::Run()
{
	while (WaitForSingleObject(m_event, 500) != 0)
	{
		CS(GLOBALINFO->m_UseRoomCS);
		for (auto itor = GLOBALINFO->m_UseRoomList.begin(); itor != GLOBALINFO->m_UseRoomList.end();)
		{
			Room* pRoom = (*itor);

			switch (pRoom->GetState())
			{
			case RoomState::ROOM_ENTER:
			{
				pRoom->EnterProcess();
				++itor;
			}
			break;

			case RoomState::ROOM_READY:
			{
				pRoom->ReadyProcess();
				++itor;
			}
			break;

			case RoomState::ROOM_STARTING:
			{
				pRoom->StartProcess();
				++itor;
			}
			break;

			case RoomState::ROOM_END:
			{
				pRoom->EndProcess();
				++itor;
			}
			break;

			case RoomState::ROOM_OVER:
			{
				itor = GLOBALINFO->m_UseRoomList.erase(itor);
				pRoom->Initialize();

				{
					CS(GLOBALINFO->m_FreeRoomCS);
					GLOBALINFO->m_FreeRoomList.push_back(pRoom);
				}
			}
			break;

			default:
				break;
			}
		}
	}

	return 0;
}

InGameManager::InGameManager()
{
	ingameThread = NULL;
}
InGameManager::~InGameManager()
{
	delete ingameThread;
	ingameThread = NULL;
}

void InGameManager::Initialize()
{
	ingameThread = new InGameThread();
	ingame_thread = thread(&InGameThread::Run, ingameThread);
}

void InGameManager::Run()
{
	ingame_thread.join();
}