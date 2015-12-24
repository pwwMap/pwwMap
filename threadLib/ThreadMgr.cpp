#include "ThreadMgr.hpp"
#include "Thread.hpp"
#include <iomanip>
#include <iostream>



CThreadMgr* CThreadMgr::m_pThreadMgr = NULL;  //类静态成员变量初始化;

CThreadMgr::CThreadMgr()
{
	m_iThreadCount = 0;
}

CThreadMgr::~CThreadMgr()
{
	if (m_pThreadMgr)
	{
		delete m_pThreadMgr;
		m_pThreadMgr = NULL;
	}	
}

/********************************************************
* @描述: createInstance ---创建线程管理器的实例指针.
*
* @参数: logObj ---日志类对象的引用.
*
* @返回值: 返回线程管理器对象的指针.
********************************************************/
CThreadMgr* CThreadMgr::createInstance()
{
	if (NULL == m_pThreadMgr)
	{
		m_pThreadMgr = new CThreadMgr();	  
	}
	return m_pThreadMgr;
}

int CThreadMgr::getThreadCount()
{   
	return m_iThreadCount;
}

/********************************************************
* @描述: checkThread(不含参) ---监测所有线程对象的状态;
********************************************************/ 
void CThreadMgr::checkThread()
{
	map<unsigned, CThread*>::iterator it = m_ThreadList.begin();
	for (; it != m_ThreadList.end(); it++)
	{
		checkThread(*(it->second));	  
	}
}

/**************************************************************
* @描述: checkThread(含参) ---监测指定线程对象的状态;
*
* @参数: threadObj ---线程对象的引用;
*
* @返回值: 返回线程运行的状态,请参考头文件中关于宏的定义说明;
**************************************************************/  
int CThreadMgr::checkThread(CThread &threadObj)
{	
	int iStateVal = TS_RUN_NOEXIST;		//状态返回值;
	if (0 == (m_pThreadMgr->m_ThreadList).count(threadObj.getThreadId()))
		return iStateVal;

	unsigned  uiWorkTime  = threadObj.getWorkTime();
	unsigned  uiLastAlive = (unsigned)threadObj.getHeartBeat();
	unsigned  uiCurTime   = time(NULL);	
	string    szException = STR_STATE_UNKNOWN;  //状态显示;

	if (uiWorkTime != 0)
	{
		//对线程对象状态的判断及设置.	
		switch (threadObj.getThreadState()) 
		{
		case e_TS_EXCEPTION:
			szException = STR_STATE_EXCEPTION; 
			iStateVal   = TS_RUN_EXCEPTION;
			break;
		case e_TS_UNKONWN:
		case e_TS_NORMAL:
		case e_TS_TIMEOUT:	
			if (uiCurTime - uiLastAlive > uiWorkTime) 	
			{
				threadObj.setThreadState(e_TS_TIMEOUT);
				szException = STR_STATE_TIMEOUT;
				iStateVal   = TS_RUN_TIMEOUT;
			}
			else
			{		  
				threadObj.setThreadState(e_TS_NORMAL);	
				iStateVal = TS_RUN_NORMAL;
				return iStateVal;
			};
			break;
		default: ;	    
		} 
	}  //end of if
	else
	{
		iStateVal = TS_RUN_UNKNOWN;
	}

	////对异常写入日志(主要包括e_TS_EXCEPTION,e_TS_UNKNOWN,e_TS_TIMEOUT);
	//m_pLog->printLog(2, "%-d  %-s  %-d  %-s  %-s",
	//	           threadObj.getThreadId(), 
	//			   (lastAlive.ToString()).c_str(), 
	//			   uiWorkTime, 
	//			   szException.c_str(), 
	//			   (threadObj.getDescription()).c_str()
	//			   );

	return iStateVal;
}

/**************************************************************
* @描述: printStateInfo(空参) ---终端打印所有线程的运行状态;
**************************************************************/ 
void CThreadMgr::printStateInfo()
{
	map<unsigned, CThread*>::iterator it = m_ThreadList.begin();

	for (; it != m_ThreadList.end(); ++it)
	{
		printStateInfo(*(it->second));
	} 
}

/**************************************************************
* @描述: printStateInfo(含参) ---终端打印指定线程的运行状态;
*
* @参数: threadObj ---线程对象的引用;
*
**************************************************************/ 
void CThreadMgr::printStateInfo( CThread & threadObj)
{
	unsigned uiWorkTime = threadObj.getWorkTime();
	unsigned uiLastAlive = (unsigned)threadObj.getHeartBeat();
	unsigned uiCurTime = time(NULL);
	string szState;
	string   szDescription = threadObj.getDescription();
	E_RUN_STATE eRunState = threadObj.getThreadState();//线程的运行状态


	cout << setiosflags(ios::left) << threadObj.getThreadId() << "  ";
	cout << setiosflags(ios::left) << uiWorkTime << "  ";

	switch (eRunState )
	{
	case e_TS_UNKONWN : 
		szState = STR_STATE_UNKNOWN;
		break;
	case e_TS_EXCEPTION:
		szState = STR_STATE_EXCEPTION;
		break;
	case e_TS_NORMAL:
		szState = STR_STATE_NORMAL;
		break;
	case e_TS_TIMEOUT:
		szState = STR_STATE_TIMEOUT;
	}

	//为了状态信息长度一致,设定统一宽度
	cout << setw(8) << setiosflags(ios::left) << szState << " ";
	cout << setiosflags(ios::left) << szDescription << endl;	
}

/******************************************************************
* @描述: clearTransaction ---在各个线程处理事务之前所做的清空操作;
*******************************************************************/ 
int CThreadMgr::clearUp()
{
	map<unsigned, CThread*>::iterator it_ID = m_ThreadList.begin();
	map<unsigned, CThread*>::reverse_iterator it_Priority;
	map<unsigned, CThread*> map_Priority;
	unsigned uipriority = 0;

	try 
	{
		for (; it_ID != m_ThreadList.end(); it_ID++)
		{
			uipriority = (it_ID->second)->getPriority();
			map_Priority.insert(pair<unsigned, CThread*>(uipriority,it_ID->second));
		}

		it_Priority = map_Priority.rbegin();
		for ( ; it_Priority != map_Priority.rend(); it_Priority++)	
		{
			(it_Priority->second)->clearUp();
		}
		return 0;
	}
	catch(...)
	{
		return -1;
	}
}
