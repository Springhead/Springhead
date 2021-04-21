# CMakeLists.txt for Springhead Application

cmake_minimum_required(VERSION 3.1)

# ------------------------------------------------------------------------------
#  Read project name.
#
if(EXISTS "${CMAKE_SOURCE_DIR}/CMakeSettings.txt")
    set(SETTING_FILE "CMakeSettings.txt")
else()
    set(SETTING_FILE "CMakeSettings.txt.Dev.dist")
endif()
include("${CMAKE_SOURCE_DIR}/${SETTING_FILE}")
message(STATUS "Host System: ${CMAKE_HOST_SYSTEM_NAME}")

# ------------------------------------------------------------------------------
#  Definition of project.
#
message(STATUS "Project: ${ProjectName}")
project(${ProjectName} CXX)
set(Target ${ProjectName})
set(CMAKE_SUPPRESS_REGENERATION true)
set(CMAKE_USE_RELATIVE_PATHS true)

# ------------------------------------------------------------------------------
#  Determine top directory of Springhead tree.
#	If cmake option "-D TOPDIR=<absolute-dir-path>" is specified,
#	<absolute-dir-path> is the one.
#	Otherwise, "${CMAKE_SOURCE_DIR}/CMakeTopdir.txt" SHOULD contain the
#	definition of top directory like;
#	    set(TOPDIR "<absolute-dir-path>")
#	For DailyBuild:
#	    Cmake option "-D TOPDIR_FILE=<absolute-file-path>" specifies
#	    the definition file instead of directory name itself.
#
if(DEFINED TOPDIR)
    # ok
elseif(DEFINED TOPDIR_FILE)
    if(NOT EXISTS ${TOPDIR_FILE})
	message("Error: File \"${TOPDIR_FILE}\" not found")
	return()
    endif()
    include(${TOPDIR_FILE})
else()
    set(TOPDIR_FILE "${CMAKE_SOURCE_DIR}/CMakeTopdir.txt")
    if(NOT EXISTS ${TOPDIR_FILE})
	message("Error: File \"${TOPDIR_FILE}\" not found")
	return()
    endif()
    include(${TOPDIR_FILE})
endif()
if(NOT EXISTS "${TOPDIR}")
    message("Error: Directory \"${TOPDIR}\" not found")
    return()
endif()

set(SPR_TOP_DIR  "${TOPDIR}")
set(SPR_CORE_DIR "${TOPDIR}/core")
set(SPR_SRC_DIR  "${TOPDIR}/core/src")
set(SPR_INC_DIR  "${TOPDIR}/core/include")
get_filename_component(TOPDIR ${TOPDIR} ABSOLUTE)
get_filename_component(SPR_TOP_DIR ${SPR_TOP_DIR} ABSOLUTE)
get_filename_component(SPR_SRC_DIR ${SPR_SRC_DIR} ABSOLUTE)
get_filename_component(SPR_INC_DIR ${SPR_INC_DIR} ABSOLUTE)
get_filename_component(SPR_BLD_DIR ${PROJECT_BINARY_DIR} ABSOLUTE)
message(STATUS "SPR_TOP_DIR: [${SPR_TOP_DIR}]")
#message(STATUS "SPR_SRC_DIR: [${SPR_SRC_DIR}]")
#message(STATUS "SPR_INC_DIR: [${SPR_INC_DIR}]")
message(STATUS "SPR_BLD_DIR: [${SPR_BLD_DIR}]")

# ------------------------------------------------------------------------------
#  Sytem environment.
#
include("${SPR_CORE_DIR}/make.system.cmake")
message(STATUS "Architecture: ${ARCHITECTURE}")
set(CMAKE_GENERATOR_PLATFORM ${ARCHITECTURE})
message(STATUS "Library type: ${SPRLIBTYPE}")

# ------------------------------------------------------------------------------
#  Read function definitions.
#
include("${SPR_CORE_DIR}/make.func.cmake")

# ------------------------------------------------------------------------------
#  Read compiler/linker option file (try to find in following order).
#	(1) File specified by option "-D OPTS=<file>".
#	(2) "${CMAKE_SOURCE_DIR}/CMakeOpts.txt".
#	(3) Default file: "${SPR_SRC_DIR}/CMakeOpts.txt.dist".
#
set(OPTS_FILE_NAME "CMakeOpts.txt")
set(OPTS_FILE "${CMAKE_CURRENT_SOURCE_DIR}/${OPTS_FILE_NAME}")
set(OPTS_DEFAULT_FILE "${SPR_SRC_DIR}/${OPTS_FILE_NAME}.dist")
include_file(OPTS ${OPTS_FILE} ${OPTS_DEFAULT_FILE})

# ------------------------------------------------------------------------------
#  Merge setting parameters.
#	Additional parameters are already read at the top of this file.
#	We need merge process here.
#
set(DEFINE_MACROS               ${DEFINE_MACROS} ${DEFINE_MACROS_ADD})
set(INCLUDE_PATHS               ${INCLUDE_PATHS_ADD} ${INCLUDE_PATHS})
set(COMP_FLAGS                  ${COMP_FLAGS_ADD} ${COMP_FLAGS})
set(LINK_FLAGS                  ${LINK_FLAGS_ADD} ${LINK_FLAGS})
set(LIBRARY_PATHS               ${LIBRARY_PATHS} ${LIBRARY_PATHS_ADD})

list(FIND SHARED_LIBRARY_NAMES_ADD EmbPython _found_shared)
list(FIND STATIC_LIBRARY_NAMES_ADD EmbPython _found_static)
set(_found 0)
if(${_found_shared} EQUAL 0 OR ${_found_static} EQUAL 0)
    set(_found 1)
endif()
if(${_found} EQUAL 0)
    message("++ EMbPython found in LIBRARY_NAMES_ADD")
endif()

if(Windows)
    if(${_found} EQUAL -1)
	set(LIBRARY_NAMES	${SPRLIB_NAMES}
				${LIBRARY_NAMES_ADD} ${STDLIB_NAMES})
    else()
 	list(REMOVE_ITEM LIBRARY_NAMES_ADD EmbPython)
 	set(LIBRARY_NAMES	EmbPython ${SPRLIB_NAMES}
				${LIBRARY_NAMES_ADD} ${STDLIB_NAMES})
    endif()
else()
    set(SHARED_LIBRARY_NAMES	${SHARED_LIBRARY_NAMES_ADD}
				${SHARED_STDLIB_NAMES})
    set(STATIC_LIBRARY_NAMES	${STATIC_LIBRARY_NAMES_ADD}
				${STATIC_STDLIB_NAMES})
    if("${SPRLIBTYPE}" STREQUAL "SHARED")
	if(${_found} EQUAL -1)
	    set(SHARED_LIBRARY_NAMES	${SPRLIB_NAMES}
					${SHARED_LIBRARY_NAMES})
	else()
	    list(REMOVE_ITEM LIBRARY_NAMES_ADD EmbPython)
	    set(SHARED_LIBRARY_NAMES	EmbPython
					${SPRLIB_NAMES}
					${SHARED_LIBRARY_NAMES})
	endif()
    else()
	if(${_found} EQUAL -1)
	    set(STATIC_LIBRARY_NAMES	${SPRLIB_NAMES}
					${SHARED_LIBRARY_NAMES}
					${STATIC_LIBRARY_NAMES})
	else()
	    if(${_found_shared} EQUAL 0)
		list(REMOVE_ITEM SHARED_LIBRARY_NAMES EmbPython)
	    endif()
	    set(SHARED_LIBRARY_NAMES	EmbPython
					${SPRLIB_NAMES}
					${SHARED_LIBRARY_NAMES})
	endif()
	#  Discard standard shared libraries.
	#set(SHARED_LIBRARY_NAMES	${EMPTY})
    endif()
#message("** SHARED_LIBRARY_NAMES: ${SHARED_LIBRARY_NAMES}")
#message("** STATIC_LIBRARY_NAMES: ${STATIC_LIBRARY_NAMES}")
endif()

# ------------------------------------------------------------------------------
#  Read definition of external packages. (try to find in following order).
#	(1) File specified by option "-D CONF=<file>".
#	(2) "${CMAKE_SOURCE_DIR}/CMakeConf.txt".
#	(3) Default file: "${SPR_SRC_DIR}/CMakeConf.txt.dist".
#
set(CONF_FILE_NAME "CMakeConf.txt")
set(CONF_FILE "${CMAKE_SOURCE_DIR}/${CONF_FILE_NAME}")
set(CONF_DEFAULT_FILE "${SPR_SRC_DIR}/${CONF_FILE_NAME}.dist")
include_file(CONF ${CONF_FILE} ${CONF_DEFAULT_FILE})

# ------------------------------------------------------------------------------
#  Find external packages if specifed.
#
if(DEFINED CMAKE_PREFIX_PATH)
    message(STATUS "CMAKE_PREFIX_PATH:")
    foreach(path IN LISTS CMAKE_PREFIX_PATH)
	message("       ${path}")
    endforeach()
    #
    message("finding package: Boost")
    cmake_policy(SET CMP0074 NEW)
    find_package(Boost REQUIRED)
    if(Boost_FOUND)
	message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
    else()
	message("** Boost NOT FOUND")
	return()
    endif()
    #
    message("finding package: GLEW")
    cmake_policy(SET CMP0074 NEW)
    if(DEFINED GLEW_LIB32_DIR AND "${CMAKE_GENERATOR_PLATFORM}" STREQUAL "x86")
	list(APPEND CMAKE_PREFIX_PATH ${GLEW_LIB32_DIR})
    endif()
    if(DEFINED GLEW_LIB64_DIR AND "${CMAKE_GENERATOR_PLATFORM}" STREQUAL "x64")
	list(APPEND CMAKE_PREFIX_PATH ${GLEW_LIB64_DIR})
    endif()
    find_package(GLEW)
    if(GLEW_FOUND)
	message(STATUS "GLEW_INCLUDE_DIRS: ${GLEW_INCLUDE_DIRS}")
    else()
	message("** GLEW NOT FOUND")
	return()
    endif()
    #
    message("finding package: GLUT")
    cmake_policy(SET CMP0074 NEW)
    find_package(GLUT REQUIRED)
    if(GLUT_FOUND)
	message(STATUS "GLUT_INCLUDE_DIR: ${GLUT_INCLUDE_DIR}")
    else()
	message("** GLUT NOT FOUND")
	return()
    endif()
endif()

# ------------------------------------------------------------------------------
#  Some other definitions.
#
if(Windows)
    set(RunSwigDir ${SPR_SRC_DIR}/RunSwig)
    set(Python ${RunSwigDir}/python_adapter.bat)
    file(TO_NATIVE_PATH "${RunSwigDir}" RunSwigDir)
    file(TO_NATIVE_PATH "${Python}" Python)
    message(STATUS "Visual Studio Version: ${VS_VERSION}")
    set(LIB_SUFFIX_DEBUG   D)
    set(LIB_SUFFIX_RELEASE  )
    set(LIB_SUFFIX_TRACE   T)
elseif(unix)
    set(Python python)
endif()

# ------------------------------------------------------------------------------
#  Definition of last build library type record file.
#
set(LASTBUILD_LIBTYPE_STATIC ${SPR_SRC_DIR}/lastbuilt.static)
set(LASTBUILD_LIBTYPE_SHARED ${SPR_SRC_DIR}/lastbuilt.shared)
if("${LIBTYPE}" STREQUAL "STATIC")
    set(LASTBUILD_LIBTYPE_FILE ${LASTBUILD_LIBTYPE_STATIC})
else()
    set(LASTBUILD_LIBTYPE_FILE ${LASTBUILD_LIBTYPE_SHARED})
endif()

# ------------------------------------------------------------------------------
#  Main target.
#
if(EXCLUDE_SRCS)
    message("++ exclude: ${EXCLUDE_SRCS}")
    list(REMOVE_ITEM SRCS ${EXCLUDE_SRCS})
endif()
if(Windows)
    if("${ARCHITECTURE}" STREQUAL "x64")
	add_executable(${Target} ${SRCS})
    else()
	add_executable(${Target} WIN32 ${SRCS})
    endif()
elseif(unix)
    add_executable(${Target} ${SRCS})
endif()

# ------------------------------------------------------------------------------
#  Compiler options.
#
if(Windows)
    set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT ${ProjectName})
    set(CMAKE_CXX_FLAGS		"${COMP_FLAGS}")
    set(CMAKE_CXX_FLAGS_DEBUG	"${COMP_FLAGS_DEBUG}")
    set(CMAKE_CXX_FLAGS_RELEASE	"${COMP_FLAGS_RELEASE}")
    set(CMAKE_CXX_FLAGS_TRACE	"${COMP_FLAGS_TRACE}")
elseif(unix)
    foreach(_flag ${COMP_FLAGS})
	target_compile_options(${Target} PRIVATE ${_flag})
    endforeach()
    set(CMAKE_CXX_COMPILER "${GPP_PATH}/g++")
endif()
foreach(_macro ${DEFINE_MACROS})
	target_compile_definitions(${Target} PRIVATE ${_macro})
endforeach()

# ------------------------------------------------------------------------------
#  Include directories.
#
set(CMAKE_USE_RELATIVE_PATHS TRUE)
target_include_directories(${Target} BEFORE PRIVATE ${INCLUDE_PATHS})
if(DEFINED Boost_INCLUDE_DIRS)
    target_include_directories(${Target} BEFORE PRIVATE ${Boost_INCLUDE_DIRS})
endif()
if(DEFINED GLEW_INCLUDE_DIRS)
    target_include_directories(${Target} BEFORE PRIVATE ${GLEW_INCLUDE_DIRS})
endif()
if(DEFINED GLUT_INCLUDE_DIR)
    target_include_directories(${Target} BEFORE PRIVATE ${GLUT_INCLUDE_DIR})
endif()

# ------------------------------------------------------------------------------
#  Linker options.
#
if(Windows)
    set(CMAKE_EXE_LINKER_FLAGS		"${LINK_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS_DEBUG	"${LINK_FLAGS_DEBUG}")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE	"${LINK_FLAGS_RELEASE}")
    set(CMAKE_EXE_LINKER_FLAGS_TRACE	"${LINK_FLAGS_TRACE}")
else()
    target_link_options(${Target} BEFORE PRIVATE ${LINK_FLAGS})
endif()

# ------------------------------------------------------------------------------
#  Define depending projects.
#
if(SPR_PROJS AND Windows)
    set(DEP_Base)
    set(DEP_Collision Foundation)
    set(DEP_Creature Foundation Framework Physics)
    set(DEP_EmbPython)
    set(DEP_FileIO Foundation)
    set(DEP_Foundation Base)
    set(DEP_Framework FileIO Foundation Graphics Physics)
    set(DEP_Graphics Foundation)
    set(DEP_HumanInterface Foundation)
    set(DEP_Physics Collision Foundation)
    add_subdirectory(${SPR_SRC_DIR}/RunSwig ${SPR_BLD_DIR}/RunSwig)
    foreach(proj IN ITEMS ${SPR_PROJS})
	add_subdirectory(${SPR_SRC_DIR}/${proj} ${SPR_BLD_DIR}/${proj})
	set(DEP_LIST RunSwig)
	foreach(dep IN ITEMS ${DEP_${proj}})
	    list(APPEND DEP_LIST ${dep})
	endforeach()
	add_dependencies(${proj} ${DEP_LIST})
	#message("   dependencies: ${proj} => ${DEP_LIST}")
    endforeach()
endif()

# ------------------------------------------------------------------------------
#  Library directories.
#
if(Windows)
    if("${ARCHITECTURE}" STREQUAL "x64")
	set(DEPENDENCY_LIBDIR ${SPR_TOP_DIR}/dependency/lib/win64)
    else()
	set(DEPENDENCY_LIBDIR ${SPR_TOP_DIR}/dependency/lib/win32)
    endif()
    target_link_directories(${Target} BEFORE
	PRIVATE ${LIBRARY_PATHS}
	PRIVATE ${DEPENDENCY_LIBDIR}
    )
else()
    set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS)

    if(NOT "${SHARED_LIBRARY_NAMES}" STREQUAL "${EMPTY}")
	# expand semicolon-joined-stirng element to list
	set(_out ${EMPTY})
	foreach(_elm ${LIBRARY_PATHS})
	    string(REPLACE ":" ";" _tmp ${_elm})
	    list(APPEND _out ${_tmp})
	endforeach()
	set(LIBRARY_PATHS ${_out})

	# change into g++ option format
	set(_flags_c "@")
	set(_flags_r "@")
	foreach(_path ${LIBRARY_PATHS})
	    set(_flags_c "-L${_path} ${_flags_c}")
	    set(_flags_r "${_path},${_flags_r}")
	endforeach()
	string(REPLACE " @" "" _flags_c ${_flags_c})
	string(REPLACE ",@" "" _flags_r ${_flags_r})
    endif()

    foreach(_path ${LIBRARY_PATHS})
	target_link_directories(${Target} PUBLIC "${_path}")
    endforeach()
endif()

# ------------------------------------------------------------------------------
#  Additional libraries.
#
if(Windows)
    target_link_libraries(${Target}
	${SPR_PROJS}
	${LIBRARY_NAMES}
    )
    foreach(_tmp ${CMAKE_CONFIGURATION_TYPES})
	string(TOUPPER ${_tmp} _conf)
	set(_lib ${LIBRARY_NAMES_${_conf}})
	target_link_libraries(${Target} $<$<CONFIG:${_conf}>:${_lib}>)
    endforeach()
else()
    set(_tmp_names "@")
    foreach(_name ${SHARED_LIBRARY_NAMES})
	set(_tmp_names "${_tmp_names},-l${_name}")
    endforeach()
    string(REPLACE "@," "" _library_names "${_tmp_names}")
    if(NOT "${_library_names}" STREQUAL "@")
	target_link_libraries(${Target} "-Wl,--start-group -Wl,-Bdynamic,${_library_names}")
	message("++ shared libraries: ${_library_names}")
    endif()

    if(NOT "${SHARED_LIBRARY_NAMES}" STREQUAL "${EMPTY}")
	set(_tmp_names "@")
	foreach(_name ${STATIC_LIBRARY_NAMES})
	    set(_tmp_names "-l${_name},${_tmp_names}")
	endforeach()
	string(REPLACE ",@" "" _library_names "${_tmp_names}")
	if(NOT "${_library_names}" STREQUAL "@")
	    target_link_libraries(${Target} "-Wl,-Bstatic,${_library_names}")
	    message("++ static libraries: ${_library_names}")
	endif()
    else()
	foreach(_name ${STATIC_LIBRARY_NAMES})
            target_link_libraries(${Target} ${_name})
	    message("++ static libraries: ${_name}")
	endforeach()
    endif()
endif()

# ------------------------------------------------------------------------------
#  Exclude default libraries (Windows only).
#
if(Windows AND DEFINED EXCLUDE_LIBRARY_NAMES)
    set(CMAKE_EXE_LINKER_FLAGS
	"${CMAKE_EXE_LINKER_FLAGS} /NODEFAULTLIB:\"${EXCLUDE_LIBRARY_NAMES}\"")
endif()

# ------------------------------------------------------------------------------
#  Set working directory to VS debugger.
#
if(Windows)
    foreach(_prop DEBUGGER_WORKING_DIRECTORY DEBUGGER_COMMAND_ARGUMENTS)
	if(DEFINED ${_prop})
	    set_target_properties(${Target} PROPERTIES VS_${_prop} ${${_prop}})
	endif()
    endforeach()
endif()

# end: CMakeLists.txt
