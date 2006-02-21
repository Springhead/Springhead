################################################################
#
# 	Makefile for PHShape
#
################################################################

################################################################
#	common rules
################################################################

SRC_DIR 		= ../../..
include 		$(SRC_DIR)/make.header
TARGET_DIR  	:= $(shell /bin/pwd)
TARGET_NAME		:= $(shell /bin/basename $(TARGET_DIR))
EXECUTABLE		= $(TARGET_NAME).exe

SRCS			= $(wildcard *.cpp)
OBJS 			= ${SRCS:.cpp=.o}

INCLUDES		+=  -I.
LDFLAGS			+= -L../../../Base/ \
					-L../../../Collision/ \
					-L../../../Foundation/ \
					-L../../../Physics/ \
					-lBase -lCollision -lFoundation -lPhysics

################################################################
#	build management
################################################################
		
.cpp.o:
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(EXECUTABLE):$(OBJS)
	$(CXX) $(OBJS) -o $(EXECUTABLE) $(LDFLAGS)
			
clean:
	-$(RM) $(OBJS)
	
BAK		= $(wildcard *.BAK)
BACK	= $(wildcard *.back)
CORE	= $(wildcard core*)
realclean:
	-$(RM) $(OBJS) $(EXECUTABLE) $(BAK) $(BACK) $(CORE)
			
################################################################
#	THE END
################################################################
