################################################################
#
# 	Makefile for Physics test
#
################################################################

################################################################
#	A rule to make sub directories.
################################################################

SRC_DIR 	= ../..

SUB_DIRS	= PHConstraintTest \
				PHShape \
				PHShapeGL \
				PHSimple \
				PHSimpleGL

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
	@echo "*----*----*----*----*----*----*----*----*----*"
	@echo " [clean] : delete all the object files. "
	@echo "*----*----*----*----*----*----*----*----*----*"
	@for dir in $(SUB_DIRS); \
	do $(MAKE) -C $$dir  clean || exit; \
	done
	
realclean:
	@echo "*----*----*----*----*----*----*----*----*----*"
	@echo " [realclean] : delete all generated files. "
	@echo "*----*----*----*----*----*----*----*----*----*"
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
