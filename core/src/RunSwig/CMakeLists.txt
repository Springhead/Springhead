# CMakeLists.txt for RunSwig

# target
set(Target "RunSwig")
message("** ${Target} **")

# sytem environment
if("$ENV{SPR_TOP_DIR}" STREQUAL "")
    # assume current directory is ".../Springhead/core/src"
    set(SPR_TOP_DIR ${CMAKE_SOURCE_DIR}/../..)
else()
    set(SPR_TOP_DIR $ENV{SPR_TOP_DIR})
endif()
get_filename_component(SPR_TOP_DIR ${SPR_TOP_DIR} ABSOLUTE)
get_filename_component(SPR_BLD_DIR ${PROJECT_BINARY_DIR} ABSOLUTE)
set(SPR_SRC_DIR ${SPR_TOP_DIR}/core/src)
#
file(TO_NATIVE_PATH "${SPR_TOP_DIR}" SPR_TOP_DIR)
file(TO_NATIVE_PATH "${SPR_SRC_DIR}" SPR_SRC_DIR)
file(TO_NATIVE_PATH "${SPR_BLD_DIR}" SPR_BLD_DIR)
#message(STATUS  ${Target}:SPR_TOP_DIR: [${SPR_TOP_DIR}])
#message(STATUS  ${Target}:SPR_SRC_DIR: [${SPR_SRC_DIR}])
#message(STATUS  ${Target}:SPR_BLD_DIR: [${SPR_BLD_DIR}])

# RunSwig directory
set(RSSrcDir ${SPR_SRC_DIR}/RunSwig)
set(RSBldDir ${SPR_BLD_DIR}/RunSwig)
file(TO_NATIVE_PATH "${RSSrcDir}" RSSrcDir)
file(TO_NATIVE_PATH "${RSBldDir}" RSBldDir)
#message(STATUS  ${Target}:RSSrcDir: [${RSSrcDir}])
#message(STATUS  ${Target}:RSBldDir: [${RSBldDir}])

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
	COMMAND copy ${RSSrcDir}\\python_adapter.bat ${RSBldDir} >NUL 2>&1
	COMMAND copy ${RSSrcDir}\\PrepforCMake.py ${RSBldDir} >NUL 2>&1
	COMMAND ${Python} PrepForCMake.py ${SPR_SRC_DIR} ${SPR_BLD_DIR}
	COMMAND ${Python} make_manager.py -A -c
	COMMAND ${Make} -f ${Makefile}
	COMMENT [[  generating files...]]
    )
elseif(${Linux})
    add_custom_target(${Target}
	COMMAND cp -f ${RSOrgDir}/PrepforCMake.py ${RSBldDir} >/dev/null 2>&1
	COMMAND ${Python} PrepForCMake.py ${SPR_SRC_DIR} ${SPR_BLD_DIR}
	COMMAND ${Python} make_manager.py -A -c
	COMMAND ${Make} -f ${Makefile}
	COMMENT [[  generating files...]]
    )
endif()

# end: CMakeLists.txt
