%module Scilab
//%include "../../include/Scilab/core/includes/stack-c.h"
//%include "../../include/Scilab/call_scilab/includes/call_scilab.h"
%include "../../include/Scilab/call_scilab/includes/dynlib_call_scilab.h"
%include "../../include/Scilab/api_scilab/includes/api_scilab.h"
#define __INTERNAL_API_SCILAB__
//%include "../../include/Scilab/api_scilab/includes/api_common.h"
%include "../../include/Scilab/api_scilab/includes/api_double.h"
%include "../../include/Scilab/api_scilab/includes/api_string.h"
%include "../../include/Scilab/api_scilab/includes/api_int.h"
%include "../../include/Scilab/api_scilab/includes/api_poly.h"
%include "../../include/Scilab/api_scilab/includes/api_sparse.h"
%include "../../include/Scilab/api_scilab/includes/api_boolean.h"
%include "../../include/Scilab/api_scilab/includes/api_boolean_sparse.h"
%include "../../include/Scilab/api_scilab/includes/api_pointer.h"
%include "../../include/Scilab/api_scilab/includes/api_list.h"
%include "../../include/Scilab/api_scilab/includes/api_error.h"
%include "../../include/Scilab/api_scilab/includes/dynlib_api_scilab.h"
BOOL TerminateScilab(char *ScilabQuit);
int SendScilabJob(char *job);

