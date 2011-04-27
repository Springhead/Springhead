setlocal enabledelayedexpansion
rem Create .i files PART
rem call .\SwigTest.bat Base
call .\SwigTest.bat Foundation
call .\SwigTest.bat Collision Foundation
call .\SwigTest.bat Graphics Physics Foundation
call .\SwigTest.bat Physics Collision Foundation
call .\SwigTest.bat Creature Graphics Physics Foundation


exit 0
