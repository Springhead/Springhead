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

file(TO_NATIVE_PATH "${OrgDir}" OrgDir)
file(TO_NATIVE_PATH "${RSOrgDir}" RSOrgDir)
file(TO_NATIVE_PATH "${BldDir}" BldDir)
file(TO_NATIVE_PATH "${RSBldDir}" RSBldDir)

if(${Windows})
    set(Python python_adapter.bat)
    set(Make nmake)
    set(Makefile Makefile.win)
elseif(${Linux})
    set(Python python)
    set(Make make)
    set(Makefile Makefile.unix)
endif()

# main target
#
if(${Windows})
    add_custom_target(${Target}
	COMMAND copy ${RSOrgDir}\\python_adapter.bat ${RSBldDir} >NUL 2>&1
	COMMAND copy ${RSOrgDir}\\PrepforCMake.py ${RSBldDir} >NUL 2>&1
	COMMAND ${Python} PrepForCMake.py ${OrgDir} ${BldDir}
	COMMAND ${Python} make_manager.py -A -c
	COMMAND ${Make} -f ${Makefile}
	COMMENT [[  generating files...]]
    )
elseif(${Linux})
    add_custom_target(${Target}
	COMMAND cp -f ${RSOrgDir}/PrepforCMake.py ${RSBldDir} >/dev/null 2>&1
	COMMAND ${Python} PrepForCMake.py ${OrgDir} ${BldDir}
	COMMAND ${Python} make_manager.py -A -c
	COMMAND ${Make} -f ${Makefile}
	COMMENT [[  generating files...]]
    )
endif()

# end: CMakeLists.txt
