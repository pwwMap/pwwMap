/******************************************************************************
Title:       The core of the core of the big data solutions -- Map
Author:      pengwenwei
Email:       pww71@foxmail.com
Language:    c++
Platform:    Windows, linux
Technology:  Perfect hash algorithm
Level:       Advanced
Description: Enter a brief description of your article
Section      MFC c++ map stl
SubSection   c++ algorithm
License:     (GPLv3)

This file is part of pwwHashMap.
 
pwwHashMap is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
 
pwwHashMap is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.
 
You should have received a copy of the GNU General Public License
along with pwwHashMap.  If not, see <http://www.gnu.org/licenses/>.

******************************************************************************/

#include "../pwwHashLib/PwwStrMap.h"
#include "../pwwHashLib/safePwwStrMap.h"
#include "../threadLib/Thread.hpp"

#if defined WIN32
#include "stdafx.h"
#include "HiResTimer.h"
#endif
#include <config.h>
#include <google/dense_hash_map>
#include <google/sparse_hash_map>
#include <sstream>
#include <fstream>
#include <iostream>
using namespace std;

pww__int64 g_count = 0;
const 	int iIndexLen = 20;//The number of bytes
const   int iNum =  1000000;//The number of data  1 million don't use diskMap unless the use of solid-state hard disk
int g_sign=1;//the beginning of 1

//google map test
typedef google::dense_hash_map<string, int>::iterator StrIterator;
typedef std::pair<string, int> PairStr; 
void googleHash()
{
	google::dense_hash_map<string, int> dMap;
	dMap.set_empty_key("");
	dMap.set_deleted_key("_");
	//rand input  
#ifdef WIN32
	CHiResTimer mytime;
#endif
	string szStr = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ!\"#$%&\'()*+,-./:;< => ?@[\\]^_`{|}~";
	int count = 0;
#ifdef WIN32
	mytime.Start();
#endif
	while (count++ < iNum)
	{	
		string szKey="";
		int iRand = /*1+rand()%*/iIndexLen;
		for (int i=0;i<iRand;i++)
		{
			szKey += szStr[rand()%szStr.size()];
		}
		//	cout << szKey << endl;
		dMap.insert(std::pair<string,int>(szKey,count));
	}
#ifdef WIN32
	mytime.Stop();
	cout << "google insert time = " << mytime.GetTime()<<endl;
#endif
	string szCode="ml11111";//Search whether it contains characteristic  "ml from the string"
	dMap.insert(std::pair<string,int>(szCode,count));
#ifdef WIN32
	mytime.Reset();
	mytime.Start();
#endif
	for(int i=0;i<iNum/*szCode.size()*/;i++)
	{
		StrIterator itFind = dMap.find(szCode/*.substr(i,szCode.size()*/);
		if(itFind != dMap.end())
		{
			//			cout << "exist key  "  << itFind->second << endl;
			//			break;
		}
		/*else
		{
		cout << "does't exist key" << szCode << endl;
		}*/
	}
#ifdef WIN32
	mytime.Stop();
	cout << "google find time = " << mytime.GetTime()<<endl;
#endif
	cout << "mapsize = "<<dMap.size()<<endl;
#ifdef WIN32
	mytime.Reset();
	mytime.Start();
#endif
	StrIterator itFind = dMap.begin();
	while(itFind != dMap.end())
	{
		dMap.erase(itFind);
		itFind++;
	}
#ifdef WIN32
	mytime.Stop();
#endif
	cout << "mapsize = "<<dMap.size()<<endl;
#ifdef WIN32
	cout << "google each for time = " << mytime.GetTime()<<endl;
#endif
}


//Ergodic external execution function returns true, delete the data which meets the condition
ofstream m_ofs;
// all kinds of data into a map test
// hash table is established after the search is not a grade faster than Google. But not only is used to insert and delete query

bool mapPrtKV(unsigned char*& ucKey, int klen,unsigned char*& ucValue,void* p)
{
	g_count++;
	//std::cout<<std::string((char*)ucKey,klen) <<"		" << *(int*)&ucValue[VALUEPOS]<<std::endl;
	return false;
}
bool mapHashKV(unsigned char*& ucValue,int pos,void* p)
{
	g_count++;
	//std::cout<< std::string(&((char*)ucValue)[VALUEPOS],*(unsigned short*)&ucValue[VALUELENPOS])<<" ";
	return false;
}

void memMapTest()
{
	g_count=0;
	PwwStrMap<int, string> myMap;

#ifdef WIN32
	CHiResTimer mytime;
#endif
	string szStr = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ!\"#$%&\'()*+,-./:;< => ?@[\\]^_`{|}~";
	int count = 0;

	m_ofs.open("charTreeTest.txt", ios_base::out | ios_base::app);
	int num=0;
#ifdef WIN32
	mytime.Start();
#endif
	while (count++ < iNum)//
	{	
		string szKey="";
		int iRand = /*1+rand()%*/iIndexLen;
		for (int i=0;i<iRand;i++)
		{
			szKey += szStr[rand()%szStr.size()];
		}
		//	cout << szKey  << "		" <<count << endl;
		if (myMap.memMapInsert(count,szKey)) 
			num++;
		
	}
#ifdef WIN32
	mytime.Stop();
	cout << "insert time = "<<mytime.GetTime()<<endl;
#endif
	cout <<"map num = "<<num<<endl;

	string szCode="ml11111";
	myMap.memMapInsert(count,szCode);
	myMap.memMapModify(count,szCode);
	string value;
#ifdef WIN32
	mytime.Reset();
	mytime.Start();
#endif
	//for (int i=0;i<iNum;i++)
	{
		if (myMap.memMapFind(count,value))
		{
			cout<<"value = "<<value<<endl;
		}
		else 
			cout <<"no find result"<<endl;
	}
#ifdef WIN32
	mytime.Stop();
	cout << "find time = " <<mytime.GetTime()<<endl;
#endif
	myMap.memMapDel(count);

#ifdef WIN32
	mytime.Reset();
	mytime.Start();
#endif
	myMap.memMapForEach(mapPrtKV);
	cout<<"g_count = "<<g_count<<endl;
#ifdef WIN32
	mytime.Stop();
	cout << "each for time = " <<mytime.GetTime()<<endl;
#endif

#ifdef WIN32
	mytime.Reset();
	mytime.Start();
#endif
	myMap.memMap2HashMap();

#ifdef WIN32
	mytime.Stop();
	cout << "tohash time = " <<mytime.GetTime()<<endl;
#endif
	g_count = 0;
#ifdef WIN32
	mytime.Reset();
	mytime.Start();
#endif
	myMap.hashMapForEach2(mapHashKV);
	cout<<"g_count = "<<g_count<<endl;
#ifdef WIN32
	mytime.Stop();
	cout << "hasheachfor time = " <<mytime.GetTime()<<endl;
#endif
	m_ofs.close();

}


void diskMapTest()
{
	g_count=0;

	PwwStrMap<string, int> myMap;
	myMap.setDiskSign(g_sign++,1024*100,1024*1000*20);//The same symbols will insert error, set read and write buffer 20M


#ifdef WIN32
	CHiResTimer mytime;
#endif
	string szStr = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ!\"#$%&\'()*+,-./:;< => ?@[\\]^_`{|}~";
	int count = 0;

	m_ofs.open("charTreeTest.txt", ios_base::out | ios_base::app);
	int num=0;
#ifdef WIN32
	mytime.Start();
#endif
	while (count++ < iNum)
	{	
		string szKey="";
		int iRand = /*1+rand()%*/iIndexLen;
		for (int i=0;i<iRand;i++)
		{
			szKey += szStr[rand()%szStr.size()];
		}

		//	cout << szKey  << "		" <<count << endl;
		if (myMap.diskMapInsert(szKey,count)) 
			num++;
		//else
		//{
		//	int newVal = -count;
		//	myMap.diskMapModify(szKey,newVal);
		////	cout<<newVal<<" ";
		//}
	}
#ifdef WIN32
	mytime.Stop();
	cout << "insert time = "<<mytime.GetTime()<<endl;
#endif
	cout <<"map num = "<<num<<endl;

	string szCode="ml11111";
	myMap.diskMapInsert(szCode,count);
	myMap.diskMapModify(szCode,count+=1000000);
	int value;
#ifdef WIN32
	mytime.Reset();
	mytime.Start();
#endif
	for (int i=0;i<iNum;i++)
	{
		if (myMap.diskMapFind(szCode,value))
		{
			//cout<<"value = "<<value<<endl;
		}
		//else 
		//	cout <<"no find result"<<endl;
	}
#ifdef WIN32
	mytime.Stop();
	cout << "find time = " <<mytime.GetTime()<<endl;
#endif
	myMap.diskMapDel(szCode,1);

#ifdef WIN32
	mytime.Reset();
	mytime.Start();
#endif
	myMap.diskMapForEach(mapPrtKV);
	cout<<"g_count = "<<g_count<<endl;
#ifdef WIN32
	mytime.Stop();
	cout << "each for time = " <<mytime.GetTime()<<endl;
#endif
/*
#ifdef WIN32
	mytime.Reset();
	mytime.Start();
#endif
	myMap.rebuildDiskData();

#ifdef WIN32
	mytime.Stop();
	cout << "rebuildData time = " <<mytime.GetTime()<<endl;
#endif

#ifdef WIN32
	mytime.Reset();
	mytime.Start();
#endif
	myMap.diskMap2HashMap();

#ifdef WIN32
	mytime.Stop();
	cout << "tohash time = " <<mytime.GetTime()<<endl;
#endif
	g_count = 0;
#ifdef WIN32
	mytime.Reset();
	mytime.Start();
#endif
	myMap.hashMapForEach(mapPrtKV);
	myMap.hashMapForEach2(mapHashKV);
	cout<<"g_count = "<<g_count<<endl;
#ifdef WIN32
	mytime.Stop();
	cout << "hasheachfor time = " <<mytime.GetTime()<<endl;
#endif
*/
	m_ofs.close();

}

class mapInMap
{
public:
	mapInMap()
	{
		myMap.setDiskSign(g_sign++);
	};

	~mapInMap(){};

	static bool loops(unsigned char*& ucKey, int klen,unsigned char*& ucValue,void* p)
	{
		if (g_count == 0)
		{
			mapInMap* mp = (mapInMap*)p;
			cout << mp->myflag << endl;
		}
		g_count++;

		//std::cout<<std::string((char*)ucKey,klen) <<"		dismapsign = " << *(int*)&ucValue[VALUEPOS]<<std::endl;
		return false;
	}

	void test()
	{
		string szStr = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ!\"#$%&\'()*+,-./:;< => ?@[\\]^_`{|}~";
		int countOUT=0;
		myflag = "pwwhash";
		while (countOUT++ < 100)
		{	
			int num=0;
			int count = 0;
			PwwStrMap<string, int> inMap;
			inMap.setDiskSign(g_sign++);

			while (count++ < 10)
			{	
				string szKey="";
				int iRand = 1+rand()%iIndexLen;
				for (int i=0;i<iRand;i++)
				{
					szKey += szStr[rand()%szStr.size()];
				}
				//	cout << szKey  << "		" <<count << endl;
				if (inMap.diskMapInsert(szKey,count)) 
					num++;
			}
			cout <<"inmap num = "<<num<<endl;

			string szKey="";
			int iRand = 1+rand()%iIndexLen;
			for (int i=0;i<iRand;i++)
			{
				szKey += szStr[rand()%szStr.size()];
			}
			//	cout << szKey  << "		" <<count << endl;
			int sign = inMap.getMapSign();
			if (myMap.diskMapInsert(szKey,sign)) 
			{
			}
		}

		{
			int ret = 888888;

			PwwStrMap<string, int> inMap;
			inMap.setDiskSign(g_sign++);
			inMap.diskMapInsert(myflag,ret);
			int sign = inMap.getMapSign();
			myMap.diskMapInsert(myflag,sign);

		}
		//find
		int sign;
		if( myMap.diskMapFind(myflag,sign) )
		{
			if (sign > 0)
			{
				PwwStrMap<string, int> inMap;
				inMap.setDiskSign(sign);//we must ensure that being initialized by sign before you use diskmap
				int ret;
				if (inMap.diskMapFind(myflag,ret))
				{
					cout<<ret<<endl;
				}
			}
		}
		
		//eachfor
		myMap.diskMapForEach(loops,this);

	}
public:
	PwwStrMap<string, int> myMap;
	string myflag;
};

class CthreadSafeTest:public CThread
{
public:
	CthreadSafeTest(safePwwStrMap<string , int>* tt)
	{
		m_tt=tt;
		m_count = 0;
	};
	~CthreadSafeTest(){};
protected:
	int svc()
	{
		string szStr = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ!\"#$%&\'()*+,-./:;< => ?@[\\]^_`{|}~";
		while(m_count<1000){
			 
			string szKey="";
			int iRand = /*1+rand()%*/iIndexLen;
			for (int i=0;i<iRand;i++)
			{
				szKey += szStr[rand()%szStr.size()];
			}

			if(m_tt->diskMapInsert(szKey,m_count))
				m_count++;
		//	m_tt->diskMapModify();
		//	m_tt->diskMapDel();
		}
		g_count++;
		return 0;
	};
private:
	int m_count;
	safePwwStrMap<string , int>* m_tt;
};

void SafeTest()
{
	g_count=0;
	safePwwStrMap<std::string , int> tt;
	tt.setDiskSign(1000);
	CthreadSafeTest* test2[100];
	for (int i=0;i<100;i++)
	{
		test2[i] = new CthreadSafeTest(&tt);
		test2[i]->open();
	}

	while(g_count<100)
	{
#ifndef WIN32
		sleep(1);
#else
		Sleep(1000);
#endif
	}

	for (int i=0;i<100;i++)
	{
		delete test2[i];
	}
	g_count=0;
	tt.diskMapForEach(mapPrtKV);
	cout<<g_count<<endl;
}
int main(int argc, char* argv[])
{
	//	googleHash();	//Algorithm of Google, the performance is not good, but also the collision probability.

	//for (int i=0;i<1000;i++)
//		memMapTest();	//Based on memory No hard disk consumption

	CPwwDiskMap::pwwDiskMapIni("./pwwDiskMapTest","./pwwDiskMapTest");
//	diskMapTest();	//Based on the hard disk No memory consumption

//	mapInMap test;
//	test.test();

	SafeTest();

	std:cin.get();
	CPwwDiskMap::pwwDiskMapRelease();
	return 0;
}

