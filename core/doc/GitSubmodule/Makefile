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
PDFNAME	= GitSubmodule.pdf
INSPATH	= ../../../generated/doc

TEXFILE = $(PDFNAME:.pdf=.tex)
PDFFILE	= $(TOPFILE:.tex=.pdf)
INSFILE	= $(INSPATH)/$(PDFNAME)

SRCS	=\
	main.tex			\
	macro.tex			\
	LastUpdate.tex

#	1.WhatIsDailyBuild.tex		\
#	2.RoutineWork.tex		\
#	3.WikiMaintenance.tex		\
#	4.VisualStudioVersion.tex	\
#	5.Task.tex			\
#	6.0.Script.tex			\
#	6.1.0.TestMain.tex		\
#	6.1.1.TestAll.tex		\
#	6.1.2.BuidVC.tex		\
#	6.2.0.MakeReport.tex		\
#	6.2.1.BuildMonitor.tex		\
#	6.2.2.BaseLib.tex		\
#	6.2.3.DailyBuildLib.tex		\

TIMESTAMPFILE	= timestamp.tex
CLEANS	= *.pdf *.dvi *.log *.aux *.toc *.ilg $(TIMESTAMPFILE)

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
