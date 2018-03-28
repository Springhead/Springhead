################################################################
#
# 	Makefile for Samples
#
################################################################

################################################################
#	A rule to make sub directories.
################################################################
TOP_DIR 	= ../..

TARGET_DIRS	= \
	Creature EmbPython Foundation Framework \
	HumanInterface Physics Scilab

SILENT		= --no-print-directory

all:
	@for dir in $(TARGET_DIRS); do \
		echo "--------------------------------------------"; \
		echo "	make $$dir."; \
		echo "--------------------------------------------"; \
		$(MAKE) $(SILENT) -C $$dir all || exit; \
	done


################################################################
#	Dummy rules for dailybuild.
################################################################
compile:
test:


################################################################
#	Default rules
################################################################

clean:
	@echo ""
	@echo "--------------------------------------------"
	@echo "	making clean in Samples ..."
	@echo "--------------------------------------------"
	@for dir in $(TARGET_DIRS); do \
		$(MAKE) $(SILENT) -s -C $$dir clean || exit; \
	done
	
realclean:
	@echo ""
	@echo "--------------------------------------------"
	@echo "	making realclean in Samples ..."
	@echo "--------------------------------------------"
	@for dir in $(TARGET_DIRS); do \
		$(MAKE) $(SILENT) -s -C $$dir realclean || exit; \
	done


################################################################
# 	THE END
################################################################
