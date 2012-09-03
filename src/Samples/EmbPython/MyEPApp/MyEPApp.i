%module MyEPApp

%ignore MyEPApp::MyEPApp;
%ignore MyEPApp::Init;
%ignore MyEPApp::hiSdk;
%ignore MyEPApp::spaceNavigator0;
%ignore MyEPApp::spaceNavigator1;
%ignore MyEPApp::argv;
%ignore MyEPApp::argc;

class SampleApp {};

%include "MyEPApp.h"
