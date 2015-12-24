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

#ifndef _SAFEPWWMAP_H
#define _SAFEPWWMAP_H

#include "PwwHash.h"
#include "../threadLib/ResLock.hpp"

//using namespace std;

template <typename T1,typename T2>
class safePwwMap:public PwwMap<T1,T2>
{
public:
	safePwwMap(void);
	virtual ~safePwwMap(void);

public:
	bool memMapInsert(unsigned char* ucKey, int iKeyLen, unsigned char* ucValue, unsigned short usValueLen);
	bool memMapInsert(T1& tKey,  T2& tValue);
	bool memMapModify(unsigned char* ucKey, int iKeyLen, unsigned char* ucValue, unsigned short usValueLen);
	bool memMapModify(T1& tKey,T2& tValue);
	bool memMapDel(unsigned char* ucKey, int iKeyLen);
	bool memMapDel(T1& tKey);

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
private:
	CResLock m_Lock;

};

#include "safePwwMap.inl"
#endif
