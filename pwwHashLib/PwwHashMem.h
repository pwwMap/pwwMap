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

#ifndef _PWWHASHMEM_H
#define _PWWHASHMEM_H
#include <string.h>
#include <algorithm>
#include <iostream>
#if (defined WIN32 || defined _WIN32 || defined WINCE)
#define pww__int64 __int64
#else
#define pww__int64 int64_t
#endif
/*
#include "math.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <float.h>
#include <stdarg.h>
#include <limits.h>
#include <locale.h>
#include <errno.h>
#include <iostream>
using namespace std;
*/


template<typename T>
struct S_PWWMEM
{
	S_PWWMEM();
	S_PWWMEM(pww__int64 lLen);
	~S_PWWMEM();
	struct S_TMEM
	{
		/*S_TMEM()
		{
			memset(this,0,sizeof(*this));
		};*/
		T* datamem;
		S_TMEM* prev;
	}*pwwMEM;

	T* pwwAllocPos(int pos);

	pww__int64 blockLen;
	pww__int64 restLen;
	pww__int64 lCurPos;
};
#include "PwwHashMem.inl"
#endif
