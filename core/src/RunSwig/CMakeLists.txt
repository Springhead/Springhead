# CMakeLists.txt for RunSwig

# target
set(Target "RunSwig")
message("** ${Target} **")

# original directory
set(RSOrgDir ${PROJECT_SOURCE_DIR}/RunSwig)
set(FOOrgDir ${PROJECT_SOURCE_DIR}/Foundation)
set(FWOrgDir ${PROJECT_SOURCE_DIR}/Framework)

# build directory
set(RSBldDir ${PROJECT_SOURCE_DIR}/build/RunSwig)
set(FOBldDir ${PROJECT_SOURCE_DIR}/build/Foundation)
set(FWBldDir ${PROJECT_SOURCE_DIR}/build/Framework)

if(${Windows})
    set(Python python)
    set(Make nmake)
    set(Makefile Makefile.win)
elseif(${Linux})
    set(Python python_adapter.bat)
    set(Make make)
    set(Makefile Makefile.unix)
endif()

# Script make_manager.py will be run at src/build/RunSwig and finds paths
# needed for invoking child scripts (these paths are on original file tree).
# So we copy scripts from src/build/RunSwig and related ones first.
#
file(TO_NATIVE_PATH "${RSOrgDir}" RSOrgDir)
file(TO_NATIVE_PATH "${FOOrgDir}" FOOrgDir)
file(TO_NATIVE_PATH "${FWOrgDir}" FWOrgDir)
file(TO_NATIVE_PATH "${RSBldDir}" RSBldDir)
file(TO_NATIVE_PATH "${FOBldDir}" FOBldDir)
file(TO_NATIVE_PATH "${FWBldDir}" FWBldDir)

# Also rewrite .i file because it has relative path hardcoded!
#
set(PattI ../../include=../../../include)
set(PattF ../Foundation=../../Foundation)

# main target
#
if(${Windows})
    add_custom_target(RunSwig
	COMMAND copy ${RSOrgDir}\\FindSprPath.py ${RSBldDir} >NUL 2>&1
	COMMAND copy ${RSOrgDir}\\PrepforCMake.py ${RSBldDir} >NUL 2>&1
	COMMAND ${Python} PrepForCMake.py
	COMMAND ${Python} make_manager.py -A -c
	COMMAND ${Make} -f ${Makefile}
	COMMENT [[  generating files...]]
    )
elseif(${Linux})
    add_custom_target(RunSwig
	COMMAND cp -f ${RSOrgDir}/FindSprPath.py ${RSBldDir} >/dev/null 2>&1
	COMMAND cp -f ${RSOrgDir}/PrepforCMake.py ${RSBldDir} >/dev/null 2>&1
	COMMAND ${Python} PrepForCMake.py
	COMMAND ${Python} make_manager.py -A -c
	COMMAND ${Make} -f ${Makefile}
	COMMENT [[  generating files...]]
    )
endif()

# end: CMakeLists.txt
