

#ifdef WIN32
//#define  _WIN32_WINNT  (0x0400)

#include <process.h>
#include <windows.h>
#else
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#endif
#include <stdio.h>

#include "Thread.hpp"
#include <iostream>

/* *******************************************************************
* @描述: CThread--- 完成线程对象数据成员的初始化;
*
* @参数: szDescription---线程的功能描述,默认值为空串;
* @参数: uiPriority---线程的优先级;
* @参数: pThreadMgr---要注册的线程管理容器的指针;
* @参数: uiWorkTime---线程的业务处理时间,默认值为0，表
*					   示不设置业务处理时间,单位为秒;
* *******************************************************************/
CThread::CThread(pThreadAttrib pAttrib, CThreadMgr *pThreadMgr, 
				 unsigned uiWorkTime)
{
	m_thread_handle = 0;
	m_uiThreadId = 0;
	m_pThreadMgr = pThreadMgr;
	if (NULL == pAttrib)
	{
		m_szDescription = "";
		m_uiPriority = 0;
		m_iSysPriority =0;
	}
	else
	{
		m_szDescription = pAttrib->szDescription;
		m_uiPriority = pAttrib->ui_ClrPriority; 
		m_iSysPriority = pAttrib->sysPriority;
	}		

	m_uiWorkTime = uiWorkTime;
	if (uiWorkTime > 0)
	{
		m_ThreadState = e_TS_NORMAL;
	}
	else
	{
		m_ThreadState = e_TS_UNKONWN;
	}
	m_LastHeartBeat = 0;

}

CThread::~CThread()
{	
	setThreadState(e_TS_EXIT);
#ifndef WIN32
	pthread_attr_destroy(&m_threadAttrib);
#endif
	stop();
}

/* ******************************************************************
* @描述：open--- 完成线程的创建工作，并调用regThread函数在线程
*		         管理类对象中进行注册登记;
*
* @参数：args--- 默认为空,暂做保留;
*
* @返回值：0表示成功,1表示失败;
********************************************************************/
int CThread::open(void *args)
{
	if (0 == m_thread_handle) 
	{
		m_thread_args = args;
#ifdef WIN32        

		m_thread_handle = (HANDLE)_beginthreadex(0, 0, thread_proxy, (void*)this, 0, &m_uiThreadId);

		//设置线程优先级
		switch (m_iSysPriority)
		{
		case THREAD_PRIORITY_IDLEX :
			SetThreadPriority(reinterpret_cast<HANDLE>(m_thread_handle), THREAD_PRIORITY_IDLE);
			break;
		case THREAD_PRIORITY_ERROR_RETURNX:
			SetThreadPriority(reinterpret_cast<HANDLE>(m_thread_handle), THREAD_PRIORITY_ERROR_RETURN);
			break;
		case THREAD_PRIORITY_LOWESTX :
			SetThreadPriority(reinterpret_cast<HANDLE>(m_thread_handle), THREAD_PRIORITY_LOWEST);
			break;
		case THREAD_PRIORITY_BELOW_NORMALX :
			SetThreadPriority(reinterpret_cast<HANDLE>(m_thread_handle), THREAD_PRIORITY_BELOW_NORMAL);
			break;
		case THREAD_PRIORITY_ABOVE_NORMALX :
			SetThreadPriority(reinterpret_cast<HANDLE>(m_thread_handle), THREAD_PRIORITY_ABOVE_NORMAL);
			break;
		case THREAD_PRIORITY_HIGHESTX :
			SetThreadPriority(reinterpret_cast<HANDLE>(m_thread_handle), THREAD_PRIORITY_HIGHEST);
			break;
		case THREAD_PRIORITY_TIME_CRITICALX:
			SetThreadPriority(reinterpret_cast<HANDLE>(m_thread_handle), THREAD_PRIORITY_TIME_CRITICAL);
			break;
		default :
			SetThreadPriority(reinterpret_cast<HANDLE>(m_thread_handle), THREAD_PRIORITY_NORMAL);
		}


		sleep(100);
		//printf("thread_id is %x,thread handle is %x\n",m_uiThreadId,m_thread_handle);		
#else
		sched_param schedParam;
		schedParam.__sched_priority = m_iSysPriority;			

		//初始化pthread_attr_t结构参数;
		pthread_attr_init (&m_threadAttrib);

		//设置优先级
		pthread_attr_setschedparam(&m_threadAttrib, &schedParam);

		//创建线程
		int res = pthread_create(&m_thread_handle, &m_threadAttrib, &CThread::thread_proxy, (void*)this);	

		//pthread_attr_destroy ( threadAttrib );
		m_uiThreadId = m_thread_handle;
		sleep(100);
		//printf("thread_id is %ud \n",m_thread_handle);	
#endif	
		fflush(stdout);

		if (m_pThreadMgr)
		{
			regThread();
		}

		return (m_thread_handle != 0) ?0 :1;
	}//end if

	return 0;
}

/*******************************************************************
* @描述： stop--- 关闭线程,同时进行反注册;
*******************************************************************/
void CThread::stop()
{
	if (0 != m_thread_handle) 
	{
#ifdef WIN32

		WaitForSingleObject(HANDLE(m_thread_handle), INFINITE);
		CloseHandle(HANDLE(m_thread_handle));
#else
		pthread_join(m_thread_handle, 0);
#endif

		if (m_pThreadMgr)
		{
			unRegThread();
		}
		m_thread_handle = 0;
	}
}

/*********************************************************
* @描述：threadProxy ---线程执行函数;
*	     
* @返回值：Windows平台下设返回值，成功返回0;
*		   Linux平台有返回值;
********************************************************/
#ifdef WIN32
unsigned __stdcall CThread::thread_proxy(void *param)
#else
void* CThread::thread_proxy(void *param)
#endif
{
#ifndef WIN32
	sigset_t block_set;
	sigfillset(&block_set);
	pthread_sigmask(SIG_BLOCK, &block_set, 0);	
#endif
	CThread *pObj = (CThread*)param;	
	pObj->svc();	
	return 0;
}

/********************************************************
* @描述： sleep ---线程休眠函数;
*	     
* @参数:  dwMilliseconds---休眠时间,单位为毫秒;
*
* @返回值：Windows平台下设返回值，成功返回0;
*		   Linux平台有返回值;
********************************************************/
void CThread::sleep(unsigned long dwMilliseconds)
{
#ifdef WIN32
	::Sleep(dwMilliseconds);
#else
	timeval wait;
	wait.tv_sec = (dwMilliseconds / 1000);
	wait.tv_usec = (dwMilliseconds - ((long)(dwMilliseconds / 1000)) * 1000) * 1000;
	::select (0, 0, 0, 0, &wait);
#endif
}

unsigned CThread::getThreadId()
{
	return m_uiThreadId;
}

/*******************************************************
* @描述：getHeatBeat ---获得当前线程的末次心跳;
*	     
* @返回值: 末次心跳的时间,单位秒;
*
********************************************************/
time_t  CThread::getHeartBeat()
{
	return m_LastHeartBeat;
}

string CThread::getDescription()
{
	return m_szDescription;
}

/********************************************************
* @描述：getWorkTime ---获得当前线程的业务处理时间;
*	     
* @返回值: 线程的业务处理时间,单位秒;
*
********************************************************/
unsigned CThread::getWorkTime()
{
	return m_uiWorkTime;
}


E_RUN_STATE CThread::getThreadState()
{
	return m_ThreadState;
}

/*******************************************************************
* @描述：getPriority ---获得线程为线程管理器进行清空操作而设置的优先级;
*	     
* @返回值: 线程为清空操作而设置的优先级;
*
*******************************************************************/
unsigned CThread::getPriority()
{
	return m_uiPriority;
}

//获取线程关于系统内核设置的优先级
int  CThread::getSysPriority()
{
#ifdef WIN32
	return GetThreadPriority(m_thread_handle);
#else
	sched_param schedParam;
	pthread_attr_getschedparam(&m_threadAttrib, &schedParam);
	return schedParam.__sched_priority;
#endif
}

/********************************************************
* @描述：setThreadState ---设置线程对象运行状态;
*	     
* @参数: thrRunState ---线程运行时的状态;
*
********************************************************/

void CThread::setThreadState(E_RUN_STATE thrRunState)
{
	m_ThreadState = thrRunState;
}

/********************************************************
* @描述：sendHeartBeat ---向线程管理器发送心跳;
********************************************************/
void CThread::sendHeartBeat()
{
	m_LastHeartBeat = time(NULL);
}

/*******************************************************
* @描述：regThread ---向线程管理器注册线程;
********************************************************/
void CThread::regThread( )
{
	(m_pThreadMgr->m_ThreadList).insert(pair<unsigned, CThread*>(this->getThreadId(), this));
	(m_pThreadMgr->m_iThreadCount)++;
}

/********************************************************
* @描述：unregThread ---向线程管理器注销线程(反注册);
********************************************************/
void CThread::unRegThread()
{
	unsigned uiThreadId = this->getThreadId();
	if ((m_pThreadMgr->m_ThreadList).count(uiThreadId))
	{
		(m_pThreadMgr->m_ThreadList).erase(this->getThreadId());
		(m_pThreadMgr->m_iThreadCount)--;
	}
}

/********************************************************
* @描述：clearUp ---线程的清空处理;
********************************************************/	
int CThread::clearUp()
{
	return 0; //成功返回0;
}

