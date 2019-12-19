# CMakeLists.txt for Base

# target
set(Target "Base")
message("** ${Target} **")

# sytem environment
set(Windows "${CMAKE_HOST_SYSTEM_NAME}" STREQUAL "Windows")
set(Linux   "${CMAKE_HOST_SYSTEM_NAME}" STREQUAL "Linux")

# find Springhead top directory
if("${TOPDIR}" STREQUAL "")
    set(TOPDIR "${CMAKE_SOURCE_DIR}/../..")
endif()
set(SPR_TOP_DIR  "${TOPDIR}")
set(SPR_SRC_DIR  "${TOPDIR}/core/src")
set(SPR_INC_DIR  "${TOPDIR}/core/include")
get_filename_component(TOPDIR ${TOPDIR} ABSOLUTE)
get_filename_component(SPR_TOP_DIR ${SPR_TOP_DIR} ABSOLUTE)
get_filename_component(SPR_SRC_DIR ${SPR_SRC_DIR} ABSOLUTE)
get_filename_component(SPR_INC_DIR ${SPR_INC_DIR} ABSOLUTE)
#message(STATUS "${Target}: SPR_TOP_DIR: [${SPR_TOP_DIR}]")
#message(STATUS "${Target}: SPR_SRC_DIR: [${SPR_SRC_DIR}]")
#message(STATUS "${Target}: SPR_INC_DIR: [${SPR_INC_DIR}]")

#===============================================================
#  Define sources and headers to include this project.
#	If you specify headers under core/include directory,
#	you must add directory path explicitly. The variable
#	reference ${SPR_INC_DIR} indicates ".../core/include".
#
#	If GLOB collects unnessessary files, replace "" of the
#	last element of EXCLUDES_SRCS by those file names.
#---------------------------------------------------------------
file(GLOB SRCS *.cpp *.h)
file(GLOB COMMON_HDRS ${SPR_INC_DIR}/${Target}/*.h)
file(GLOB EXCLUDE_SRCS "")
#  end of definition
#===============================================================
list(APPEND SRCS ${COMMON_HDRS})
list(LENGTH EXCLUDE_SRCS LEN_EXCLUDE_SRCS)
if(${LEN_EXCLUDE_SRCS} GREATER 0)
	list(REMOVE_ITEM SRCS ${EXCLUDE_SRCS})
endif()

# modify compile/link options if neccessary
#
set(_override_file "${SPR_TOP_DIR}/core/make.modify_opts.cmake")
include(${_override_file})
if (${Windows})
else()
    set(LINUX_COPT      ${LINUX_COPT_CONFIG}
			${LINUX_COPT_FLAGS}
			${LINUX_COPT_MACROS}
			${LINUX_COPT_WARNING_FLAGS})
    set(LINUX_INCDIRS   ${LINUX_COPT_INCDIR})
    set(LINUX_LDFLAGS   #${LINUX_LDFLAGS_PREPEND}
			${LINUX_SPRLIB}
			${LINUX_STDLIB}
			${LINUX_LDFLAGS})
			#${LINUX_LDFLAGS_APPEND})
    list(JOIN LINUX_COPT " " CMAKE_CXX_FLAGS)
    add_definitions(${LINUX_COPT_MACROS_APPEND})
endif()

# create library
#
if(${Windows})
    add_library(${Target} ${LIBTYPE} ${SRCS})
    set_target_properties("${Target}" PROPERTIES
	OUTPUT_NAME_DEBUG   "${Target}${VS_VERSION}D${CMAKE_GENERATOR_PLATFORM}"
	OUTPUT_NAME_RELEASE "${Target}${VS_VERSION}${CMAKE_GENERATOR_PLATFORM}"
	OUTPUT_NAME_TRACE   "${Target}${VS_VERSION}T${CMAKE_GENERATOR_PLATFORM}"
	ARCHIVE_OUTPUT_DIRECTORY_DEBUG   ${SPR_SRC_DIR}/${Target}
	ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${SPR_SRC_DIR}/${Target}
	ARCHIVE_OUTPUT_DIRECTORY_TRACE   ${SPR_SRC_DIR}/${Target}
    )
else()
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${SPR_SRC_DIR}/${Target})
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${SPR_SRC_DIR}/${Target})
    add_library(${Target} ${LIBTYPE} ${SRCS})
endif()

# include directories
#
foreach(_dir ${LINUX_INCDIRS})
    string(REPLACE "-I" "" _tmp ${_dir})
    target_include_directories(${Target}
	PRIVATE ${_tmp}
    )
endforeach()

if(${Windows})
    target_include_directories(${Target}
	PRIVATE .
	PRIVATE ${SPR_INC_DIR}
	PRIVATE ${SPR_SRC_DIR}
	PRIVATE ${SPR_TOP_DIR}/dependency/src
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
set(ACTDIR ${SPR_SRC_DIR}/${Target}/${ARCHITECTURE})
if(${Windows})
    set(ACTDIR ${ACTDIR}/${VS_VERSION})
    set(LINKDIR ${INTDIR})
elseif(${Linux})
    set(LINKDIR .${CMAKE_FILES_DIRECTORY}/${INTDIR})
endif()
set(LINKLIST
    "${LINKDIR}=${ACTDIR}/${INTDIR}"
)
set(_script_dir ${SPR_SRC_DIR})
if(${Windows})
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
