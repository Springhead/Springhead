################################################################
#
# 	Makefile for Framework
#
################################################################

################################################################
#	common rules
################################################################
TOP_DIR		= ../..
include 	$(TOP_DIR)/make.header
TARGET_DIR	:= $(shell env pwd)
TARGET_NAME	:= $(shell env basename $(TARGET_DIR))
TARGET_LIB	:= lib$(TARGET_NAME).a

SRCS		= $(wildcard *.cpp)
OBJS 		= ${SRCS:.cpp=.o}

CXXFLAGS	= -std=gnu++11 \
		-DWINGDIAPI= -DAPIENTRY= -DCALLBACK= \
                -DGLUTCALLBACK= -D__cdecl=


################################################################
#	build management
################################################################

all: $(TARGET_LIB)

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
