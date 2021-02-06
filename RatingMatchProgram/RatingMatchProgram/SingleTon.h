#pragma once

template < class T >
class SingleTon
{
private:
	static T* m_Instance;

public:
	static T* getInstance()
	{
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