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
PwwMap<T1,T2>::PwwMap(void)
{
	m_iDynamicAlloc = 0;
	m_schMem = NULL;
	m_valMem = NULL;
	m_sIdxMemLen = INDEXLEN;
	m_iIndex = (int*)malloc(sizeof(int)*m_sIdxMemLen);
	memset(m_iIndex,-1,sizeof(int)*m_sIdxMemLen);
	m_pwwMap = (CPwwHash **)calloc(INDEXLEN,sizeof(CPwwHash *));
	m_sMapLen = 0;
	m_sMapMemLen = INDEXLEN;
	m_mapSign = 0;// need >0 if <0 then rebuild if ==0 then is memmap
	m_fpDat = m_fpDel = NULL;
}

template <typename T1,typename T2>
PwwMap<T1,T2>::~PwwMap(void)
{
	if (m_iDynamicAlloc>0)
	{
		for (int i=0;i<m_sMapLen;i++)
			m_pwwMap[i]->delHashMapMem();
	}

	for (int i=0;i<m_sMapLen;i++)
	{
		delete m_pwwMap[i];
		m_pwwMap[i] = NULL;
	}
	if (NULL!=m_pwwMap)
		free(m_pwwMap);
	if (NULL!=m_iIndex)
		free(m_iIndex);
	if (NULL!=m_fpDat)
		delete m_fpDat;
	if (NULL!=m_fpDel)
		delete m_fpDel;

	freePwwMem();

	if (0 != m_mapSign )
	{
		CPwwDiskMap::g_mpSign.memMapDel((unsigned char*)&m_mapSign);
	}
}

template <typename T1,typename T2>
void PwwMap<T1,T2>::freePwwMem()
{
	if (NULL != m_schMem)
	{
		delete m_schMem;
		m_schMem=NULL;
	}
	if (NULL != m_valMem)
	{
		delete m_valMem;
		m_valMem=NULL;
	}
}

template <typename T1,typename T2>
void PwwMap<T1,T2>::newMEM(pww__int64 treememlen,pww__int64 charmem)
{
	freePwwMem();
	if (NULL == m_schMem)
	{
		m_schMem = new CPwwHash::searchMem(treememlen);
	}
	if (NULL == m_valMem)
	{
		m_valMem = new CPwwHash::valueMem(charmem+sizeof(int));
	}
}

template <typename T1,typename T2>
bool PwwMap<T1,T2>::memMap2HashMap(bool deleteCharTree)
{
	if (m_iDynamicAlloc>0)
		return false;
	int i;
	pww__int64 ulsearchMemLen=0;
	pww__int64 ulValMemlen=0;
	for ( i=0;i<m_sMapLen;i++)
	{
		m_pwwMap[i]->memMapAnalaysis(ulsearchMemLen,ulValMemlen);
	}
#ifdef _DEBUG
	printf("%ld\t",ulsearchMemLen);
	printf("%ld\n",ulValMemlen);
#endif
	newMEM(ulsearchMemLen,ulValMemlen);
	for ( i=0;i<m_sMapLen;i++)
	{
		m_pwwMap[i]->memMap2HashMap(m_schMem,m_valMem,deleteCharTree);
	}
	return true;
}

template <typename T1,typename T2>
bool PwwMap<T1,T2>::hashMapFind(unsigned char* ucKey,int iLen,unsigned char*& ucValue)
{
	if ( iLen>=m_sIdxMemLen || -1 == m_iIndex[iLen])
	{
		return false;
	}
	return m_pwwMap[m_iIndex[iLen]]->hashMapFind(ucKey,ucValue);
}

template<typename T1,typename T2>
bool PwwMap<T1,T2>::hashMapFind(T1& tKey,T2& tValue)
{
	unsigned char* ucValue;
	if (hashMapFind((unsigned char*)(&tKey), sizeof(T1),ucValue))
	{
		tValue = *(T2*)(&ucValue[MAPVALUEPOS]);
		return true;
	}
	else
		return false;
}

template <typename T1,typename T2>
bool PwwMap<T1,T2>::hashMapForEach( CPwwHash::loopKeyValue fun ,void*p )
{
	for (int i=0;i<m_sMapLen;i++)
	{
		m_pwwMap[i]->hashMapForEach(  fun,p );
	}
	return true;
}

template <typename T1,typename T2>
bool PwwMap<T1,T2>::hashMapForEach2( bool (*fun)(unsigned char*& ,int,void* ),void*p)
{
	if (NULL != m_valMem)
	{
		S_PWWMEM<unsigned char>::S_TMEM* val = m_valMem->pwwMEM;
		while(NULL != val)
		{
			if (NULL != val->datamem)
			{
				int pos=0;
				while(1==*(int*)&val->datamem[pos])
				{
					fun(val->datamem,pos,p);
					pos += VALUEPOS+*(unsigned short*)&val->datamem[pos+VALUELENPOS];
				}
			}
			val = val->prev;
		}
	}
	return true;
}

template <typename T1,typename T2>
bool PwwMap<T1,T2>::memMapFind(unsigned char* ucKey, int iKeyLen, unsigned char*& ucValue)
{
	if ( iKeyLen>=m_sIdxMemLen || -1 == m_iIndex[iKeyLen])
	{
		return false;
	}
	return m_pwwMap[m_iIndex[iKeyLen]]->memMapFind(ucKey,ucValue);
}

template<typename T1,typename T2>
bool PwwMap<T1,T2>::memMapFind(T1& tKey,T2& tValue)
{
	unsigned char* ucValue;
	if (memMapFind((unsigned char*)(&tKey), sizeof(T1),ucValue))
	{
		tValue = *(T2*)(&ucValue[MAPVALUEPOS]);
		return true;
	}
	else
		return false;
}

template <typename T1,typename T2>
bool PwwMap<T1,T2>::memMapModify(unsigned char* ucKey, int iKeyLen, unsigned char* ucValue, unsigned short usValueLen)
{
	if ( iKeyLen>=m_sIdxMemLen || -1 == m_iIndex[iKeyLen])
	{
		return false;
	}
	return m_pwwMap[m_iIndex[iKeyLen]]->memMapModify(ucKey,ucValue,usValueLen) ;
}

template<typename T1,typename T2>
bool PwwMap<T1,T2>::memMapModify(T1& tKey,T2& tValue)
{
	return memMapModify((unsigned char*)(&tKey), sizeof(T1),(unsigned char*)&tValue,sizeof(T2));
}

template <typename T1,typename T2>
bool PwwMap<T1,T2>::memMapInsert(unsigned char* ucKey, int iKeyLen, unsigned char* ucValue, unsigned short usValueLen)
{
	if (iKeyLen>=m_sIdxMemLen)
	{
		int len = (iKeyLen<<1);
		if (len > 0xFFFF)
			len = 0xFFFF;

		m_iIndex = (int*)realloc(m_iIndex,sizeof(int)*len);
		memset(m_iIndex+m_sIdxMemLen,-1,sizeof(int)*(len-m_sIdxMemLen));
		m_sIdxMemLen = len;
	}
	if (-1 == m_iIndex[iKeyLen])
	{
		m_iIndex[iKeyLen] = m_sMapLen++;
		if (m_sMapLen >= m_sMapMemLen)
		{
			unsigned short sNewLen = (m_sMapMemLen<<1);
			m_pwwMap = (CPwwHash **)realloc(m_pwwMap,sNewLen*sizeof(CPwwHash *));
			m_sMapMemLen = sNewLen;
		}
		m_pwwMap[m_iIndex[iKeyLen]] = new CPwwHash(iKeyLen);
	}

	return m_pwwMap[m_iIndex[iKeyLen]]->memMapInsert(ucKey,ucValue,usValueLen) ;
	
}

template<typename T1,typename T2>
bool PwwMap<T1,T2>::memMapInsert(T1& tKey,  T2& tValue)
{
	return memMapInsert((unsigned char*)(&tKey), sizeof(T1), (unsigned char*)(&tValue), sizeof(T2));
}

template <typename T1,typename T2>
bool PwwMap<T1,T2>::memMapDel(unsigned char* ucKey, int iKeyLen)
{
	if ( iKeyLen>=m_sIdxMemLen || -1 == m_iIndex[iKeyLen])
	{
		return false;
	}
	return m_pwwMap[m_iIndex[iKeyLen]]->memMapDel(ucKey) ;
}

template<typename T1,typename T2>
bool PwwMap<T1,T2>::memMapDel(T1& tKey)
{
	return memMapDel((unsigned char*)(&tKey), sizeof(T1));
}

template <typename T1,typename T2>
bool PwwMap<T1,T2>::memMapForEach( CPwwHash::loopKeyValue fun,void*p)
{
	for (int i=0;i<m_sMapLen;i++)
	{
		m_pwwMap[i]->memMapForEach(  fun,p );
	}
	return true;
}

template <typename T1,typename T2>
bool PwwMap<T1,T2>::forEachFun(unsigned char*& ucKey, int len,unsigned char*& ucValue,void* p)
{
//  std::cout << "key = "<<*(T1*)ucKey<<" value = "<<*(T2*)(&((char*)ucValue)[VALUEPOS]) <<std::endl;
  return true;
}

template <typename T1,typename T2>
bool PwwMap<T1,T2>::forEachFun2(unsigned char*& ucValue, int pos,void* p)
{
  std::cout <<" value = "<<*(T2*)(&((char*)ucValue)[pos+VALUEPOS]) <<std::endl;
  return true;
}

template <typename T1,typename T2>
bool PwwMap<T1,T2>::setDiskSign(int mapSign,int rBufLen,int wBufLen)
{
	if ( !CPwwDiskMap::g_mpSign.memMapInsert((unsigned char*)&mapSign,(unsigned char*)this,sizeof(this)) )
	{
		return false;//if exist 
	}

	m_mapSign = mapSign;
	
	//iniDiskMap
	char pchPath[1024];		
	CPwwDiskMap::getPwwDiskPath(pchPath,m_mapSign);

	strcat(pchPath,pww_LocalDir);
	pww_mkdir(pchPath);
	int iL = strlen(pchPath);
	pchPath[iL++] = pww_DirSpilt;
	pchPath[iL] = 0;

	char cSign[8];
	pww_itoa(cSign,m_mapSign);
	strcat(pchPath,cSign);
	iL = strlen(pchPath);
	pchPath[iL] = 0;

	iL = strlen(pchPath);
	strcat(pchPath,DATAFILEEXT);
	m_fpDat = new CPwwDiskMap::S_PwwFile(pchPath, rBufLen, wBufLen);
	m_fpDat->openPwwRead();

	if (m_mapSign>=0)
	{
		m_mpDel.setKeyLen(PLEN);
		memcpy(pchPath+iL,DATADEL,strlen(DATADEL));
		m_fpDel = new CPwwDiskMap::S_PwwFile(pchPath);
		readDel();
	}
	else
		m_fpDel = NULL;

	iniDiskMap();
	readIdx();
	return true;
}

template <typename T1,typename T2>
int PwwMap<T1,T2>::getMapSign()
{
	return m_mapSign;
}

template <typename T1,typename T2>
bool PwwMap<T1,T2>::diskMapFind(unsigned char* ucKey, int iKeyLen, unsigned char*& ucValue)
{
	if ( iKeyLen>=m_sIdxMemLen || -1 == m_iIndex[iKeyLen])
	{
		return false;
	}

	return m_pwwMap[m_iIndex[iKeyLen]]->diskMapFind(ucKey,ucValue) ;
}

template<typename T1,typename T2>
bool PwwMap<T1,T2>::diskMapFind(T1& tKey,T2& tValue)
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
bool PwwMap<T1,T2>::diskMapModify(unsigned char* ucKey, int iKeyLen, unsigned char* ucValue, unsigned short usValueLen)
{
	if ( iKeyLen>=m_sIdxMemLen || -1 == m_iIndex[iKeyLen])
	{
		return false;
	}
	return m_pwwMap[m_iIndex[iKeyLen]]->diskMapModify(ucKey,ucValue,usValueLen) ;
}

template<typename T1,typename T2>
bool PwwMap<T1,T2>::diskMapModify(T1& tKey,T2& tValue)
{
	return diskMapModify((unsigned char*)(&tKey), sizeof(T1),(unsigned char*)&tValue,sizeof(T2));
}
template <typename T1,typename T2>
bool PwwMap<T1,T2>::diskMapInsert(unsigned char* ucKey, int iKeyLen, unsigned char* ucValue, unsigned short usValueLen)
{
	if (iKeyLen>=m_sIdxMemLen)
	{
		int len = (iKeyLen<<1);
		if (len > 0xFFFF)
			len = 0xFFFF;

		m_iIndex = (int*)realloc(m_iIndex,sizeof(int)*len);
		memset(m_iIndex+m_sIdxMemLen,-1,sizeof(int)*(len-m_sIdxMemLen));
		m_sIdxMemLen = len;
	}
	if (-1 == m_iIndex[iKeyLen])
	{
		m_iIndex[iKeyLen] = m_sMapLen++;
		if (m_sMapLen >= m_sMapMemLen)
		{
			unsigned short sNewLen = (m_sMapMemLen<<1);
			m_pwwMap = (CPwwHash **)realloc(m_pwwMap,sNewLen*sizeof(CPwwHash *));
			m_sMapMemLen = sNewLen;
		}
		m_pwwMap[m_iIndex[iKeyLen]] = new CPwwDiskMap(iKeyLen,m_fpDat, m_fpDel, &m_mpDel);
	}

	return m_pwwMap[m_iIndex[iKeyLen]]->diskMapInsert(ucKey,ucValue,usValueLen) ;
}

template<typename T1,typename T2>
bool PwwMap<T1,T2>::diskMapInsert(T1& tKey,  T2& tValue)
{
	return diskMapInsert((unsigned char*)(&tKey), sizeof(T1), (unsigned char*)(&tValue), sizeof(T2));
}

template <typename T1,typename T2>
bool PwwMap<T1,T2>::diskMapDel(unsigned char* ucKey, int iKeyLen)
{
	if ( iKeyLen>=m_sIdxMemLen || -1 == m_iIndex[iKeyLen] )
	{
		return false;
	}
	return m_pwwMap[m_iIndex[iKeyLen]]->diskMapDel(ucKey);
}

template<typename T1,typename T2>
bool PwwMap<T1,T2>::diskMapDel(T1& tKey)
{
	return diskMapDel((unsigned char*)(&tKey), sizeof(T1));
}

template <typename T1,typename T2>
bool PwwMap<T1,T2>::diskMap2HashMap()
{
	if (m_schMem != NULL)
		return false;
	m_iDynamicAlloc = 1;
	
	for (int i=0;i<m_sMapLen;i++)
	{
		m_pwwMap[i]->analyIni();
	}


	unsigned short  usVLen;
	unsigned char*ucValue;
	int pppLen = (PLEN<<1)+USLEN;
	unsigned char* ucPPpKl;

	pww__int64 lP = 0;
	while(m_fpDat->pwwRead1(ucPPpKl,USLEN,lP))//kl
	{
		unsigned short keyLen = *(unsigned short*)ucPPpKl;
		int pppkLen = pppLen+keyLen;
		int pppklLen = pppkLen+USLEN;
		if(m_fpDat->pwwRead1(ucPPpKl,pppklLen,lP+USLEN))
		{
			usVLen = *(unsigned short*)(ucPPpKl+pppkLen);//l

			if (!m_mpDel.memMapFind((unsigned char*)&lP,ucValue))//
			{
				m_pwwMap[m_iIndex[keyLen]]->analyIdx(ucPPpKl+pppLen);
			}
			lP += usVLen+pppklLen+USLEN;
		}
	}

	unsigned char* key = (unsigned char*)malloc(m_sIdxMemLen);
	lP = 0;
	while(m_fpDat->pwwRead1(ucPPpKl,USLEN,lP))//kl
	{
		unsigned short keyLen = *(unsigned short*)ucPPpKl;
		int pppkLen = pppLen+keyLen;
		int pppklLen = pppkLen+USLEN;
		if(m_fpDat->pwwRead1(ucPPpKl,pppklLen,lP+USLEN))
		{
			usVLen = *(unsigned short*)(ucPPpKl+pppkLen);//l

			if (!m_mpDel.memMapFind((unsigned char*)&lP,ucValue))//
			{
				memcpy(key,ucPPpKl+pppLen,keyLen);
				if (m_fpDat->pwwRead1(ucValue,usVLen,lP+pppklLen))// v
				{
					m_pwwMap[m_iIndex[keyLen]]->toHashMap(key, ucValue,usVLen);
				}
				else
					printf("error file read val\n");
			}
			lP += usVLen+pppklLen+USLEN;
		}
	}
	free(key);

	return true;
}

template <typename T1,typename T2>
void PwwMap<T1,T2>::diskMapForEach(  CPwwHash::loopKeyValue fun,void*p )
{
	unsigned short  usVLen;
	unsigned char*ucValue;
	unsigned char* key = (unsigned char*)malloc(m_sIdxMemLen);
	int pppLen = (PLEN<<1)+USLEN;
	unsigned char* ucPPpKl;

	pww__int64 lP = 0;
	while(m_fpDat->pwwRead1(ucPPpKl,USLEN,lP))//kl
	{
		unsigned short keyLen = *(unsigned short*)ucPPpKl;
		int pppkLen = pppLen+keyLen;
		int pppklLen = pppkLen+USLEN;
		if(m_fpDat->pwwRead1(ucPPpKl,pppklLen,lP+USLEN))
		{
			usVLen = *(unsigned short*)(ucPPpKl+pppkLen);//l

			if (!m_mpDel.memMapFind((unsigned char*)&lP,ucValue))//
			{
				memcpy(key,ucPPpKl+pppLen,keyLen);
				if (m_fpDat->pwwRead1(ucPPpKl,usVLen,lP+pppklLen))// v
				{
					ucValue = (unsigned char*)malloc(usVLen+VALUEPOS);
					memcpy(ucValue+VALUELENPOS,&usVLen,USLEN);
					memcpy(ucValue+VALUEPOS,ucPPpKl,usVLen);
					fun(key, keyLen,ucValue,p);
					free(ucValue);
				}
				else
					printf("error file read val\n");
			}
			lP += usVLen+pppklLen+USLEN;
		}
	}
	free(key);
}

template <typename T1,typename T2>
void PwwMap<T1,T2>::rebuildDiskData()
{
	if (m_fpDel->fileSize==0)
	{
		return;
	}

	PwwMap<T1,T2>* diskMapBk = new PwwMap<T1,T2>(); 
	diskMapBk->setDiskSign(-m_mapSign);

	unsigned short  usVLen;
	unsigned char*ucValue;
	unsigned char* key = (unsigned char*)malloc(m_sIdxMemLen);
	int pppLen = (PLEN<<1)+USLEN;
	unsigned char* ucPPpKl;

	pww__int64 lP = 0;
	while(m_fpDat->pwwRead1(ucPPpKl,USLEN,lP))//kl
	{
		unsigned short keyLen = *(unsigned short*)ucPPpKl;
		int pppkLen = pppLen+keyLen;
		int pppklLen = pppkLen+USLEN;
		if(m_fpDat->pwwRead1(ucPPpKl,pppklLen,lP+USLEN))
		{
			usVLen = *(unsigned short*)(ucPPpKl+pppkLen);//l

			if (!m_mpDel.memMapFind((unsigned char*)&lP,ucValue))//
			{
				memcpy(key,ucPPpKl+pppLen,keyLen);
				if (m_fpDat->pwwRead1(ucValue,usVLen,lP+pppklLen))// v
				{
					diskMapBk->diskMapInsert(key,keyLen,ucValue,usVLen);
				}
				else
					printf("error file read val\n");
			}
			lP += usVLen+pppklLen+USLEN;
		}
	}
	free(key);

	m_fpDat->closePwwRead();
	remove(m_fpDat->fileName);
	remove(m_fpDel->fileName);

	diskMapBk->m_fpDat->pwwFflush();
	diskMapBk->m_fpDat->closePwwRead();
	rename(diskMapBk->m_fpDat->fileName,m_fpDat->fileName);

	delete diskMapBk;

	m_mpDel.clearCharMem();
	m_fpDel->reSet();
	m_fpDat->reSet();

	m_fpDat->openPwwRead();
	readIdx();
}

template <typename T1,typename T2>
void PwwMap<T1,T2>::iniDiskMap()
{
	unsigned short  usVLen;
	unsigned char*ucValue;
	int pppLen = (PLEN<<1)+USLEN;
	unsigned char* ucPPpKl;

	pww__int64 lP = 0;
	while(m_fpDat->pwwRead1(ucPPpKl,USLEN,lP))//kl
	{
		unsigned short keyLen = *(unsigned short*)ucPPpKl;
		int pppkLen = pppLen+keyLen;
		int pppklLen = pppkLen+USLEN;
		if(m_fpDat->pwwRead1(ucPPpKl,pppklLen,lP+USLEN))
		{
			usVLen = *(unsigned short*)(ucPPpKl+pppkLen);//l

			if (!m_mpDel.memMapFind((unsigned char*)&lP,ucValue))//
			{
				if (keyLen>=m_sIdxMemLen)
				{
					int len = (keyLen<<1);
					if (len > 0xFFFF)
						len = 0xFFFF;

					m_iIndex = (int*)realloc(m_iIndex,sizeof(int)*len);
					memset(m_iIndex+m_sIdxMemLen,-1,sizeof(int)*(len-m_sIdxMemLen));
					m_sIdxMemLen = len;
				}
				if (-1 == m_iIndex[keyLen])
				{
					m_iIndex[keyLen] = m_sMapLen++;
					if (m_sMapLen >= m_sMapMemLen)
					{
						unsigned short sNewLen = (m_sMapMemLen<<1);
						m_pwwMap = (CPwwHash **)realloc(m_pwwMap,sNewLen*sizeof(CPwwHash *));
						m_sMapMemLen = sNewLen;
					}
					m_pwwMap[m_iIndex[keyLen]] = new CPwwDiskMap(keyLen,m_fpDat, m_fpDel, &m_mpDel);
				}
				
			}
			lP += usVLen+pppklLen+USLEN;
		}
	}
}

template <typename T1,typename T2>
void PwwMap<T1,T2>::readDel()
{
	pww__int64 lpos;
	unsigned char c=1;
	m_mpDel.clearCharMem();

	m_fpDel->openPwwRead();
	pww__int64 lP = 0;
	unsigned char* v;
	while(m_fpDel->pwwRead1(v,PLEN,lP)!=0)
	{
		lpos = *(pww__int64*)v;
		m_mpDel.memMapInsert((unsigned char*)&lpos,&c,PLEN);
		lP += PLEN;
	}
	m_fpDel->closePwwRead();
}

template <typename T1,typename T2>
void PwwMap<T1,T2>::readIdx() 
{
	unsigned char* ucPPpKl;
	unsigned char* c;
	unsigned char key[CHARINDEXDEEP];

	for (int i=0;i<m_sMapLen;i++)
		m_pwwMap[i]->clearCharMem();

	unsigned short  usVLen;
	int pppLen = (PLEN<<1)+USLEN;

	pww__int64 lP = 0;
	while(m_fpDat->pwwRead1(ucPPpKl,USLEN,lP))//kl
	{
		unsigned short keyLen = *(unsigned short*)ucPPpKl;
		int pppkLen = pppLen+keyLen;
		int pppklLen = pppkLen+USLEN;
		if(m_fpDat->pwwRead1(ucPPpKl,pppklLen,lP+USLEN))
		{
			usVLen = *(unsigned short*)(ucPPpKl+pppkLen);//l

			if (!m_mpDel.memMapFind((unsigned char*)&lP,c))//
			{
				memcpy(key,ucPPpKl+pppLen,CHARINDEXDEEP);//k
				if( !m_pwwMap[m_iIndex[keyLen]]->memMapInsert(key,(unsigned char*)&lP,PLEN) )
					m_pwwMap[m_iIndex[keyLen]]->memMapModify(key,(unsigned char*)&lP,PLEN);
			}
			lP += pppklLen+usVLen+USLEN;
		}
	}
}

