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
include 	$(TOP_DIR)/make.func
$(eval TARGET_DIR := $(subst $(SP),@,$(shell env pwd)))
TARGET_NAME	:= $(shell env basename $(TARGET_DIR))
TARGET_LIB	:= lib$(TARGET_NAME).a

FILTEROUT	= \
		PHUniversalJoint.h \
		PHUniversalJoint.cpp

SRCS		= $(wildcard *.cpp)
SRCS		:= $(filter-out $(FILTEROUT), $(SRCS))
OBJS 		= ${SRCS:.cpp=.o}

CXXFLAGS	:= $(CXXFLAGS) \
		$(call add_inc,${SPR_BOOST_INC}) \
		-std=gnu++11

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
