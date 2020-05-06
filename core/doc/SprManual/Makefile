######################################################################
#  Makefile for processing tex source and convert dvi to pdf format.
#
#  Usage:
#    Windows:	nmake DATE=yyyy-mmdd TIME=hhmm
#    unix:	make
#
#	Utilizes the property that make recognises line continuation
#	on comment line bat nmake does not.
######################################################################

TOPFILE	= main.tex
PDFNAME	= Springhead.pdf
INSPATH	= ../../../generated/doc

TEXFILE = $(PDFNAME:.pdf=.tex)
PDFFILE	= $(TOPFILE:.tex=.pdf)
INSFILE	= $(INSPATH)/$(PDFNAME)

SRCS	=\
	main.tex		\
	chapters.tex		\
	intro.tex		\
	getstarted.tex		\
	structure.tex		\
	base.tex		\
	foundation.tex		\
	collision.tex		\
	physics.tex		\
	graphics.tex		\
	fileio.tex		\
	humaninterface.tex	\
	creature.tex		\
	framework.tex		\
	embpython.tex		\
	unity.tex		\
	trouble.tex

TIMESTAMPFILE	= timestamp.tex
CLEANS	= *.pdf *.dvi *.log *.aux *.toc *.ilg *.idx *.ind *.out $(TIMESTAMPFILE)

PLATEX	= platex
MENDEF	= mendex
DVIPDF	= dvipdfmx

#\
!ifndef 0 #\
# for nmake #\
INSPATH	= $(INSPATH:/=\) #\
INSFILE	= $(INSFILE:/=\) #\
CP	= copy #\
RM	= del /Q /F #\
NULL	= NUL #\
!else
# for make #\
DATE	= `date '+%Y-%m%d'`
TIME	= `date '+%H%M'`
CP	= cp
RM	= rm -f
NULL	= /dev/null
#\
!endif
TIMESTAMP	= $(DATE)_$(TIME)

all:	install

tex:	$(PDFFILE)

$(PDFFILE):	$(SRCS)
	-echo \newcommand{\timestamp}{$(DATE)--$(TIME)} > $(TIMESTAMPFILE)
	-$(PLATEX) $(TOPFILE:.tex=)
	-$(PLATEX) $(TOPFILE:.tex=)
	-$(MENDEF) $(TOPFILE:.tex=.idx)
	-$(PLATEX) $(TOPFILE:.tex=)
	-$(DVIPDF) $(TOPFILE:.tex=)
	$(CP) $(TOPFILE:.tex=.pdf) $(TOPFILE:.tex=)_$(TIMESTAMP).pdf >$(NULL)

$(SRCS):

install:	$(INSFILE)

$(INSFILE):	$(PDFFILE)
	-mkdir $(INSPATH) >$(NULL) 2>&1
	$(CP) $(TOPFILE:.tex=.pdf) $(INSFILE) >$(NULL)

clean:
	$(RM) $(CLEANS)

# end
