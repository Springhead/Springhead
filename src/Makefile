# Makefile for cygwin (gcc 3.4.4)

INCLUDE=-I../include -I./Base/ -I.

SRCS = Base/Affine.cpp \
		Base/BaseDebug.cpp \
		Base/BaseTypeINfo.cpp \
		Base/BaseUtility.cpp \
		Base/StrbufBase.cpp
		
OBJS = Base/Affine.o \
		Base/BaseDebug.o \
		Base/BaseTypeInfo.o \
		Base/BaseUtility.o \
		Base/StrbufBase.o
		
all: libbase

libbase: $(OBJS) $(SRCS)
		g++ $(INCLUDE) -c $(SRCS)
	
.cpp.o:
		g++ $(INCLUDE) -c $<
		


