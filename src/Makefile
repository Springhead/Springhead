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

#SUB_DIRS		= Base \
				Collision \
				FileIO \
				Graphics \
				Physics
SUB_DIRS	= Physics

all:
	@for dir in $(SUB_DIRS); \
	do echo "---------------------------------------------"; \
	   echo "                 $$dir 				      "; \
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
