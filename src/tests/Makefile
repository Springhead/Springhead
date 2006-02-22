################################################################
#
# 	Makefile for tests
#
################################################################

################################################################
#	A rule to make sub directories.
################################################################

SRC_DIR 	= ..

SUB_DIRS	= FileIO Graphics Physics

all:
	@for dir in $(SUB_DIRS); \
	do $(MAKE) -C $$dir all || exit; \
	done


################################################################
#	Default rules
################################################################

clean:
	@echo ""
	@echo "*----*----*----*----*----*----*----*----*----*----*----*----*----*"
	@echo " [clean] : delete all the object files in tests directory. "
	@echo "*----*----*----*----*----*----*----*----*----*----*----*----*----*"
	@for dir in $(SUB_DIRS); \
	do $(MAKE) -C $$dir  clean || exit; \
	done
	
realclean:
	@echo ""
	@echo "*----*----*----*----*----*----*----*----*----*----*----*----*----*"
	@echo " [realclean] : delete all generated files in tests directory. "
	@echo "*----*----*----*----*----*----*----*----*----*----*----*----*----*"
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
