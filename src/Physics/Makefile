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
	
VCPROOUT 	= $(wildcard *.lib Debug* Release* *.ncb)
DOXYOUT 	= $(wildcard html *.tag *.log *.tmp)
realclean: clean
	-$(RM) $(VCPROOUT) $(DOXYOUT)
	
			
################################################################
#	THE END
################################################################
