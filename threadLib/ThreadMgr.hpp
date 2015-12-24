#ifndef _CLASS_THREAD_MGR
#define _CLASS_THREAD_MGR


#include "General.hpp"
#include "Thread.hpp"

/************************************************************
* 以下宏定义是函数checkThread的返回状态值，具体含义如下:
* TS_RUN_NOEXIST   : 要检查的线程对象已被线程管理器清除;
* TS_RUN_NORMAL    : 线程运行正常;
* TS_RUN_EXCEPTION : 线程运行异常,导致线程终止;
* TS_RUN_UNKNOWN   : 线程未设置业务处理时间;
* TS_RUN_TIMEOUT   : 线程运行超时,可参考e_TS_TIMEOUT;
*************************************************************/
#define TS_RUN_NOEXIST         -1
#define TS_RUN_NORMAL           0  
#define TS_RUN_EXCEPTION        1
#define TS_RUN_UNKNOWN          2
#define TS_RUN_TIMEOUT          3

//以下宏是线程状态显示信息
#define STR_STATE_NORMAL     "Normal"
#define STR_STATE_TIMEOUT    "TimeOut"
#define STR_STATE_EXCEPTION  "Error"
#define STR_STATE_UNKNOWN    "Unknown"



class CThread;

class CThreadMgr
{
	friend class CThread;
public:

	virtual ~CThreadMgr();

	static CThreadMgr* createInstance();   //创建线程管理类指针;

	int  getThreadCount();

	void checkThread();  //监测所有的线程对象;
	int  checkThread(CThread &threadObj);  //对指定对象进行监测;

	void printStateInfo();  //打印线程管理器中所有线程的状态信息;
	void printStateInfo(CThread &threadObj); //打印指定线程的状态信息;

	int  clearUp();  //线程在事务处理之前的清空操作;

private:    
	CThreadMgr();

private:
	static CThreadMgr *m_pThreadMgr;
	int  m_iThreadCount;
	map<unsigned, CThread*> m_ThreadList;
};

#endif
