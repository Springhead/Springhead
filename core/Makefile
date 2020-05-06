################################################################
#
# 	Makefile for g++ 5.4 on Linux.
#
################################################################

################################################################
#	A rule to make sub directories.
################################################################

export TOP_DIR := $(shell env pwd)
include $(TOP_DIR)/make.header

SUB_DIRS	= src

##
all: 
	@for dir in $(SUB_DIRS); do \
	    $(MAKE) -s -C $$dir all || exit; \
	done

library:
	@for dir in $(SUB_DIRS); do \
	    $(MAKE) -s -C $$dir library || exit; \
	done	

install:
	@for dir in $(SUB_DIRS); do \
	    $(MAKE) -s -C $$dir install || exit; \
	done	

##
test:		
	@for dir in $(SUB_DIRS); do \
	    $(MAKE) -s -C $$dir test || exit; \
	done

tests-all:		
	@for dir in $(SUB_DIRS); do \
	    $(MAKE) -s -C $$dir tests-all || exit; \
	done
	
samples-all:		
	@for dir in $(SUB_DIRS); do \
	    $(MAKE) -s -C $$dir samples-all || exit; \
	done
	
##
clean:
	@for dir in $(SUB_DIRS); do \
	    echo "*----*----*----*----*----*----*----*----*----"; \
	    echo "       making clean in $$dir ...	       "; \
	    echo "*----*----*----*----*----*----*----*----*----"; \
	    $(MAKE) -s -C $$dir clean || exit; \
	done
	
clean-library:
	@for dir in $(SUB_DIRS); do \
	    echo "*----*----*----*----*----*----*----*----*----"; \
	    echo "       making clean-library in $$dir ...     "; \
	    echo "*----*----*----*----*----*----*----*----*----"; \
	    $(MAKE) -s -C $$dir clean-library || exit; \
	done
	
clean-tests:
	@for dir in $(SUB_DIRS); do \
	    echo "*----*----*----*----*----*----*----*----*----"; \
	    echo "       making clean-tests in $$dir ..	       "; \
	    echo "*----*----*----*----*----*----*----*----*----"; \
	    $(MAKE) -s -C $$dir clean-tests || exit; \
	done
	
clean-samples:
	@for dir in $(SUB_DIRS); do \
	    echo "*----*----*----*----*----*----*----*----*----"; \
	    echo "       making clean-samples in $$dir ...     "; \
	    echo "*----*----*----*----*----*----*----*----*----"; \
	    $(MAKE) -s -C $$dir clean-samples || exit; \
	done
	
##
realclean:
	@for dir in $(SUB_DIRS); do \
	    echo "*----*----*----*----*----*----*----*----*----"; \
	    echo "       making realclean in $$dir ...	       "; \
	    echo "*----*----*----*----*----*----*----*----*----"; \
	    $(MAKE) -s -C $$dir realclean || exit; \
	done

realclean-library:
	@for dir in $(SUB_DIRS); do \
	    echo "*----*----*----*----*----*----*----*----*----"; \
	    echo "       making realclean-library in $$dir ... "; \
	    echo "*----*----*----*----*----*----*----*----*----"; \
	    $(MAKE) -s -C $$dir realclean-library || exit; \
	done

realclean-tests:
	@for dir in $(SUB_DIRS); do \
	    echo "*----*----*----*----*----*----*----*----*----"; \
	    echo "       making realclean-tests in $$dir ...   "; \
	    echo "*----*----*----*----*----*----*----*----*----"; \
	    $(MAKE) -s -C $$dir realclean-tests || exit; \
	done

realclean-samples:
	@for dir in $(SUB_DIRS); do \
	    echo "*----*----*----*----*----*----*----*----*----"; \
	    echo "       making realclean-samples in $$dir ... "; \
	    echo "*----*----*----*----*----*----*----*----*----"; \
	    $(MAKE) -s -C $$dir realclean-samples || exit; \
	done

##
help:
	@echo "Other targets are"
	@echo "  all                builds project - DEFAULT"
	@echo "  library            creates library and install it"
	@echo "  tests-all          builds tests"
	@echo "  samples-all        builds Samples"
	@echo "  clean              removes objects and binary"
	@echo "  realclean          remove all generated files" 
	@echo "  help               show this help message"


################################################################
# 	THE END
################################################################
