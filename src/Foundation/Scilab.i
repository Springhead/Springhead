%module Scilab
%include "../../include/Scilab/machine.h"
%include "../../include/Scilab/interf/stack1.h"
%include "../../include/Scilab/interf/stack2.h"
%include "../../include/Scilab/interf/stack3.h"
extern int TerminateScilab(char *ScilabQuit);
extern int SendScilabJob(char *job);
