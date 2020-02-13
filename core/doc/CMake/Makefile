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
PDFNAME	= HowToUseCMake.pdf
INSPATH	= ../../../generated/doc

TEXFILE = $(PDFNAME:.pdf=.tex)
PDFFILE	= $(TOPFILE:.tex=.pdf)
INSFILE	= $(INSPATH)/$(PDFNAME)

SRCS	=\
	main.tex		\
	macro.tex		\
	1.0.Install		\
	1.1.Download		\
	1.2.Preparation		\
	1.3.Cmake		\
	1.4.Build		\
	2.0.Application		\
	2.1.PreparationForApp	\
	2.2.QandA		\
	2.3.0.Background	\
	2.3.1.OldMethod		\
	2.3.2.CmakeMethod	\
	2.3.3.Problems		\
	2.3.4.Solutions		\
	2.3.5.Implementation	\
	LastUpdate.tex

TIMESTAMPFILE	= timestamp.tex
CLEANS	= *.pdf *.dvi *.log *.aux *.toc *.ilg *.bak *~ $(TIMESTAMPFILE)

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
