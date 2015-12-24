//#define _WIN32_WINNT (0x0500)

#include "ResLock.hpp"



//初始化互斥锁 或 临界区
CResLock::CResLock()
{
#ifndef _WIN32
	if (0 != pthread_mutex_init(&m_mutex, NULL))
	{
		perror("pthread_mutex_init error");
	}	
#else
	::InitializeCriticalSection(&m_mutex);
#endif		
}


//销毁互斥锁 或 临界区
CResLock::~CResLock()
{
#ifndef _WIN32
	if ((0 != pthread_mutex_destroy(&m_mutex)) && \
		(EBUSY != pthread_mutex_destroy(&m_mutex)))
	{
		perror("pthread_mutex_destroy error");
	}
#else
	::DeleteCriticalSection(&m_mutex);
#endif	
}


//加互斥锁 或 进入临界区
void CResLock::lock()
{
#ifndef _WIN32
    if (0 != pthread_mutex_lock(&m_mutex))
    {
        perror("pthread_mutex_lock error");
        switch (errno)
        {
            case EINVAL:
                printf("[ERROR]: EINVAL.\n");
                break;
            case EBUSY:
                printf("[ERROR]: EBUSY. \n");
                break;
            case EAGAIN:
                printf("[ERROR]: EAGAIN. \n");
                break;
            case EDEADLK:
                printf("[ERROR]: EDEADLK. \n");
                break;
            case EPERM:
                printf("[ERROR]:EPERM. \n");
                break;
            default:
                ;
        }
    }
#else
    ::EnterCriticalSection(&m_mutex);
#endif
}


//互斥锁测试加锁 或 尝试进入临界区
int CResLock::tryLock()
{
	int iValue = 0;
#ifndef _WIN32
	iValue = pthread_mutex_trylock(&m_mutex);	//获取测试锁到返回值
	if ((0 != iValue) && (EBUSY != iValue))
	{
		perror("pthread_mutex_trylock error");
	}
#else
	iValue = (int)(!::TryEnterCriticalSection(&m_mutex)); //成功则返回0
#endif
	return iValue;	//返回加测试锁状态或临界区状态
}


//互斥锁解锁 或 离开临界区
void CResLock::unLock()
{
#ifndef _WIN32
	if (0 != pthread_mutex_unlock(&m_mutex))
	{
		perror("pthread_mutex_unlock error");
	}
#else
	::LeaveCriticalSection(&m_mutex);
#endif
}
