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
safePwwMap<T1,T2>::safePwwMap(void):PwwMap<T1,T2>::PwwMap()
{
	
}

template <typename T1,typename T2>
safePwwMap<T1,T2>::~safePwwMap(void)
{
	
}


template <typename T1,typename T2>
bool safePwwMap<T1,T2>::memMapModify(unsigned char* ucKey, int iKeyLen, unsigned char* ucValue, unsigned short usValueLen)
{
	if ( iKeyLen>=PwwMap<T1,T2>::m_sIdxMemLen || -1 == PwwMap<T1,T2>::m_iIndex[iKeyLen])
	{
		return false;
	}

	m_Lock.lock();
	if ( PwwMap<T1,T2>::m_pwwMap[PwwMap<T1,T2>::m_iIndex[iKeyLen]]->memMapModify(ucKey,ucValue,usValueLen) )
	{
		m_Lock.unLock();
		return true;
	}
	else
	{
		m_Lock.unLock();
		return false;
	}
}

template<typename T1,typename T2>
bool safePwwMap<T1,T2>::memMapModify(T1& tKey,T2& tValue)
{
	return memMapModify((unsigned char*)(&tKey), sizeof(T1),(unsigned char*)&tValue,sizeof(T2));
}

template <typename T1,typename T2>
bool safePwwMap<T1,T2>::memMapInsert(unsigned char* ucKey, int iKeyLen, unsigned char* ucValue, unsigned short usValueLen)
{
	m_Lock.lock();

	if ( PwwMap<T1,T2>::memMapInsert(ucKey,iKeyLen,ucValue,usValueLen) )
	{
		m_Lock.unLock();
		return true;
	}
	else
	{
		m_Lock.unLock();
		return false;
	}
}

template<typename T1,typename T2>
bool safePwwMap<T1,T2>::memMapInsert(T1& tKey,  T2& tValue)
{
	return memMapInsert((unsigned char*)(&tKey), sizeof(T1), (unsigned char*)(&tValue), sizeof(T2));
}


template <typename T1,typename T2>
bool safePwwMap<T1,T2>::memMapDel(unsigned char* ucKey, int iKeyLen)
{
	if ( iKeyLen>=PwwMap<T1,T2>::m_sIdxMemLen || -1 == PwwMap<T1,T2>::m_iIndex[iKeyLen])
	{
		return false;
	}
	m_Lock.lock();
	if ( PwwMap<T1,T2>::m_pwwMap[PwwMap<T1,T2>::m_iIndex[iKeyLen]]->memMapDel(ucKey) )
	{
		m_Lock.unLock();
		return true;
	}
	else
	{
		m_Lock.unLock();
		return false;
	}

}

template<typename T1,typename T2>
bool safePwwMap<T1,T2>::memMapDel(T1& tKey)
{
	return memMapDel((unsigned char*)(&tKey), sizeof(T1));
}

template <typename T1,typename T2>
bool safePwwMap<T1,T2>::diskMapFind(unsigned char* ucKey, int iKeyLen, unsigned char*& ucValue)
{
	if ( iKeyLen>=PwwMap<T1,T2>::m_sIdxMemLen || -1 == PwwMap<T1,T2>::m_iIndex[iKeyLen])
	{
		return false;
	}

	m_Lock.lock();
	if ( PwwMap<T1,T2>::m_pwwMap[PwwMap<T1,T2>::m_iIndex[iKeyLen]]->diskMapFind(ucKey,ucValue) )
	{
		m_Lock.unLock();
		return true;
	}
	else
	{
		m_Lock.unLock();
		return false;
	}

}

template<typename T1,typename T2>
bool safePwwMap<T1,T2>::diskMapFind(T1& tKey,T2& tValue)
{
	unsigned char* ucValue;
	if (diskMapFind((unsigned char*)(&tKey), sizeof(T1),ucValue))
	{
		tValue = *(T2*)(ucValue);
		return true;
	}
	else
	{
		return false;
	}
}

template <typename T1,typename T2>
bool safePwwMap<T1,T2>::diskMapModify(unsigned char* ucKey, int iKeyLen, unsigned char* ucValue, unsigned short usValueLen)
{
	if ( iKeyLen>=PwwMap<T1,T2>::m_sIdxMemLen || -1 == PwwMap<T1,T2>::m_iIndex[iKeyLen])
	{
		return false;
	}
	m_Lock.lock();
	if ( PwwMap<T1,T2>::m_pwwMap[PwwMap<T1,T2>::m_iIndex[iKeyLen]]->diskMapModify(ucKey,ucValue,usValueLen) )
	{
		m_Lock.unLock();
		return true;
	}
	else
	{
		m_Lock.unLock();
		return false;
	}
}

template<typename T1,typename T2>
bool safePwwMap<T1,T2>::diskMapModify(T1& tKey,T2& tValue)
{
	return diskMapModify((unsigned char*)(&tKey), sizeof(T1),(unsigned char*)&tValue,sizeof(T2));
}

template <typename T1,typename T2>
bool safePwwMap<T1,T2>::diskMapInsert(unsigned char* ucKey, int iKeyLen, unsigned char* ucValue, unsigned short usValueLen)
{
	m_Lock.lock();
	
	if ( PwwMap<T1,T2>::diskMapInsert(ucKey,iKeyLen,ucValue,usValueLen) )
	{
		m_Lock.unLock();
		return true;
	}
	else
	{
		m_Lock.unLock();
		return false;
	}

}

template<typename T1,typename T2>
bool safePwwMap<T1,T2>::diskMapInsert(T1& tKey,  T2& tValue)
{
	return diskMapInsert((unsigned char*)(&tKey), sizeof(T1), (unsigned char*)(&tValue), sizeof(T2));
}

template <typename T1,typename T2>
bool safePwwMap<T1,T2>::diskMapDel(unsigned char* ucKey, int iKeyLen)
{
	if ( iKeyLen>=PwwMap<T1,T2>::m_sIdxMemLen || -1 == PwwMap<T1,T2>::m_iIndex[iKeyLen] )
	{
		return false;
	}
	m_Lock.lock();
	if (PwwMap<T1,T2>::m_pwwMap[PwwMap<T1,T2>::m_iIndex[iKeyLen]]->diskMapDel(ucKey))
	{
		m_Lock.unLock();
		return true;
	}
	else
	{
		m_Lock.unLock();
		return false;
	}
}

template<typename T1,typename T2>
bool safePwwMap<T1,T2>::diskMapDel(T1& tKey)
{
	return diskMapDel((unsigned char*)(&tKey), sizeof(T1));
}


template <typename T1,typename T2>
bool safePwwMap<T1,T2>::diskMap2HashMap()
{
	m_Lock.lock();
	if ( PwwMap<T1,T2>::diskMap2HashMap() )
	{
		m_Lock.unLock();
		return true;
	}
	else
	{
		m_Lock.unLock();
		return false;
	}
}

template <typename T1,typename T2>
void safePwwMap<T1,T2>::diskMapForEach(  CPwwHash::loopKeyValue fun,void*p )
{
	m_Lock.lock();
	PwwMap<T1,T2>::diskMapForEach(fun,p);
	m_Lock.unLock();
}

template <typename T1,typename T2>
void safePwwMap<T1,T2>::rebuildDiskData()
{
	m_Lock.lock();
	PwwMap<T1,T2>::rebuildDiskData();
	m_Lock.unLock();
}
