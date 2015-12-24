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

template <typename T1,typename T2>
PwwStrMap<T1,T2>::PwwStrMap(void):PwwMap<T1,T2>::PwwMap()
{
}
template <typename T1,typename T2>
PwwStrMap<T1,T2>::~PwwStrMap(void)
{
}

template <typename T1,typename T2>
bool PwwStrMap<T1,T2>::hashMapFind(string szKey,string& szValue,int flag)
{
	unsigned char* ucValue;
	if (PwwMap<T1,T2>::hashMapFind((unsigned char*)szKey.c_str(),szKey.size(),ucValue))
	{
		szValue = string(&((char*)ucValue)[MAPVALUEPOS],*(unsigned short*)ucValue);
		return true;
	}
	else
		return false;
}

template <typename T1,typename T2>
bool PwwStrMap<T1,T2>::hashMapFind(string szKey,T2& tValue)
{
	unsigned char* ucValue;
	if (PwwMap<T1,T2>::hashMapFind((unsigned char*)szKey.c_str(), szKey.size(),ucValue))
	{
		tValue = *(T2*)(&ucValue[MAPVALUEPOS]);
		return true;
	}
	else
		return false;
}

template <typename T1,typename T2>
bool PwwStrMap<T1,T2>::hashMapFind(T1& tKey,string& szValue)
{
	unsigned char* ucValue;
	if (PwwMap<T1,T2>::hashMapFind((unsigned char*)(&tKey), sizeof(T1),ucValue))
	{
		szValue = string(&((char*)ucValue)[MAPVALUEPOS],*(unsigned short*)ucValue);
		return true;
	}
	else
		return false;
}

template <typename T1,typename T2>
bool PwwStrMap<T1,T2>::hashMapForEach(  CPwwHash::loopKeyValue fun,void*p  )
{
	return PwwMap<T1,T2>::hashMapForEach( fun,p);;
}
template <typename T1,typename T2>
bool PwwStrMap<T1,T2>::hashMapForEach2( bool (*fun)(unsigned char*& ,int,void*))
{
	return PwwMap<T1,T2>::hashMapForEach2(fun);
}

template <typename T1,typename T2>
bool PwwStrMap<T1,T2>::memMapDel(T1& tKey)
{
	return PwwMap<T1,T2>::memMapDel((unsigned char*)(&tKey), sizeof(T1));
}

template <typename T1,typename T2>
bool PwwStrMap<T1,T2>::memMapDel(string szKey,int flag)
{
	return PwwMap<T1,T2>::memMapDel((unsigned char*)szKey.c_str(), szKey.size());
}

template <typename T1,typename T2>
bool PwwStrMap<T1,T2>::memMapInsert(string szKey,string szValue,int flag)
{
	return PwwMap<T1,T2>::memMapInsert((unsigned char*)szKey.c_str(),szKey.size(),(unsigned char*)szValue.c_str(),szValue.size());
}

template <typename T1,typename T2>
bool PwwStrMap<T1,T2>::memMapInsert(T1& tKey,  string szValue)
{
	return PwwMap<T1,T2>::memMapInsert((unsigned char*)(&tKey), sizeof(T1), (unsigned char*)szValue.c_str(), szValue.size());
}

template <typename T1,typename T2>
bool PwwStrMap<T1,T2>::memMapInsert(string szKey,  T2& tValue)
{
	return PwwMap<T1,T2>::memMapInsert((unsigned char*)szKey.c_str(), szKey.size(), (unsigned char*)(&tValue), sizeof(T2));
}

template <typename T1,typename T2>
bool PwwStrMap<T1,T2>::memMapFind(string szKey,string& szValue,int flag)
{
	unsigned char* ucValue;
	if (PwwMap<T1,T2>::memMapFind((unsigned char*)szKey.c_str(),szKey.size(),ucValue))
	{
		szValue = string(&((char*)ucValue)[MAPVALUEPOS],*(unsigned short*)ucValue);
		return true;
	}
	else
		return false;
}

template <typename T1,typename T2>
bool PwwStrMap<T1,T2>::memMapFind(string szKey,T2& tValue)
{
	unsigned char* ucValue;
	if (PwwMap<T1,T2>::memMapFind((unsigned char*)szKey.c_str(), szKey.size(),ucValue))
	{
		tValue = *(T2*)(&ucValue[MAPVALUEPOS]);
		return true;
	}
	else
		return false;
}

template <typename T1,typename T2>
bool PwwStrMap<T1,T2>::memMapFind(T1& tKey,string& szValue)
{
	unsigned char* ucValue;
	if (PwwMap<T1,T2>::memMapFind((unsigned char*)(&tKey), sizeof(T1),ucValue))
	{
		szValue = string(&((char*)ucValue)[MAPVALUEPOS],*(unsigned short*)ucValue);
		return true;
	}
	else
		return false;
}

template <typename T1,typename T2>
bool PwwStrMap<T1,T2>::memMapModify(string szKey,string& szValue, int flag)
{
	unsigned char* ucValue=szValue.c_str();
	if (PwwMap<T1,T2>::memMapModify((unsigned char*)szKey.c_str(),szKey.size(),(unsigned char*)szValue.c_str(),szValue.size()))
	{
		return true;
	}
	else
		return false;
}

template <typename T1,typename T2>
bool PwwStrMap<T1,T2>::memMapModify(string szKey,T2& tValue)
{
	if (PwwMap<T1,T2>::memMapModify((unsigned char*)szKey.c_str(), szKey.size(),(unsigned char*)&tValue,sizeof(T2)))
	{
		return true;
	}
	else
		return false;
}

template <typename T1,typename T2>
bool PwwStrMap<T1,T2>::memMapModify(T1& tKey,string& szValue)
{
	if (PwwMap<T1,T2>::memMapModify((unsigned char*)(&tKey), sizeof(T1),(unsigned char*)szValue.c_str(),szValue.size()))
	{
		return true;
	}
	else
		return false;
}


template <typename T1,typename T2>
bool PwwStrMap<T1,T2>::memMapForEach(   CPwwHash::loopKeyValue fun,void*p )
{
	return PwwMap<T1,T2>::memMapForEach( fun,p);
}

template <typename T1,typename T2>
bool PwwStrMap<T1,T2>::strForEachFun(unsigned char*& ucKey, int len,unsigned char*& ucValue,void* p)
{
//	cout << "key = "<<string((char*)ucKey,len)<<" value = "<<*(T2*)(&((char*)ucValue)[VALUEPOS]) <<endl;
	return true;
}

template <typename T1,typename T2>
bool PwwStrMap<T1,T2>::strForEachFun2(unsigned char*& ucValue, int pos,void* p)
{
//	cout <<" value = "<<string(&((char*)ucValue)[pos+VALUEPOS],*(unsigned short*)&ucValue[pos+VALUELENPOS]) <<endl;
	return true;
}

template <typename T1,typename T2>
bool PwwStrMap<T1,T2>::diskMapDel(T1& tKey)
{
	return PwwMap<T1,T2>::diskMapDel((unsigned char*)(&tKey), sizeof(T1));
}

template <typename T1,typename T2>
bool PwwStrMap<T1,T2>::diskMapDel(string szKey,int flag)
{
	return PwwMap<T1,T2>::diskMapDel((unsigned char*)szKey.c_str(), szKey.size());
}

template <typename T1,typename T2>
bool PwwStrMap<T1,T2>::diskMapInsert(string szKey,string szValue,int flag)
{
	return PwwMap<T1,T2>::diskMapInsert((unsigned char*)szKey.c_str(),szKey.size(),(unsigned char*)szValue.c_str(),szValue.size());
}

template <typename T1,typename T2>
bool PwwStrMap<T1,T2>::diskMapInsert(T1 tKey,  string& szValue)
{
	return PwwMap<T1,T2>::diskMapInsert((unsigned char*)(&tKey), sizeof(T1), (unsigned char*)szValue.c_str(), szValue.size());
}

template <typename T1,typename T2>
bool PwwStrMap<T1,T2>::diskMapInsert(string& szKey,  T2& tValue)
{
	return PwwMap<T1,T2>::diskMapInsert((unsigned char*)szKey.c_str(), szKey.size(), (unsigned char*)(&tValue), sizeof(T2));
}

template <typename T1,typename T2>
bool PwwStrMap<T1,T2>::diskMapFind(string szKey,string& szValue, int flag)
{
	unsigned char* ucValue;
	if (PwwMap<T1,T2>::diskMapFind((unsigned char*)szKey.c_str(),szKey.size(),ucValue ))
	{
		szValue =string((char*)ucValue,*(unsigned short*)(ucValue-USLEN));

		return true;
	}
	else
		return false;
}

template <typename T1,typename T2>
bool PwwStrMap<T1,T2>::diskMapFind(string szKey,T2& tValue )
{
	unsigned char* ucValue;
	if (PwwMap<T1,T2>::diskMapFind((unsigned char*)szKey.c_str(), szKey.size(),ucValue ))
	{
		tValue = *(T2*)(ucValue);

		return true;
	}
	else
		return false;
}

template <typename T1,typename T2>
bool PwwStrMap<T1,T2>::diskMapFind(T1& tKey,string& szValue )
{
	unsigned char* ucValue;
	if (PwwMap<T1,T2>::diskMapFind((unsigned char*)(&tKey), sizeof(T1),ucValue ))
	{
		szValue = string((char*)ucValue,*(unsigned short*)(ucValue-USLEN));

		return true;
	}
	else
		return false;
}


template <typename T1,typename T2>
bool PwwStrMap<T1,T2>::diskMapModify(string szKey,string& szValue, int flag)
{
	unsigned char* ucValue;
	if (PwwMap<T1,T2>::diskMapModify((unsigned char*)szKey.c_str(),szKey.size(),(unsigned char*)szValue.c_str(),szValue.size()))
	{
		return true;
	}
	else
		return false;
}

template <typename T1,typename T2>
bool PwwStrMap<T1,T2>::diskMapModify(string szKey,T2& tValue )
{
	if (PwwMap<T1,T2>::diskMapModify((unsigned char*)szKey.c_str(), szKey.size(),(unsigned char*)&tValue,sizeof(T2)))
	{
		return true;
	}
	else
		return false;
}

template <typename T1,typename T2>
bool PwwStrMap<T1,T2>::diskMapModify(T1& tKey,string& szValue )
{
	if (PwwMap<T1,T2>::diskMapModify((unsigned char*)(&tKey), sizeof(T1),(unsigned char*)szValue.c_str(),szValue.size()))
	{
		return true;
	}
	else
		return false;
}

template <typename T1,typename T2>
void PwwStrMap<T1,T2>::diskMapForEach(  CPwwHash::loopKeyValue fun ,void*p)
{
	return PwwMap<T1,T2>::diskMapForEach( fun,p);
}




























