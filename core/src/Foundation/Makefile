################################################################
#
# 	Makefile for Foundation
#
################################################################

################################################################
#	common rules
################################################################
TOP_DIR 	= ../..
include 	$(TOP_DIR)/make.header
include 	$(TOP_DIR)/make.func
$(eval TARGET_DIR := $(subst $(SP),@,$(shell env pwd)))
TARGET_NAME	:= $(shell env basename $(TARGET_DIR))
TARGET_LIB	:= lib$(TARGET_NAME).a

FILTEROUT	= \
		UTMMTimer.cpp

SRCS		= $(wildcard *.cpp)
SRCS		:= $(filter-out $(FILTEROUT), $(SRCS))
OBJS 		= ${SRCS:.cpp=.o}

CXXFLAGS	:= $(CXXFLAGS) \
		$(call add_inc,${SPR_BOOST_INC}) \
		-std=gnu++11 \
		-fpermissive


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
	-$(RM) $(OBJS) $(TARGET_LIB) $(TARGET_NAME)Stub.cpp

realclean: clean
	-$(RM) $(REALCLEAN_TARGET)


################################################################
#	THE END
################################################################
