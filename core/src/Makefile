################################################################
#
# 	Makefile for src
#
################################################################

################################################################
#	A rule to make sub directories.
################################################################
TOP_DIR		= ..
include 	$(TOP_DIR)/make.header
include		$(TOP_DIR)/make.func

Python = $(call def,$(PYTHON),$(SPR_PYTHON),python)
InsDir = $(call def,$(LIB_INS_DIR),$(SPR_LIB_INS_DIR),../lib)

RUNSWIG_DIR	= RunSwig
LIBS_DIR	= Base \
		  Collision \
		  Creature \
		  FileIO \
		  Foundation \
		  Framework \
		  Graphics \
		  HumanInterface \
		  Physics

TESTS_DIR	= tests 
SAMPLES_DIR	= Samples
SRC_SUB_DIRS 	= $(LIBS_DIR) $(TESTS_DIR) $(SAMPLES_DIR)

SPRINGHEAD_LIB 	= libSpringhead.a	

ALL_OBJS=$(foreach DIR, $(LIBS_DIR), \
		$(foreach src, $(wildcard $(DIR)/*.o), \
		$(basename $(src)).o ))

SILENT		= --no-print-directory


all:	install test

##
compile:	install

##
library:	runswig
	@for dir in $(LIBS_DIR); do \
		echo "============================================"; \
		echo "	Make $$dir Library."; \
		echo "============================================"; \
		$(MAKE) PYTHON=$(Python) $(SILENT) -C $$dir all || exit; \
	done	
	$(AR) $(ARFLAGS) $(SPRINGHEAD_LIB) $(ALL_OBJS) 
	$(RANLIB) $(SPRINGHEAD_LIB)

runswig:	
	@echo "============================================"
	@echo "	Make RunSwig."
	@echo "============================================"
	$(MAKE) PYTHON=$(Python) $(SILENT) -C $(RUNSWIG_DIR) compile || exit

install:	library
	mkdir -p $(InsDir)
	install $(SPRINGHEAD_LIB) $(InsDir)

##
test:		install tests-all samples-all
	
tests-all:		
	@for dir in $(TESTS_DIR); do \
		echo "============================================"; \
		echo "	Entering $$dir."; \
		echo "============================================"; \
		$(MAKE) PYTHON=$(Python) $(SILENT) -C $$dir $(TARGET) || exit; \
	done
	
samples-all:		
	@for dir in $(SAMPLES_DIR); do \
		echo "============================================"; \
		echo "	Entering $$dir."; \
		echo "============================================"; \
		$(MAKE) PYTHON=$(Python) $(SILENT) -C $$dir $(TARGET) || exit; \
	done	
		
##
clean:		clean-library clean-tests clean-samples

clean-library:
	@for dir in $(LIBS_DIR); do \
		$(MAKE) PYTHON=$(Python) $(SILENT) -s -C $$dir clean || exit; \
	done
	$(MAKE) PYTHON=$(Python) $(SILENT) -s -C $(RUNSWIG_DIR) clean || exit
	
clean-tests:
	@for dir in $(TESTS_DIR); do \
		$(MAKE) PYTHON=$(Python) $(SILENT) -s -C $$dir clean || exit; \
	done

clean-samples:
	@for dir in $(SAMPLES_DIR); do \
		$(MAKE) PYTHON=$(Python) $(SILENT) -s -C $$dir clean || exit; \
	done

##
realclean:	realclean-library realclean-tests realclean-samples

realclean-library:
	@for dir in $(LIBS_DIR); do \
		$(MAKE) PYTHON=$(Python) $(SILENT) -s -C $$dir realclean || exit; \
	done 
	$(MAKE) PYTHON=$(Python) $(SILENT) -s -C $(RUNSWIG_DIR) realclean || exit
	-/bin/rm -f $(InsDir)/$(SPRINGHEAD_LIB)

realclean-tests:
	@for dir in $(TESTS_DIR); do \
		$(MAKE) PYTHON=$(Python) $(SILENT) -s -C $$dir realclean || exit; \
	done

realclean-samples:
	@for dir in $(SAMPLES_DIR); do \
		$(MAKE) PYTHON=$(Python) $(SILENT) -s -C $$dir realclean || exit; \
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
