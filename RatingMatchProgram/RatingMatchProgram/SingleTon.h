#pragma once

#include <mutex>

template < class T >
class SingleTon
{
private:
	static T* m_Instance;
	static std::mutex m_mutex;

public:
	static T* getInstance()
	{
		std::lock_guard<std::mutex> guard(m_mutex);
		if (!m_Instance)
			m_Instance = new T;

		return m_Instance;
	}

	static void releaseInstance()
	{
		if (m_Instance != nullptr)
		{
			delete m_Instance;
			m_Instance = nullptr;
		}
	}
};

template < class T >
T* SingleTon<T>::m_Instance = nullptr;

template < class T >
std::mutex SingleTon<T>::m_mutex;