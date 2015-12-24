CXXFLAGS = -o -D_DEBUG -D_REENTRANT -D_PTHREADS -D_TRACE_TO_ALL -DSYB_LP32 #-D_TEST_G_
XERCES := /opt
INCLUDE = -I./ \
	-I./pwwhash \
	-I../pwwHash/src \
	-I../threadLib \
	-I../pwwHashLib 
	
LIB = -L./lib -L./ 
LIBS = -lPwwHashMap -ldl -lpthread 

CXX = g++ $(INCLUDE)

TARGET = dlltest 

MAIN_DIR = ./pwwhash
SRC_DIR = ./src
THREAD_DIR = ../threadLib

MAIN_SRCS := $(wildcard $(MAIN_DIR)/main.cpp)
SRC_SRCS := $(wildcard $(SRC_DIR)/*.cpp)
THREAD_SRCS := $(wildcard $(THREAD_DIR)/*.cpp)

MAIN_OBJS := $(patsubst %.cpp, %.o, $(MAIN_SRCS))
SRC_OBJS := $(patsubst %.cpp, %.o, $(SRC_SRCS))
THREAD_OBJS := $(patsubst %.cpp, %.o, $(THREAD_SRCS))

SRCS := $(wildcard *.cpp)
OBJS := $(patsubst %.cpp, %.o, $(SRCS))

%.d: %.cpp
		@set -e; rm -f $@; \
		$(CXX) -MM $(CXXFLAGS) $< > $@.$$$$; \
		sed 's,.*\.o[ ]*:,$*.o $@ :,g'< $@.$$$$ > $@; \
		rm -f $@.$$$$
		$(CXX) $(CXXFLAGS) -c -o $*.o $<

$(TARGET) :  $(MAIN_OBJS) $(SRC_OBJS)  $(THREAD_OBJS) 
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(MAIN_OBJS) $(SRC_OBJS) $(THREAD_OBJS)  $(LIBS) $(LIB)

include $(MAIN_SRCS:.cpp=.d)
include $(SRC_SRCS:.cpp=.d)
include $(THREAD_SRCS:.cpp=.d)

clean:
	rm -f $(MAIN_DIR)/*.[od] $(SRC_DIR)/*.[od] $(THREAD_DIR)/*.[od] $(TARGET)

