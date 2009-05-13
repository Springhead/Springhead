################################################################
#
# 	Makefile for Graphics
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

SRCS		= $(wildcard *.cpp)
OBJS 		= ${SRCS:.cpp=.o}


################################################################
#	build management
################################################################

all: swig $(TARGET_LIB)

swig:
	$(TOP_DIR)/src/Foundation/runswig Graphics Foundation

$(TARGET_LIB): $(OBJS)
		-$(RM) $@
		$(AR) $(ARFLAGS) $@ $(OBJS)	
		$(RANLIB) $@

.cpp.o:
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	-$(RM) $(OBJS) $(TARGET_LIB) $(TARGET_NAME)Stub.cpp

realclean: clean
	-$(RM) $(REALCLEAN_TARGET)


################################################################
#	THE END
################################################################
