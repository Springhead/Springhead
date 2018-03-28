################################################################
#
# 	Makefile for tests
#
################################################################

################################################################
#	A rule to make sub directories.
################################################################
TOP_DIR 	= ../..

TARGET_DIRS	= \
	Base Collision FileIO Foundation Framework \
	Graphics HumanInterface Physics

SILENT		= --no-print-directory

all:
	@for dir in $(TARGET_DIRS); do \
		echo "--------------------------------------------"; \
		echo "	make $$dir."; \
		echo "--------------------------------------------"; \
		$(MAKE) $(SILENT) -C $$dir all || exit; \
	done


################################################################
#	Default rules
################################################################

clean:
	@echo ""
	@echo "--------------------------------------------"
	@echo "	making clean in tests ..."
	@echo "--------------------------------------------"
	@for dir in $(TARGET_DIRS); do \
		$(MAKE) $(SILENT) -s -C $$dir clean || exit; \
	done
	
realclean:
	@echo ""
	@echo "--------------------------------------------"
	@echo "	making real clean in tests ..."
	@echo "--------------------------------------------"
	@for dir in $(TARGET_DIRS); do \
		$(MAKE) $(SILENT) -s -C $$dir realclean || exit; \
	done

	
################################################################
# 	THE END
################################################################
