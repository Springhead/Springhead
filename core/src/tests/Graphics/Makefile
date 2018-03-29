################################################################
#
# 	Makefile for Graphics
#
################################################################

TOP_DIR 	= ../..
MAKE_DIR	:= $(shell env pwd)
MAKE_DIRNAME	:= $(shell env basename $(MAKE_DIR))

TARGET_DIRS	= \
	GRColorSpheres GRSimple GRVertexBlending

SILENT		= --no-print-directory

##
all:
	@for dir in $(TARGET_DIRS); do \
		echo "--------------------------------------------"; \
		echo "	make $$dir."; \
		echo "--------------------------------------------"; \
		$(MAKE) -C $$dir all || exit; \
	done


################################################################
#	Default rules
################################################################

clean:
	@echo ""
	@echo "--------------------------------------------"
	@echo "	making clean in $(MAKE_DIRNAME) ..."
	@echo "--------------------------------------------"
	@for dir in $(TARGET_DIRS); do \
		$(MAKE) $(SILENT) -s -C $$dir clean || exit; \
	done
	
realclean:
	@echo ""
	@echo "--------------------------------------------"
	@echo "	making realclean in $(MAKE_DIRNAME) ..."
	@echo "--------------------------------------------"
	@for dir in $(TARGET_DIRS); do \
		$(MAKE) $(SILENT) -s -C $$dir realclean || exit; \
	done

	
################################################################
# 	THE END
################################################################

