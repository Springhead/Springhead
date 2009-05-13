################################################################
#
# 	Makefile for Physics
#
################################################################

################################################################
#	common rules
################################################################
TOP_DIR 	= ../..
include 	$(TOP_DIR)/make.header
TARGET_DIR  := $(shell env pwd)
TARGET_NAME	:= $(shell env basename $(TARGET_DIR))
TARGET_LIB	:= lib$(TARGET_NAME).a

FILTEROUT	= \
				PHUniversalJoint.h \
				PHUniversalJoint.cpp
SRCS		= $(wildcard *.cpp)
SRCS		:= $(filter-out $(FILTEROUT), $(SRCS))
OBJS 		= ${SRCS:.cpp=.o}


################################################################
#	build management
################################################################

all: swig $(TARGET_LIB)

swig:
	$(TOP_DIR)/src/Foundation/runswig Physics Collision Foundation

$(TARGET_LIB): $(OBJS)
		-$(RM) $@
		$(AR) $(ARFLAGS) $@ $(OBJS)	
		$(RANLIB) $@

.cpp.o:
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	-$(RM) $(OBJS) $(TARGET_LIB)

realclean: clean
	-$(RM) $(REALCLEAN_TARGET)


################################################################
#	THE END
################################################################
