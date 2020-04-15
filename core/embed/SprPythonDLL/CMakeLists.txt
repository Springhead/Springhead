# CMakeLists.txt for Springhead Application

cmake_minimum_required(VERSION 3.1)

# ------------------------------------------------------------------------------
#  Read project name.
#
set(SETTING_FILE "CMakeSettings.txt")
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
#  Determine directries.
#
set(TOPDIR ../../..)
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
list(FIND LIBRARY_NAMES_ADD EmbPython _found)
if(${_found} EQUAL -1)
    set(LIBRARY_NAMES	${SPRLIB_NAMES}
			${LIBRARY_NAMES_ADD} ${STDLIB_NAMES})
else()
    list(REMOVE_ITEM LIBRARY_NAMES_ADD EmbPython)
    set(LIBRARY_NAMES	EmbPython ${SPRLIB_NAMES}
			${LIBRARY_NAMES_ADD} ${STDLIB_NAMES})
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
#  Main target.
#
if(EXCLUDE_SRCS)
    message("++ exclude: ${EXCLUDE_SRCS}")
    list(REMOVE_ITEM SRCS ${EXCLUDE_SRCS})
endif()
add_library(${Target} SHARED ${SRCS})

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
    set(CMAKE_CXX_COMPILER "/usr/bin/g++")
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
#  Library output directories.
#
if(Windows)
    set(LIBNAME_DEBUG   SprD)
    set(LIBNAME_RELEASE Spr)
    set(LIBNAME_TRACE   SprT)
    if("${CMAKE_GENERATOR_PLATFORM}" STREQUAL "x64")
	set(OUTPUT_DIRECTORY_BASE ${SPR_TOP_DIR}/generated/bin/win64)
    else()
	set(OUTPUT_DIRECTORY_BASE ${SPR_TOP_DIR}/generated/bin/win32)
    endif()
    set(OUTPUT_DIRECTORY ${OUTPUT_DIRECTORY_BASE}/Python)
    set(SUFFIX pyd)

    set_target_properties(${Target} PROPERTIES
	RUNTIME_OUTPUT_NAME_DEBUG   ${LIBNAME_DEBUG}
	RUNTIME_OUTPUT_NAME_RELEASE ${LIBNAME_RELEASE}
	RUNTIME_OUTPUT_NAME_TRACE   ${LIBNAME_TRACE}
	RUNTIME_OUTPUT_DIRECTORY_DEBUG   ${OUTPUT_DIRECTORY}
	RUNTIME_OUTPUT_DIRECTORY_RELEASE ${OUTPUT_DIRECTORY}
	RUNTIME_OUTPUT_DIRECTORY_TRACE   ${OUTPUT_DIRECTORY}
	SUFFIX ".${SUFFIX}"
    )
elseif(unix)
    set(LIBNAME libSpr)
    set(OUTPUT_DIRECTORY_BASE ${SPR_TOP_DIR}/generated/bin)
    set(OUTPUT_DIRECTORY ${OUTPUT_DIRECTORY_BASE}/PYthon)
    set(SUFFIX so)

    set_target_properties(${Target} PROPERTIES
	RUNTIME_OUTPUT_NAME ${LIBNAME}
	RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIRECTORY}
	SUFFIX ".${SUFFIX}"
    )
endif()

# ------------------------------------------------------------------------------
#  Add target to synchronize configuration with ${SRR_SRC_DIR}.
#
if(Windows)
    set(SYNC "sync")
    set(_tmp_args "-v"
		  "-a" "\"$(Platform)\""
		  "-c" "\"$(Configuration)\""
		  "${SPR_SRC_DIR}/${OOS_BLD_DIR}" "${CMAKE_BINARY_DIR}"
		  "${Target}" "${SPR_PROJS}")
    file(TO_NATIVE_PATH "${_tmp_args}" _tmp_args)
    add_custom_target(${SYNC}
	COMMAND python ${SPR_SRC_DIR}/cmake_sync.py ${_tmp_args}
	WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
	COMMENT "sync configuration with ${SPR_SRC_DIR}"
    )
endif()

# ------------------------------------------------------------------------------
#  Define depending projects.
#
if(Windows)
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
    foreach(proj IN ITEMS ${SPR_PROJS_PLUS})
	add_subdirectory(${SPR_SRC_DIR}/${proj} ${SPR_BLD_DIR}/${proj})
	set(DEP_LIST RunSwig)
	foreach(dep IN ITEMS ${DEP_${proj}})
	    list(APPEND DEP_LIST ${dep})
	endforeach()
	add_dependencies(${proj} ${DEP_LIST})
	#message("   dependencies: ${proj} => ${DEP_LIST}")
    endforeach()
    add_dependencies(RunSwig ${SYNC})
    add_dependencies(EmbPython_RunSwig ${SPR_PROJS} RunSwig)
    add_dependencies(${Target} ${SPR_PROJS_PLUS} ${SYNC})
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
    foreach(_proj ${SPR_PROJS_PLUS})
	target_link_directories(${Target} BEFORE
		PRIVATE ${SPR_SRC_DIR}/${_proj})
    endforeach()
else()
    set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS)
    foreach(_path ${LIBRARY_PATHS})
	target_link_directories(${Target} BEFORE PRIVATE ${_path})
    endforeach()
endif()

# ------------------------------------------------------------------------------
#  Additional libraries.
#
if(Windows)
    target_link_libraries(${Target} ${LIBRARY_NAMES})
    foreach(_proj ${SPR_PROJS_PLUS})
	set(_base ${_proj}${VS_VERSION})
	set(_post ${ARCHITECTURE}.lib)
	target_link_libraries(${Target} $<$<CONFIG:Debug>:${_base}D${_post}>)
	target_link_libraries(${Target} $<$<CONFIG:Release>:${_base}${_post}>)
	target_link_libraries(${Target} $<$<CONFIG:Trace>:${_base}T${_post}>)
    endforeach()
else()
    foreach(_name ${LIBRARY_NAMES})
	target_link_libraries(${Target} ${_name})
    endforeach()
endif()

# ------------------------------------------------------------------------------
#  Copy generated library file to "${SPR_TOP_DIR}/generated/lib"
#
if(Windows)
    set(_install_dir ${OUTPUT_DIRECTORY_BASE})
    set(_libdir ${OUTPUT_DIRECTORY})
    file(TO_NATIVE_PATH ${_install_dir} _install_dir)
    file(TO_NATIVE_PATH ${_libdir} _libdir)

    foreach(_tmp ${CMAKE_CONFIGURATION_TYPES})
	string(TOUPPER ${_tmp} _conf)
	set(_libfile_${_conf} ${_libdir}/${LIBNAME_${_conf}}.${SUFFIX})
	file(TO_NATIVE_PATH ${_libfile_${_conf}} _libfile_${_conf})
	set(_cmnd_${_conf} copy ${_libfile_${_conf}} ${_install_dir})
    endforeach()

    add_custom_command(TARGET ${Target}
	POST_BUILD
	#COMMAND mkdir ${_libdir}
	COMMAND if "'$(Configuration)'" equ "'Debug'"   ${_cmnd_DEBUG}
	COMMAND if "'$(Configuration)'" equ "'Release'" ${_cmnd_RELEASE}
	COMMAND if "'$(Configuration)'" equ "'Trace'"   ${_cmnd_TRACE}
	WORKING_DIRECTORY ${_wrkdir}
	COMMENT [[  copying DLL file ]]
    )
elseif(unix)
    set(_install_dir ${OUTPUT_DIRECTORY_BASE})
    set(_libdir ${OUTPUT_DIRECTORY})
    set(_libfile ${_libdir}/${OUTPUT_NAME})
    file(TO_NATIVE_PATH ${_install_dir} _install_dir)
    file(TO_NATIVE_PATH ${_libfile} _libfile)

    add_custom_command(TARGET ${Target}
	POST_BUILD
	COMMAND mkdir -p ${_libdir}
	COMMAND cp ${_libfile} ${_install_dir}
	WORKING_DIRECTORY ${_wrkdir}
	COMMENT [[  copying shared library file ]]
    )
endif()

# end: CMakeLists.txt
