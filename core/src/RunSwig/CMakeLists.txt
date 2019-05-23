# CMakeLists.txt for RunSwig
#	CMake variable "SPRINGHEAD_TOPDIR" must be set by the caller.

# target
set(Target "RunSwig")
message("** ${Target} **")

# sytem environment
set(Windows "${CMAKE_HOST_SYSTEM_NAME}" STREQUAL "Windows")
set(Linux   "${CMAKE_HOST_SYSTEM_NAME}" STREQUAL "Linux")

# Springhead top directory and build directory
if("${TOPDIR}" STREQUAL "")
    set(TOPDIR "${CMAKE_SOURCE_DIR}/../..")
endif()
set(SPR_TOP_DIR  "${TOPDIR}")
set(SPR_SRC_DIR  "${TOPDIR}/core/src")
set(SPR_INC_DIR  "${TOPDIR}/core/include")
get_filename_component(TOPDIR ${TOPDIR} ABSOLUTE)
get_filename_component(SPR_TOP_DIR ${SPR_TOP_DIR} ABSOLUTE)
get_filename_component(SPR_SRC_DIR ${SPR_SRC_DIR} ABSOLUTE)
get_filename_component(PRJ_BLD_DIR ${PROJECT_BINARY_DIR} ABSOLUTE)
file(TO_NATIVE_PATH "${TOPDIR}" TOPDIR)
file(TO_NATIVE_PATH "${SPR_TOP_DIR}" SPR_TOP_DIR)
file(TO_NATIVE_PATH "${SPR_SRC_DIR}" SPR_SRC_DIR)
file(TO_NATIVE_PATH "${PRJ_BLD_DIR}" PRJ_BLD_DIR)
#message(STATUS "${Target}: SPR_TOP_DIR: [${SPR_TOP_DIR}]")
#message(STATUS "${Target}: SPR_SRC_DIR: [${SPR_SRC_DIR}]")
#message(STATUS "${Target}: PRJ_BLD_DIR: [${PRJ_BLD_DIR}]")

# RunSwig directory
set(RSSrcDir ${SPR_SRC_DIR}/RunSwig)
set(RSBldDir ${PRJ_BLD_DIR}/RunSwig)
file(TO_NATIVE_PATH "${RSSrcDir}" RSSrcDir)
file(TO_NATIVE_PATH "${RSBldDir}" RSBldDir)
#message(STATUS "${Target}: RSSrcDir: [${RSSrcDir}]")
#message(STATUS "${Target}: RSBldDir: [${RSBldDir}]")

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
add_custom_target(${Target}
    COMMAND cd ${RSSrcDir}
    COMMAND ${Python} make_manager.py -A -c
    COMMAND ${Make} -f ${Makefile} SPRSRCDIR=${SPR_SRC_DIR}
    COMMENT [[  generating files...]]
)

# for Windows (Visual Studio)
#   make $(IntDir) be linked to
#	${SPR_SRC_DIR}/${Target}/${ARCHITECTURE}/${VS_VERSION}/${Target}.dir
#
if(${Windows})
    set(INTDIR ${Target}.dir)
    set(CMFDIR CMakeFiles)
    set(LOGDIR ${ARCHITECTURE})		# created by add_custom_target
    set(ACTDIR ${SPR_SRC_DIR}/${Target}/${ARCHITECTURE}/${VS_VERSION})
    set(LINKLIST
	"${INTDIR}=${ACTDIR}/${INTDIR}" ;
	"${CMFDIR}=${ACTDIR}/${CMFDIR}" ;
	"${LOGDIR}=${ACTDIR}/${INTDIR}" ;
    )
    set(cmnd "python ${SPR_SRC_DIR}/cmake_ln.py -v ${LINKLIST}")
    execute_process(
	COMMAND cmd /c ${cmnd}
	RESULT_VARIABLE CMD_RESULT
	OUTPUT_VARIABLE CMD_OUTPUT
	ERROR_VARIABLE CMD_ERROR
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )
    if(${CMD_RESULT})
        message("Error: ${CMD_ERROR}")
    endif()
    string(REPLACE "\n" "\n   " CMD_OUTPUT ${CMD_OUTPUT})
    #message("   ${CMD_OUTPUT}")
    unset(INTDIR)
    unset(CMFDIR)
    unset(LOGDIR)
    unset(ACTDIR)
    unset(cmnd)
endif()

# end: CMakeLists.txt
