################################################################
#
# 	Makefile for Graphics test
#
################################################################

################################################################
#	A rule to make sub directories.
################################################################

TOP_DIR 		= ../../..

GRAPHICS_TEST_DIR	= \
						GRSimple \
						GRColorSpheres \
						#GRVertrexBlending

all:
	@for dir in $(GRAPHICS_TEST_DIR); \
	do echo "---------------------------------------------"; \
	   echo "             make $$dir    			      "; \
	   echo "---------------------------------------------"; \
	$(MAKE) -C $$dir all || exit; \
	done


################################################################
#	Default rules
################################################################

clean:
	@for dir in $(GRAPHICS_TEST_DIR); \
	do $(MAKE) -C $$dir  clean || exit; \
	done
	
realclean:
	@for dir in $(GRAPHICS_TEST_DIR); \
	do $(MAKE) -C $$dir  realclean || exit; \
	done 


################################################################
# 	THE END
################################################################
