cd ..

cd Foundation
set PATH=..\..\bin\swig
swig -sprcli -w305,312,319,325,401,402 -DSWIG_Foundation -c++ Foundation.i
move FoundationCLI.cpp ..\SprNET\FoundationNET.cpp
cd ..

cd Physics
set PATH=..\..\bin\swig
swig -sprcli -w305,312,319,325,401,402 -DSWIG_Physics -c++ Physics.i
move PhysicsCLI.cpp ..\SprNET\PhysicsNET.cpp
cd ..

pause
