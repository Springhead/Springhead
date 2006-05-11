################################################################
#
# 	Makefile for GRSimple
#
################################################################

################################################################
#	common rules
################################################################

TOP_DIR 		= ../../../..
include 		$(TOP_DIR)/make.header
TARGET_DIR  	:= $(shell env pwd)
TARGET_NAME		:= $(shell env basename $(TARGET_DIR))
EXECUTABLE		= $(TARGET_NAME).exe

SRCS			= $(wildcard *.cpp)
OBJS 			= ${SRCS:.cpp=.o}


################################################################
#	build management
################################################################

all: $(EXECUTABLE)
		
.cpp.o:
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(EXECUTABLE):$(OBJS)
	$(CXX) $(OBJS) -o $(EXECUTABLE) $(LDFLAGS)
			
clean:
	-$(RM) $(OBJS) $(EXECUTABLE)
	
realclean: clean
	-$(RM) $(REALCLEAN_TARGET)
		
			
################################################################
#	THE END
################################################################
