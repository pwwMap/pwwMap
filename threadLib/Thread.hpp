

#ifndef __THREAD_HPP__
#define __THREAD_HPP__

#ifndef WIN32
#include <pthread.h>
#endif


#include "ThreadMgr.hpp"


/************************************************************************
*    e_TS_NORMAL： 	 表示线程运行正常;
*    e_TS_TIMEOUT：  表示线程异常,即线程管理器监测时间与线程末次心跳时间
*	                 的时间差超过了构造对象时所设置的业务处理时间;
*	 e_TS_UNKNOWN：  表示线程运行状态未知,如果构造线程对象时，没有传递
*					 业务处理时间,线程管理器会对线程状态不做判断;
*	 e_TS_EXCEPTION：表示由于异常产生,导致线程结束;
*    e_TS_EXIT:      表示线程正常退出;
**************************************************************************/
enum E_RUN_STATE{e_TS_NORMAL, e_TS_UNKONWN, e_TS_TIMEOUT, e_TS_EXCEPTION, e_TS_EXIT};



//Windows平台下的线程优先级
enum E_TS_PRIORITY {THREAD_PRIORITY_IDLEX = -15, 
THREAD_PRIORITY_ERROR_RETURNX = 0x7fffffff,
THREAD_PRIORITY_LOWESTX = -2, 
THREAD_PRIORITY_BELOW_NORMALX = -1,
THREAD_PRIORITY_NORMALX = 0, 
THREAD_PRIORITY_ABOVE_NORMALX = 1, 
THREAD_PRIORITY_HIGHESTX = 2, 
THREAD_PRIORITY_TIME_CRITICALX = 15};


/***********************线程属性结构体**************************
* @参数介绍:
*	argSzDescrip: 线程的功能描述符;
*	argClrPrior : 线程的优先级;
***************************************************************/
typedef struct ThreadAttribute
{
	ThreadAttribute(string argSzDescrip = "", unsigned argClrPrior = 0, 
		int corePrior =THREAD_PRIORITY_NORMALX)
	{
		sysPriority = corePrior;
		ui_ClrPriority = argClrPrior;
		szDescription = argSzDescrip;
	}

	string        szDescription ;
	unsigned      ui_ClrPriority;		//线程管理器清理时的分配的优先级;
	int           sysPriority;		//基于系统内核的优先级	  
} SThreadAttrib, *pThreadAttrib;


class CThreadMgr;

class CThread
{
public:
	CThread(pThreadAttrib pAttrib = NULL, CThreadMgr *pThreadMgr = NULL,
		unsigned uiWorkTime = 0);

	virtual ~CThread();

	virtual int open(void *args = 0);

	virtual void stop();

	virtual int clearUp();			//线程清空处理操作。

	void sleep(unsigned long dwMilliseconds);

	unsigned   getThreadId();		//获取线程ID标识符;

	time_t	   getHeartBeat();	    //获取线程最近一次的心跳;

	string     getDescription();    //获取线程的功能描述;

	unsigned   getWorkTime();       //获取线程的业务处理时间;

	int   getSysPriority();			//获取线程关于系统内核设置的优先级;

	unsigned   getPriority();		//获取线程为线程管理器在进行清空操作而设置的优先级;

	E_RUN_STATE getThreadState();	//获取线程运行状态;

	void  setThreadState(E_RUN_STATE thrRunState);  //设置线程的运行状态;

	void  sendHeartBeat();          //向线程管理器发送心跳;


protected:
	virtual int svc() = 0;				

	void *m_thread_args;


#ifdef WIN32
	static  unsigned __stdcall thread_proxy(void*);
	void *m_thread_handle;
#else
	static void* thread_proxy(void*);
	pthread_t m_thread_handle;
	pthread_attr_t m_threadAttrib;
#endif	


private:
	CThread(const CThread&);

	const CThread& operator = (const CThread&);

	void  regThread();     //向线程管理器注册;

	void  unRegThread();   //向线程管理器注销(反注册);

private:
	unsigned    m_uiThreadId;
	time_t      m_LastHeartBeat;
	string      m_szDescription;
	CThreadMgr  *m_pThreadMgr;
	E_RUN_STATE m_ThreadState;		//线程运行状态;
	unsigned    m_uiWorkTime;		//线程函数一次业务处理时间;
	unsigned    m_uiPriority;		//线程为线程管理器在完成清空操作设置的优先级;
	int     	m_iSysPriority;		//线程基于系统内核的优先级;
};



#endif

