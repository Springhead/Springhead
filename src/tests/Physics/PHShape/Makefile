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
LDFLAGS			+= -L$(SRC_DIR)/Base/ \
					-L$(SRC_DIR)/Collision/ \
					-L$(SRC_DIR)/Foundation/ \
					-L$(SRC_DIR)/Physics/ \
					-lBase -lCollision -lFoundation -lPhysics


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
	
VCBUILD = $(wildcard *.ncb *.suo Debug* Release*)
realclean: clean
	-$(RM) $(VCBUILD)
			
			
################################################################
#	THE END
################################################################
