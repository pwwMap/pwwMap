#! /bin/bash

g++ -pipe -g ../pwwHashLib/PwwHashMem.h ../pwwHashLib/PwwHash.cpp ../pwwHashLib/PwwHash.h  -I./ -Wall -W -D_DEBUG -D_REENTRANT -D_PTHREADS -D_TRACE_TO_ALL -DSYB_LP32 -fPIC -shared -o libPwwHashMap.so

