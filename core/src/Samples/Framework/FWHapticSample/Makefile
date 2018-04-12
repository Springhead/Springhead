################################################################
#
# 	Makefile for FWHapticSample
#
################################################################

################################################################
#	common rules
################################################################

TOP_DIR 	= ../../../..
include 	$(TOP_DIR)/make.header
include 	$(TOP_DIR)/make.func

MAKE_DIR  	= $(shell env pwd)
MAKE_DIRNAME	= $(shell env basename $(MAKE_DIR))
BIN_NAME  	= $(MAKE_DIRNAME)

SRCS		= $(wildcard *.cpp)
OBJS 		= ${SRCS:.cpp=.o}
REALCLEANS	= $(wildcard log/*)

CXXFLAGS	:= $(CXXFLAGS) \
		-D__cdecl= \
		-std=gnu++11

LDFLAGS		:= $(LDFLAGS) \
		-llapack


################################################################
#	compile, link and go
################################################################
		
all:		compile test

.cpp.o:
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

compile:	$(OBJS)
	$(CXX) $(OBJS) -o $(BIN_NAME) $(LDFLAGS)

test:	
	./$(BIN_NAME)


################################################################
#	clean up
################################################################

clean:
	@echo "entering $(MAKE_DIRNAME)"
	-$(RM) $(OBJS) $(BIN_NAME)
	
realclean:	clean
	-$(RM) $(REALCLEANS)
			
			
################################################################
#	THE END
################################################################
