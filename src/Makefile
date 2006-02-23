################################################################
#
# 	Makefile for gcc 3.4 on cygwin and Linux.
#
################################################################

################################################################
#	A rule to make sub directories.
################################################################

export SRC_DIR := $(shell /bin/pwd)
include $(SRC_DIR)/make.header

LIB_DIRS		= Base \
				Collision \
				FileIO \
				Graphics \
				Physics \
				Foundation
TESTS_DIR		= tests 
SAMPLES_DIR		= Samples

SUB_DIRS = $(LIB_DIRS) $(TESTS_DIR) $(SAMPLES_DIR)

all: library tests-all Samles-all

library:
	@for dir in $(LIB_DIRS); \
	do echo "============================================="; \
	   echo "       Make $$dir library.   			      "; \
	   echo "============================================="; \
	$(MAKE) -C $$dir all || exit; \
	done	

tests-all:		
	@for dir in $(TESTS_DIR); \
	do echo "============================================="; \
	   echo "             Entring $$dir 			      "; \
	   echo "============================================="; \
	$(MAKE) -C $$dir all || exit; \
	done
	
Samples-all:		
	@for dir in $(SAMPLES_DIR); \
	do echo "============================================="; \
	   echo "             Entring $$dir 			      "; \
	   echo "============================================="; \
	$(MAKE) -C $$dir all || exit; \
	done	


################################################################
#	Default rules
################################################################

clean:
	@echo "*----*----*----*----*----*----*----*----*----*----*----*----*----*"
	@echo " [clean] : removes objects and binary. "
	@echo "*----*----*----*----*----*----*----*----*----*----*----*----*----*"
	@for dir in $(SUB_DIRS); \
	do $(MAKE) -C $$dir  clean || exit; \
	done
	
realclean:
	@echo "*----*----*----*----*----*----*----*----*----*----*----*----*----*"
	@echo " [realclean] : remove all generated files. "
	@echo "*----*----*----*----*----*----*----*----*----*----*----*----*----*"
	@for dir in $(SUB_DIRS); \
	do $(MAKE) -C $$dir  realclean || exit; \
	done 

help:
	@echo "Other targets are"
	@echo "  help          show this help message"
	@echo "  all           builds project - DEFAULT"
	@echo "  library       creates library"
	@echo "  tests-all     builds tests"
	@echo "  Samples-all   builds Samples"
	@echo "  clean         removes objects and binary"
	@echo "  realclean     remove all generated files" 


################################################################
# 	THE END
################################################################