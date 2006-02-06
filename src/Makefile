# Makefile for cygwin (gcc 3.4.4)

TARGET		= Base
TARGET_DIR	= $(TARGET)
LIB 		= $(TARGET_DIR)/$(TARGET).lib

SRCS		= $(wildcard $(TARGET_DIR)/*.cpp)
OBJS 		= ${SRCS:.cpp=.o}
INCLUDE		= -I../include -I./$(TARGET_DIR)/ -I.
CXX			= g++
AR			= ar
RM			= rm -f
RANLIB		= ranlib

		
all: $(LIB)

$(LIB): $(OBJS)
		-$(RM) $@
		$(AR) r $@ $?	
		$(RANLIB) $@
	
.cpp.o:
		$(CXX) $(INCLUDE) -c $< -o $@
			
clean:
	-$(RM) $(OBJS) $(LIB)
		
		
