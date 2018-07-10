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
include 	$(TOP_DIR)/make.func
$(eval TARGET_DIR := $(subst $(SP),@,$(shell env pwd)))
TARGET_NAME	:= $(shell env basename $(TARGET_DIR))
TARGET_LIB	:= lib$(TARGET_NAME).a

SRCS		= $(wildcard *.cpp)
OBJS 		= ${SRCS:.cpp=.o}

CXXFLAGS	:= $(CXXFLAGS) \
		$(call add_inc,${SPR_BOOST_INC}) \
		$(call add_inc,${SPR_GL_INC}) \
		-std=gnu++11 \
		-DWINGDIAPI= -DAPIENTRY= -DCALLBACK= \
		-DGLUTCALLBACK= -D__cdecl=\
		-Wno-deprecated-declarations


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
