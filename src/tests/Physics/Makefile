################################################################
#
# 	Makefile for Physics test
#
################################################################

################################################################
#	A rule to make sub directories.
################################################################

TOP_DIR 		= ../../..

PHYSICS_TEST_DIR		= PHConstraintTest \
					PHShape \
					PHShapeGL \
					PHSimple \
					PHSimpleGL

all:
	@for dir in $(PHYSICS_TEST_DIR); \
	do echo "---------------------------------------------"; \
	   echo "             make $$dir    			      "; \
	   echo "---------------------------------------------"; \
	$(MAKE) -C $$dir all || exit; \
	done


################################################################
#	Default rules
################################################################

clean:
	@for dir in $(PHYSICS_TEST_DIR); \
	do $(MAKE) -C $$dir  clean || exit; \
	done
	
realclean:
	@for dir in $(PHYSICS_TEST_DIR); \
	do $(MAKE) -C $$dir  realclean || exit; \
	done 


################################################################
# 	THE END
################################################################
