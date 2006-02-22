################################################################
#
# 	Makefile for Graphics test
#
################################################################

################################################################
#	A rule to make sub directories.
################################################################

SRC_DIR 	= ../..

SUB_DIRS	= GRSimple

all:
	@for dir in $(SUB_DIRS); \
	do echo "---------------------------------------------"; \
	   echo "             make $$dir    			      "; \
	   echo "---------------------------------------------"; \
	$(MAKE) -C $$dir all || exit; \
	done


################################################################
#	Default rules
################################################################

clean:
	@for dir in $(SUB_DIRS); \
	do $(MAKE) -C $$dir  clean || exit; \
	done
	
realclean:
	@for dir in $(SUB_DIRS); \
	do $(MAKE) -C $$dir  realclean || exit; \
	done 

help:
	@echo "Other targets are"
	@echo "  help       show this help message."
	@echo "  clean      delete all the object files."
	@echo "  realclean  delete all generated files. "


################################################################
# 	THE END
################################################################
