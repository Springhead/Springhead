################################################################
#
# 	Makefile for src
#
################################################################

################################################################
#	A rule to make sub directories.
################################################################

TOP_DIR			= ..

LIBS_DIR		= Base \
				Collision \
				FileIO \
				Graphics \
				Physics \
				Foundation \
				Creature
TESTS_DIR		= tests 
SAMPLES_DIR		= Samples

SRC_SUB_DIRS = $(LIBS_DIR) $(TESTS_DIR) $(SAMPLES_DIR)

all: library tests-all samples-all

library:
	@for dir in $(LIBS_DIR); \
	do echo "============================================="; \
	   echo "       Make $$dir Library.   			      "; \
	   echo "============================================="; \
	$(MAKE) -C $$dir all || exit; \
	done	

tests-all:		
	@for dir in $(TESTS_DIR); \
	do echo "============================================="; \
	   echo "             Entring $$dir 			      "; \
	   echo "============================================="; \
	$(MAKE) -C $$dir all || exit; \
	done
	
samples-all:		
	@for dir in $(SAMPLES_DIR); \
	do echo "============================================="; \
	   echo "             Entring $$dir 			      "; \
	   echo "============================================="; \
	$(MAKE) -C $$dir all || exit; \
	done	


################################################################
#	Default rules
################################################################

clean:
	@for dir in $(SRC_SUB_DIRS); \
	do $(MAKE) -C $$dir  clean || exit; \
	done
	
realclean:
	@for dir in $(SRC_SUB_DIRS); \
	do $(MAKE) -C $$dir  realclean || exit; \
	done 

help:
	@echo "Other targets are"
	@echo "  help          show this help message"
	@echo "  all           builds project - DEFAULT"
	@echo "  library       creates library"
	@echo "  tests-all     builds tests"
	@echo "  samples-all   builds Samples"
	@echo "  clean         removes objects and binary"
	@echo "  realclean     remove all generated files" 


################################################################
# 	THE END
################################################################