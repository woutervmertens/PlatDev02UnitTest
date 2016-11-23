//Only works in Visual Studio 2013
//C++11 > Variadic Templates (not supported in VS2012)
//http://msdn.microsoft.com/en-us/library/vstudio/hh567368.aspx

//#pragma once
//#include <stdarg.h>
//#include <iostream>
//using namespace std;
//
//template<class T>
//class Singleton
//{
//public:
//	
//	template<typename... Args>
//	static T* GetInstance(Args... args)
//	{
//		if(!m_Instance)
//		{
//			m_Instance = new T(forward<Args>(args)...);
//		}
//
//		return m_Instance;
//	}
//
//	static void DestroyInstance()
//	{
//		delete m_Instance;
//		m_Instance = nullptr;
//	}
//
//private:
//	static T* m_Instance;
//};
//
//template<class T> 
//T* Singleton<T>::m_Instance = nullptr;

