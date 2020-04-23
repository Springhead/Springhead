# CMakeLists.txt for RunSwig
#	CMake variable "SPRINGHEAD_TOPDIR" must be set by the caller.

# target
set(Target "RunSwig")
message("** ${Target} **")

# sytem environment
set(Windows "${CMAKE_HOST_SYSTEM_NAME}" STREQUAL "Windows")
set(Linux   "${CMAKE_HOST_SYSTEM_NAME}" STREQUAL "Linux")
set(unix    "${CMAKE_HOST_SYSTEM_NAME}" STREQUAL "Linux")
set(FreeBSD "${CMAKE_HOST_SYSTEM_NAME}" STREQUAL "FreeBSD")
set(unix    "${CMAKE_HOST_SYSTEM_NAME}" STREQUAL "FreeBSD")

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

if(Windows)
    set(Make nmake)
    set(Makefile Makefile.win)
elseif(unix)
    set(Make make)
    set(Makefile Makefile.unix)
endif()

# RunSwig generate files
#	These files are removed when 'clean' target executed
#
set(_for_all @.i @Decl.hpp @Stub.cpp @Stub.mak.txt do_swigone.projs 
	     makefile.swig Spr@Decl.hpp swig_spr.log)
set(_for_FW  FWOldSpringheadDecl.hpp FWOldSpringheadStub.cpp)

set(generated_files)
foreach(_dir ${SPR_PROJS})
    foreach(_file ${_for_all})
	set(_tmp ${SPR_SRC_DIR}/${_dir}/${_file})
	string(REPLACE @ ${_dir} _tmp ${_tmp})
	list(APPEND generated_files ${_tmp})
    endforeach()
endforeach()
set(_dir Framework)
foreach(_file ${_for_FW})
    set(_tmp ${SPR_SRC_DIR}/${_dir}/${_file})
    list(APPEND generated_files ${_tmp})
endforeach()

foreach(_file ${generated_files})
    file(TO_NATIVE_PATH "${_file}" _file)
    #message("     generate: ${_file}")
endforeach()

# main target
#
if(Windows)
    add_custom_target(${Target}
	BYPRODUCTS ${genereated_files}
	DEPENDS ${generated_files}
    )
    set(_tmp_cmnd cmd /c ${SPR_SRC_DIR}\\do_python.bat)
    set(_tmp_args ${RSSrcDir}\\make_manager.py -A -c)
    add_custom_command(
	OUTPUT ${generated_files}
	COMMAND set _SPRTOP_=${SPR_TOP_DIR} & ${_tmp_cmnd} ${_tmp_args}
	COMMAND ${Make} -f ${RSSrcDir}\\${Makefile}
	WORKING_DIRECTORY ${RSSrcDir}
	COMMENT [[  generating files...]]
    )
elseif(unix)
    set(_tmp_cmnd python)
    set(_tmp_args ${RSSrcDir}/make_manager.py -A -c)
    add_custom_target(${Target}
	COMMAND _SPRTOP_=${SPR_TOP_DIR} ${_tmp_cmnd} ${_tmp_args}
	COMMAND ${Make} -f ${RSSrcDir}/${Makefile}
	BYPRODUCTS ${generated_files}
	WORKING_DIRECTORY ${RSSrcDir}
	COMMENT [[  generating files...]]
    )
endif()

# integrate intermediate directory
#	${CMAKE_BINARY_DIR}/${Target}/${Target}.dir
#	linked to (for Windows)
#	  => ${SPR_SRC_DIR}/${Target}/${ARCHITECTURE}/${VS_VERSION}/${Target}.dir
#	or (for unix)
#	  => ${SPR_SRC_DIR}/${Target}/${ARCHITECTURE}/${Target}.dir
#
set(INTDIR ${Target}.dir)
set(LOGDIR ${ARCHITECTURE})		# created by add_custom_target
set(ACTDIR ${SPR_SRC_DIR}/${Target}/${ARCHITECTURE})
if(Windows)
    set(ACTDIR ${ACTDIR}/${VS_VERSION})
    set(LINKDIR ${INTDIR})
elseif(unix)
    set(LINKDIR .${CMAKE_FILES_DIRECTORY}/${INTDIR})
    set(LOGDIR  .${CMAKE_FILES_DIRECTORY}/${INTDIR})
endif()
set(LINKLIST
    "${LINKDIR}=${ACTDIR}/${INTDIR}"
    "${LOGDIR}=${ACTDIR}/${INTDIR}" ;
)
set(_script_dir ${SPR_SRC_DIR})
if(Windows)
    set(_tmp_cmnd cmd /c ${SPR_SRC_DIR}\\do_python.bat)
    set(_tmp_args ${_script_dir}\\cmake_preconfig.py -v ${LINKLIST})
else()
    set(_tmp_cmnd python)
    set(_tmp_args ${_script_dir}/cmake_preconfig.py -v ${LINKLIST})
endif()
set(ENV{_SPRTOP_} ${SPR_TOP_DIR})
execute_process(
    COMMAND ${_tmp_cmnd} ${_tmp_args}
    RESULT_VARIABLE CMD_RESULT
    OUTPUT_VARIABLE CMD_OUTPUT
    ERROR_VARIABLE CMD_ERROR
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
)
if(${CMD_RESULT})
    message("Error: ${CMD_ERROR}")
endif()
if(NOT "${CMD_OUTPUT}" STREQUAL "")
    string(REPLACE "\n" "\n     " CMD_OUTPUT ${CMD_OUTPUT})
endif()
string(STRIP ${CMD_OUTPUT} CMD_OUTPUT)
message("     ${CMD_OUTPUT}")

# end: CMakeLists.txt
