# CMakeLists.txt for RunSwig

# target
set(Target "RunSwig")
message("** ${Target} **")

# original directory
set(OrgDir ${PROJECT_SOURCE_DIR})
set(RSOrgDir ${OrgDir}/RunSwig)

# build directory
set(BldDir ${PROJECT_BINARY_DIR})
set(RSBldDir ${BldDir}/RunSwig)

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
file(TO_NATIVE_PATH "${OrgDir}" OrgDir)
file(TO_NATIVE_PATH "${RSOrgDir}" RSOrgDir)
file(TO_NATIVE_PATH "${BldDir}" BldDir)
file(TO_NATIVE_PATH "${RSBldDir}" RSBldDir)

# Also rewrite .i file because it has relative path hardcoded!
#
set(PattI ../../include=../../../include)
set(PattF ../Foundation=../../Foundation)

# main target
#
if(${Windows})
    add_custom_target(RunSwig
	COMMAND copy ${RSOrgDir}\\PrepforCMake.py ${RSBldDir} >NUL 2>&1
	COMMAND ${Python} PrepForCMake.py ${OrgDir} ${BldDir}
	COMMAND ${Python} make_manager.py -A -c
	COMMAND ${Make} -f ${Makefile}
	COMMENT [[  generating files...]]
    )
elseif(${Linux})
    add_custom_target(RunSwig
	COMMAND cp -f ${RSOrgDir}/PrepforCMake.py ${RSBldDir} >/dev/null 2>&1
	COMMAND ${Python} PrepForCMake.py ${OrgDir} ${BldDir}
	COMMAND ${Python} make_manager.py -A -c
	COMMAND ${Make} -f ${Makefile}
	COMMENT [[  generating files...]]
    )
endif()

# end: CMakeLists.txt
