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


#ifndef _STRING__SAFEPWWMAP_H
#define _STRING__SAFEPWWMAP_H

#include "safePwwMap.h"

#include "math.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <float.h>
#include <string.h>
#include <stdarg.h>
#include <string>
#include <limits.h>
#include <locale.h>
#include <algorithm>
#include <errno.h>
#include <iostream>
#include <time.h>
using namespace std;
#include <typeinfo>
template <typename T1,typename T2>
class safePwwStrMap:public safePwwMap<T1,T2>
{
public:
	safePwwStrMap(void);
	virtual ~safePwwStrMap(void);

public:

	bool hashMapFind(string szKey,string& szValue,int flag);
	bool hashMapFind(string szKey,T2& tValue);
	bool hashMapFind(T1& tKey,string& szValue);
	virtual bool hashMapForEach( CPwwHash::loopKeyValue fun  = strForEachFun,void*p=0);
	virtual bool hashMapForEach2( bool (*fun)(unsigned char*& ,int,void* ) = strForEachFun2);

	bool memMapInsert(string szKey,string szValue,int flag);
	bool memMapInsert(T1& tKey,  string szValue);
	bool memMapInsert(string szKey,  T2& tValue);
	bool memMapDel(T1& tKey);
	bool memMapDel(string szKey,int flag);
	bool memMapFind(string szKey,string& szValue, int flag);
	bool memMapFind(string szKey,T2& tValue );
	bool memMapFind(T1& tKey,string& szValue );
	bool memMapModify(string szKey,string& szValue, int flag);
	bool memMapModify(string szKey,T2& tValue );
	bool memMapModify(T1& tKey,string& szValue );
	virtual bool memMapForEach( CPwwHash::loopKeyValue fun  = strForEachFun,void*p=0);

	bool diskMapInsert(string szKey,string szValue,int flag);
	bool diskMapInsert(T1 tKey,  string& szValue);
	bool diskMapInsert(string& szKey,  T2& tValue);
	bool diskMapFind(string szKey,string& szValue, int flag);
	bool diskMapFind(string szKey,T2& tValue );
	bool diskMapFind(T1& tKey,string& szValue );
	bool diskMapModify(string szKey,string& szValue, int flag);
	bool diskMapModify(string szKey,T2& tValue );
	bool diskMapModify(T1& tKey,string& szValue );
	bool diskMapDel(T1& tKey);
	bool diskMapDel(string tKey,int flag);
	virtual void diskMapForEach( CPwwHash::loopKeyValue fun = CPwwHash::printKV,void*p=0);


private:
	static bool strForEachFun(unsigned char*& ucKey, int len,unsigned char*& ucValue,void* p) ;
	static bool strForEachFun2(unsigned char*& ucValue, int pos,void* p);

};
#include "safePwwStrMap.inl"

#endif
