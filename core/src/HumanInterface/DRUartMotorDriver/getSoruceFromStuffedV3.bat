mkdir PIC
mkdir WROOM
mkdir WROOM\main
mkdir WROOM\main\softRobot
mkdir PCRobokey
copy WROOM\main\softRobot\esp_log.h WROOM\main
del /Q Robokey\*
del /Q WROOM\main\softRobot\*
del /Q PIC\*
move WROOM\main\esp_log.h WROOM\main\softRobot
ln ..\..\..\..\..\Nuibot\StuffedV3\PCRobokey\commandId.cs PCRobokey\commandId.cs
ln ..\..\..\..\..\Nuibot\StuffedV3\PIC\commandCommon.h PIC\commandCommon.h
ln ..\..\..\..\..\Nuibot\StuffedV3\PIC\commandTemplate.h PIC\commandTemplate.h
ln ..\..\..\..\..\Nuibot\StuffedV3\PIC\control.h PIC\control.h
ln ..\..\..\..\..\Nuibot\StuffedV3\PIC\env.h PIC\env.h
ln ..\..\..\..\..\Nuibot\StuffedV3\PIC\fixed.h PIC\fixed.h
ln ..\..\..\..\..\Nuibot\StuffedV3\WROOM\components\softRobot\CommandWROOM.h WROOM\main\softRobot\CommandWROOM.h
ln ..\..\..\..\..\Nuibot\StuffedV3\WROOM\components\softRobot\Board.h WROOM\main\softRobot\Board.h
ln ..\..\..\..\..\Nuibot\StuffedV3\WROOM\components\softRobot\BoardBase.h WROOM\main\softRobot\BoardBase.h
ln ..\..\..\..\..\Nuibot\StuffedV3\WROOM\components\softRobot\Board.cpp WROOM\main\softRobot\Board.cpp
