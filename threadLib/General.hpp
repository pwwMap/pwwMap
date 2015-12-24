#ifndef __GENERAL_HPP__
#define __GENERAL_HPP__

#ifdef WIN32
#pragma warning (disable : 4786)
#pragma warning (disable : 4996)
#pragma warning (disable : 4267)
#pragma warning (disable : 4290)
#pragma warning (disable : 4018)
#pragma warning (disable : 4503)
#pragma warning (disable : 4541)
#pragma warning (disable : 4200)
#endif

#ifdef WIN32
#include <WINSOCK.H>
#pragma comment(lib, "Ws2_32.lib")
#pragma  
#else
#include <arpa/inet.h>
#endif

#include <cstdlib>
#include <cstring>
#include <ctime>
#include <cstdarg>


#include <list>
#include <map>
#include <set>
#include <vector>
#include <queue>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <strstream>
using namespace std;


typedef unsigned char u_char;
typedef unsigned short u_short;
typedef unsigned int u_int;
typedef unsigned long u_long;


#endif

