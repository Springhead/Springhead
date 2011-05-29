setlocal enabledelayedexpansion
rem Create .i files PART
rem call .\EmbPythonSwig.bat Base
call .\EmbPythonSwig.bat Foundation
call .\EmbPythonSwig.bat Collision Foundation
call .\EmbPythonSwig.bat Physics Collision Foundation
call .\EmbPythonSwig.bat Graphics Physics Foundation
call .\EmbPythonSwig.bat Creature Graphics Physics Foundation
call .\EmbPythonSwig.bat HumanInterface Foundation
call .\EmbPythonSwig.bat Framework HumanInterface Graphics Physics Foundation

exit 0
