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

#ifndef _PWWHASH_H
#define _PWWHASH_H
#include "PwwHashMem.h"
#ifdef WIN32
#include <direct.h> //use _chdir() and _getcwd() function
#include <io.h> //use _findfirst() and findnext() function
#else
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#endif
#include <stdio.h>
#include <string.h>
#include <sstream>
#include <fstream>
#include <iostream>
#include <stdlib.h>
//#include <errno.h>

#if (defined WIN32 || defined _WIN32 || defined WINCE)
#ifndef PWWHASH_DLL
#define PWWHASH_EXPORTS __declspec(dllexport)
#else
#define PWWHASH_EXPORTS __declspec(dllimport)
#endif

#else
#define PWWHASH_EXPORTS 
#endif

#ifndef PWWHASH_EXTERN_C
#  ifdef __cplusplus
#    define PWWHASH_EXTERN_C extern "C"
#  else
#    define PWWHASH_EXTERN_C
#  endif
#endif

#ifdef WIN32
#define pww_fseeki64 _fseeki64
#define pww_ftelli64 _ftelli64
#define pww_access(a,b) _access(a,b)
#define pww_mkdir(a) _mkdir(a)
#define pww_rmdir _rmdir
#define pww_unlink _unlink
#define pww_itoa(p,kpos) itoa(kpos,p,10)
#define pww_DirSpilt '\\'
#define pww_LocalDir   "\\pww"
#else
#define pww_DirSpilt '/'
#define pww_LocalDir  "/pww"
#define pww_fseeki64 fseeko64
#define pww_ftelli64 ftello64
#define pww_access(a,b) access(a,b)
#define pww_mkdir(a) mkdir(a,S_IRUSR|S_IWUSR|S_IXUSR)
#define pww_rmdir rmdir
#define pww_unlink unlink
#define pww_itoa(p,kpos) sprintf(p,"%d",kpos)
#endif
#define DATAFILEEXT		".dat"
#define DATADEL			".del"
#define VALUEPOS 6
#define VALUELENPOS 4
#define MAPVALUEPOS 2
const unsigned char CHARINDEXDEEP = 3;
const unsigned short INDEXLEN = 256;	
const short PLEN = sizeof(pww__int64);
const short USLEN = sizeof(unsigned short);
const unsigned int RWBUF = 1024000; //diskmap read and write cache, used to adjust the performance
class PWWHASH_EXPORTS CPwwHash
{
protected:
	union U_SEARCHMEM
	{
		unsigned char* ucValue;	
		U_SEARCHMEM *next;		
		unsigned int isEnd;
	}	m_searchTree;			

	struct S_CHARTREE
	{
		S_CHARTREE()
		{
			memset(this,0,sizeof(*this));
		}
		unsigned char* ucStr;		
		S_CHARTREE *next;			
	}	*m_charTree;			

public:
	struct PWWHASH_EXPORTS S_SEARCHINDEX
	{
		S_SEARCHINDEX(int indexLen);
		~S_SEARCHINDEX();

		void insertIndex( unsigned char uc, int iDeep);
		bool hasIndex(char* szKey, int iLen);
		short** sIndex;					
		unsigned char** ucReIndex;		
		short* sIndexMem;				
		unsigned char* ucReIndexMem;	
		int iLenIndex;					
		unsigned short* usMaxNum;		
	}	*m_searchIndex;					

public:
	CPwwHash();
	CPwwHash(int iKeyLen);
	virtual ~CPwwHash(void);
	void setKeyLen(int iKeyLen);
	void clearCharMem();

	typedef S_PWWMEM<U_SEARCHMEM> searchMem;
	typedef S_PWWMEM<unsigned char> valueMem;
	typedef bool (*loopKeyValue)(unsigned char*& ucKey, int klen,unsigned char*& ucValue,void* p);

	//No delete and insert function, but performance is best
	//deleteCharTree -- whether or not delete memMap in the process of hashMap form.
	void memMap2HashMap(searchMem *schMem,valueMem *valMem,bool deleteCharTree=true); 
	bool isExist(unsigned char* data,int len,unsigned char*& ucValue);
	bool hashMapFind(unsigned char* ucKey,unsigned char*& ucValue);
	bool hashMapForEach( loopKeyValue fun,void*p);

	//Based on memory No hard disk consumption
	bool memMapInsert(unsigned char* ucKey,  unsigned char* ucValue = 0, unsigned short usValueLen = 0);
	bool delMemMap( S_CHARTREE * charTree,int iDeep, int iMlen);
	bool memMapFind(unsigned char* ucKey,  unsigned char*& ucValue);
	bool memMapModify(unsigned char* ucKey, unsigned char* ucValue, unsigned short usValueLen);
	bool memMapForEach(  loopKeyValue fun,void*p );
	bool memMapDel(unsigned char* ucKey);
	void memMapAnalaysis(pww__int64& ulsearchMemLen,pww__int64& ulValMemBlocklen);

	virtual bool diskMapInsert(unsigned char* ucKey,  unsigned char* ucValue = 0, unsigned short usValueLen = 0){return true;};
	virtual bool diskMapFind(unsigned char* ucKey,  unsigned char*& ucValue){return true;};
	virtual bool diskMapModify(unsigned char* ucKey,  unsigned char*& ucValue, unsigned short usValueLen){return true;};
	virtual bool diskMapDel(unsigned char* ucKey){return true;};
	virtual void delHashMapMem(){};
	virtual void analyIni(){};
	virtual void analyIdx(unsigned char* ucKey){};
	virtual bool toHashMap(unsigned char* ucKey,  unsigned char* ucValue, unsigned short usValueLen){return true;};

	//You can customize the external function 'printKV', As a parameter passed to the 'MapForeach' function
	static bool printKV(unsigned char*& ucKey, int klen,unsigned char*& ucValue,void* p)
	{
			//	std::cout<<std::string((char*)ucKey,3)<<"		" << *(pww__int64*)&ucValue[VALUEPOS]<<std::endl;
		return true;
	};
	static char* pwwVersion();

protected:
	unsigned short m_iKeyLen;
};

class PWWHASH_EXPORTS CPwwDiskMap :public CPwwHash
{
public:
	struct PWWHASH_EXPORTS S_PwwFile
	{
		S_PwwFile(){};
		S_PwwFile(char* filename,int rBufLen=RWBUF>>3,int wBufLen=RWBUF);

		~S_PwwFile();

		void reSet();
		void openPwwRead();
		void closePwwRead();
		bool pwwRead1(unsigned char *& ptr, unsigned int len, pww__int64 pos);
		bool pwwRead2(unsigned char *& ptr, unsigned int len, pww__int64 pos);

		void pwwWrite(void * ptr, unsigned int  len);
		void pwwFflush();
		bool checkPos();
		void checkWbuf(unsigned int  len);
		
		pww__int64 fileSize;
		unsigned char* WBuf;
		unsigned int WPos;
		unsigned int WBufLen;
		FILE* fpR;
		char* fileName;

		unsigned char* RBuf;
		pww__int64 curPos;
		unsigned int RPos;
		unsigned int RBufLen;
	};

public:
	CPwwDiskMap(int iKeyLen,S_PwwFile *fpDat,S_PwwFile* fpDel,CPwwHash* mpDel);
	virtual ~CPwwDiskMap();

	//Based on the hard disk No memory consumption
	virtual bool diskMapInsert(unsigned char* ucKey,  unsigned char* ucValue = 0, unsigned short usValueLen = 0);
	virtual bool diskMapFind(unsigned char* ucKey,  unsigned char*& ucValue);
	virtual bool diskMapModify(unsigned char* ucKey,  unsigned char*& ucValue, unsigned short usValueLen);
	virtual bool diskMapDel(unsigned char* ucKey);
	virtual void delHashMapMem();
	virtual void analyIni();
	virtual void analyIdx(unsigned char* ucKey);
	virtual bool toHashMap(unsigned char* ucKey,  unsigned char* ucValue, unsigned short usValueLen);
	/*
		path:	the data path 
		pathbk:	the temporary folder path to organize the fragments.
	*/
	static void pwwDiskMapIni(char* path,char*pathbk);
	static void pwwDiskMapRelease();
	static 	void mkMapDir(char* mapPath);
	static void getPwwDiskPath(char* pchPath,int sign);
	static char* g_diskMapPath[2];
	static CPwwHash g_mpSign;

private:
	unsigned short getPos( pww__int64& nPos,unsigned short& vL,pww__int64& lPos,unsigned char* ucKey);

private:
	CPwwHash* m_mpDel;
	S_PwwFile *m_fpDat,*m_fpDel;
	unsigned short m_diskKeyLen;
	int m_iLPPpKlLen;
};
CPwwHash CPwwDiskMap::g_mpSign(sizeof(int));

#endif
