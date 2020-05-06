################################################################
#
# 	Makefile for gcc 3.4 on cygwin and Linux.
#
################################################################

################################################################
#	A rule to make sub directories.
################################################################

export TOP_DIR := $(shell env pwd)
include $(TOP_DIR)/make.header

SUB_DIRS		= src

all: 
	@for dir in $(SUB_DIRS); \
	do $(MAKE) -C $$dir all || exit; \
	done

library:
	@for dir in $(SUB_DIRS); \
	do $(MAKE) -C $$dir library || exit; \
	done	

tests-all:		
	@for dir in $(SUB_DIRS); \
	do $(MAKE) -C $$dir tests-all || exit; \
	done
	
samples-all:		
	@for dir in $(SUB_DIRS); \
	do $(MAKE) -C $$dir samples-all || exit; \
	done
	
package-library: 	
	@for dir in $(SUB_DIRS); \
	do $(MAKE) -C $$dir tests-all || exit; \
	done
	

################################################################
#	Default rules
################################################################

clean:
	@for dir in $(SUB_DIRS); \
	do echo "*----*----*----*----*----*----*----*----*----*----*----*----*----*"; \
	   echo "               making clean in $$dir ...                          "; \
	   echo "*----*----*----*----*----*----*----*----*----*----*----*----*----*"; \
	$(MAKE) -C $$dir  clean || exit; \
	done
	
realclean:
	@for dir in $(SUB_DIRS); \
	do echo "*----*----*----*----*----*----*----*----*----*----*----*----*----*"; \
	   echo "               making realclean in $$dir ...                      "; \
	   echo "*----*----*----*----*----*----*----*----*----*----*----*----*----*"; \
	$(MAKE) -C $$dir  realclean || exit; \
	done

help:
	@echo "Other targets are"
	@echo "  help               show this help message"
	@echo "  all                builds project - DEFAULT"
	@echo "  library            creates library"
	@echo "  tests-all          builds tests"
	@echo "  samples-all        builds Samples"
	@echo "  package-library    creating the package library"	
	@echo "  clean              removes objects and binary"
	@echo "  realclean          remove all generated files" 


################################################################
# 	THE END
################################################################