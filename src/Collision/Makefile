################################################################
#
# 	Makefile for Collision
#
################################################################

################################################################
#	common rules
################################################################
SRC_DIR 	= ..
include 	$(SRC_DIR)/make.header
TARGET_DIR  := $(shell /bin/pwd)
TARGET_NAME	:= $(shell /bin/basename $(TARGET_DIR))
TARGET_LIB	:= lib$(TARGET_NAME).a

SRCS		= $(wildcard *.cpp)
OBJS 		= ${SRCS:.cpp=.o}
	

################################################################
#	build management
################################################################

all: $(TARGET_LIB)

$(TARGET_LIB): $(OBJS)
		-$(RM) $@
		$(AR) r $@ $?	
		$(RANLIB) $@
	
.cpp.o:
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@
			
clean:
	-$(RM) $(OBJS) $(TARGET_LIB)
	
VCPROOUT 	= $(wildcard *.lib Debug* Release*)
DOXYOUT 	= $(wildcard html *.tag *.log *.tmp)
realclean: clean
	-$(RM) $(VCPROOUT) $(DOXYOUT)
			
			
################################################################
#	THE END
################################################################
