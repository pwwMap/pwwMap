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

const pww__int64 TREEMEMBLOCKLEN = 1024*1024*512;

template<typename T>
S_PWWMEM<T>::S_PWWMEM()
{
	memset(this,0,sizeof(*this));
	pwwMEM = (S_TMEM*)malloc(sizeof(S_TMEM));
	pwwMEM->prev = NULL;
	pwwMEM->datamem = NULL;
}

template<typename T>
S_PWWMEM<T>::S_PWWMEM(pww__int64 lLen)
{
	memset(this,0,sizeof(*this));
	pwwMEM = (S_TMEM*)malloc(sizeof(S_TMEM));
	pwwMEM->prev = NULL;
	restLen = lLen;
	blockLen = TREEMEMBLOCKLEN/sizeof(T);
	if (blockLen < restLen)
	{
		pwwMEM->datamem = (T*)calloc(blockLen,sizeof(T));
	}
	else 
		pwwMEM->datamem = (T*)calloc(restLen,sizeof(T));
}

template<typename T>
S_PWWMEM<T>::~S_PWWMEM()
{
	if (NULL != pwwMEM)
	{
		while(NULL != pwwMEM->prev)
		{
			S_TMEM* prev = pwwMEM->prev;
			if (NULL != pwwMEM->datamem) 
				free(pwwMEM->datamem);
			free(pwwMEM);
			pwwMEM = prev;
		}
		if (NULL != pwwMEM->datamem) 
			free(pwwMEM->datamem);
		free(pwwMEM);		
		pwwMEM=NULL;
	}
}

template<typename T>
T* S_PWWMEM<T>::pwwAllocPos(int pos)
{
	lCurPos += pos;
	if (NULL == pwwMEM->datamem)
	{
		pwwMEM->datamem = (T*)calloc(blockLen=1024*1024,sizeof(T));
		restLen = 0xffffffffffffffff;
		lCurPos = pos;
	}
	if (lCurPos > blockLen)
	{
		restLen -= lCurPos-pos;
		S_TMEM* prev = pwwMEM;
		pwwMEM = (S_TMEM*)malloc(sizeof(S_TMEM));
		if (blockLen < restLen)
		{
			pwwMEM->datamem = (T*)calloc(blockLen,sizeof(T));
		}
		else 
			pwwMEM->datamem = (T*)calloc(restLen,sizeof(T));
		pwwMEM->prev = prev;
		lCurPos = pos;
	}
	return pwwMEM->datamem+lCurPos-pos;
}

