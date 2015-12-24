/******************************************************************************
Title:       The core of the big data solutions -- Map
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

#ifndef _PWWMAP_H
#define _PWWMAP_H

#include "PwwHash.h"

//using namespace std;

template <typename T1,typename T2>
class PwwMap
{
public:
	PwwMap(void);
	virtual ~PwwMap(void);
	
public:
	bool memMap2HashMap(bool deleteCharTree=true);
	bool hashMapFind(unsigned char* ucKey,int iLen,unsigned char*& ucValue);
	bool hashMapFind(T1& tKey,T2& tValue);
	virtual bool hashMapForEach( CPwwHash::loopKeyValue fun  = forEachFun,void*p=0);
	virtual bool hashMapForEach2( bool (*fun)(unsigned char*& ,int,void* ) = forEachFun2,void*p=0);

	bool memMapInsert(unsigned char* ucKey, int iKeyLen, unsigned char* ucValue, unsigned short usValueLen);
	bool memMapInsert(T1& tKey,  T2& tValue);
	bool memMapFind(unsigned char* ucKey, int iKeyLen, unsigned char*& ucValue);
	bool memMapFind(T1& tKey,T2& tValue);
	bool memMapModify(unsigned char* ucKey, int iKeyLen, unsigned char* ucValue, unsigned short usValueLen);
	bool memMapModify(T1& tKey,T2& tValue);
	bool memMapDel(unsigned char* ucKey, int iKeyLen);
	bool memMapDel(T1& tKey);
	virtual bool memMapForEach( CPwwHash::loopKeyValue fun  = forEachFun,void*p=0);

	bool setDiskSign(int mapSign,int rBufLen=RWBUF>>3,int wBufLen=RWBUF<<3);
	bool diskMapInsert(unsigned char* ucKey, int iKeyLen, unsigned char* ucValue, unsigned short usValueLen);
	bool diskMapInsert(T1& tKey,  T2& tValue);
	bool diskMapFind(unsigned char* ucKey, int iKeyLen, unsigned char*& ucValue);
	bool diskMapFind(T1& tKey,T2& tValue);
	bool diskMapModify(unsigned char* ucKey, int iKeyLen, unsigned char* ucValue, unsigned short usValueLen);
	bool diskMapModify(T1& tKey,T2& tValue);
	bool diskMapDel(unsigned char* ucKey, int iKeyLen);
	bool diskMapDel(T1& tKey);
	bool diskMap2HashMap();
	void rebuildDiskData();
	virtual void diskMapForEach( CPwwHash::loopKeyValue fun = CPwwHash::printKV,void*p=0);
	int getMapSign();

private:
	//Analysis of memory size, one-time allocation of memory
	void newMEM(pww__int64 treememlen,pww__int64 charmem);
	void freePwwMem();

	void iniDiskMap();
	void readDel();
	void readIdx();
	static bool forEachFun(unsigned char*& ucKey, int len,unsigned char*& ucValue,void* p) ;
	static bool forEachFun2(unsigned char*& ucValue, int pos,void* p);

protected:
	int m_mapSign;//set the unique sign,  must be >0 if <0 then rebuild if ==0 then no free mem
	int m_iDynamicAlloc;//if >0 then dynamic alloc mem else newMEM(unsigned long treememlen,unsigned long charmem)
	CPwwHash m_mpDel;
	CPwwDiskMap::S_PwwFile *m_fpDat,*m_fpDel;
	int* m_iIndex;
	unsigned short m_sIdxMemLen;
	unsigned short m_sMapMemLen;
	unsigned short m_sMapLen;
	CPwwHash **m_pwwMap;

private:
	CPwwHash::searchMem *m_schMem;
	CPwwHash::valueMem *m_valMem;

};

#include "PwwMap.inl"
#endif
