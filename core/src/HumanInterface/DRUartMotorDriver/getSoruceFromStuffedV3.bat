mkdir PIC
mkdir WROOM
mkdir WROOM\main
mkdir WROOM\main\softRobot
mkdir PCRobokey
copy WROOM\main\softRobot\esp_log.h WROOM\main
del /y Robokey\*
del /y WROOM\main\softRobot\*
del /y PIC\*
move WROOM\main\esp_log.h WROOM\main\softRobot
ln ..\..\..\..\..\\StuffedV3\PCRobokey\commandId.cs PCRobokey\commandId.cs
ln ..\..\..\..\..\\StuffedV3\PIC\commandCommon.h PIC\commandCommon.h
ln ..\..\..\..\..\\StuffedV3\PIC\commandTemplate.h PIC\commandTemplate.h
ln ..\..\..\..\..\\StuffedV3\PIC\control.h PIC\control.h
ln ..\..\..\..\..\\StuffedV3\PIC\env.h PIC\env.h
ln ..\..\..\..\..\\StuffedV3\PIC\fixed.h PIC\fixed.h
ln ..\..\..\..\..\\StuffedV3\WROOM\main\softRobot\CommandWROOM.h WROOM\main\softRobot\CommandWROOM.h
ln ..\..\..\..\..\\StuffedV3\WROOM\main\softRobot\Board.h WROOM\main\softRobot\Board.h
ln ..\..\..\..\..\\StuffedV3\WROOM\main\softRobot\BoardBase.h WROOM\main\softRobot\BoardBase.h
ln ..\..\..\..\..\\StuffedV3\WROOM\main\softRobot\Board.cpp WROOM\main\softRobot\Board.cpp

