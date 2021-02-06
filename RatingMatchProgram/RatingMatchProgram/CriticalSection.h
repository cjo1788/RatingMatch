#pragma once
#include <iostream>
#include <windows.h>

class CriticalSection
{
public:
	class CSObject
	{
	private:
		CRITICAL_SECTION cs;

	public:
		CSObject()
		{
			if (!InitializeCriticalSectionAndSpinCount(&cs, 4000))
			{
				printf("Error : %d\n", GetLastError());
			}
		}

		virtual ~CSObject()
		{
			DeleteCriticalSection(&cs);
		}

		inline void enter()
		{
			EnterCriticalSection(&cs);
		}

		inline void leave()
		{
			LeaveCriticalSection(&cs);
		}
	};

private:
	CSObject* m_CSObject;

public:
	CriticalSection(CSObject* ob)
	{
		m_CSObject = ob;
		ob->enter();
	}

	virtual ~CriticalSection()
	{
		m_CSObject->leave();
	}
};

#define SYNC(p)	CriticalSection cs(&p)
#define CS CriticalSection::CSObject