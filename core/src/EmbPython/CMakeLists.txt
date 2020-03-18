# CMakeLists.txt for EmbPython

# ------------------------------------------------------------------------------
#  Commandline option "-DSTANDALONE=1" indicates 'standalone' version of
#  creating library "libEmbPyson.a" on unix system.
#
if(STANDALONE)
    cmake_minimum_required(VERSION 3.1)
    set(ProjectName EmbPython)
    message(STATUS "Project: ${ProjectName}")
    project(${ProjectName} CXX)
    set(Target ${ProjectName})
    set(CMAKE_SUPPRESS_REGENERATION true)
    set(CMAKE_USE_RELATIVE_PATHS true)
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
    #
    include("${SPR_CORE_DIR}/make.system.cmake")
    message(STATUS "Architecture: ${ARCHITECTURE}")
    set(CMAKE_GENERATOR_PLATFORM ${ARCHITECTURE})
    include("${SPR_CORE_DIR}/make.func.cmake")
    #
    if(Windows)
	set(CMAKE_CONFIGURATION_TYPES Debug Release Trace)
	set(LIBTYPE STATIC)
	set(VS_VERSION "15.0")
	set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT ${ProjectName})
    else()
	set(CMAKE_CONFIGURATION_TYPES Release)
	set(LIBTYPE STATIC)
    endif()
    #
    set(OPTS_FILE_NAME "CMakeOpts.txt")
    set(OPTS_FILE "${SPR_SRC_DIR}/${OPTS_FILE_NAME}")
    set(OPTS_DEFAULT_FILE "${SPR_SRC_DIR}/${OPTS_FILE_NAME}.dist")
    include_file(OPTS ${OPTS_FILE} ${OPTS_DEFAULT_FILE})
endif() 

# ------------------------------------------------------------------------------
#  Definition of target.
#
set(Target "EmbPython")
message("** ${Target} **")

# ------------------------------------------------------------------------------
#  Read setting parameters.
#
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/CMakeSettings.txt")
    include("${CMAKE_CURRENT_SOURCE_DIR}/CMakeSettings.txt")
endif()

# ------------------------------------------------------------------------------
# EmbPython_RunSwig generate files.
#	These files are removed when 'clean' target executed.
#
set(SPR_PROJS Base Collision Creature FileIO Foundation Framework Graphics
		HumanInterface Physics)
set(_for_src EP@.h EP@.cpp Makefile_EmbPython.swig.@ swig_@.log)
set(_for_inc SprEP@.h)

set(generated_files)
set(EMBPYTHON_PROJS ${SPR_PROJS})
list(REMOVE_ITEM EMBPYTHON_PROJS EmbPython)
foreach(_dir ${EMBPYTHON_PROJS})
    foreach(_file ${_for_src})
	set(_tmp ${SPR_SRC_DIR}/EmbPython/${_file})
	string(REPLACE @ ${_dir} _tmp ${_tmp})
	list(APPEND generated_files ${_tmp})
    endforeach()
endforeach()
foreach(_dir ${EMBPYTHON_PROJS})
    foreach(_file ${_for_inc})
	set(_tmp ${SPR_INC_DIR}/EmbPython/${_file})
	string(REPLACE @ ${_dir} _tmp ${_tmp})
	list(APPEND generated_files ${_tmp})
    endforeach()
endforeach()

foreach(_file ${generated_files})
    file(TO_NATIVE_PATH "${_file}" _file)
    #message("     generate: ${_file}")
endforeach()

# ------------------------------------------------------------------------------
#  Define sources and headers.
#
file(GLOB SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
	       *.i EPUtility.h
	       EPInterpreter.cpp EPUtility.cpp
 	       Utility/*.i Utility/*.cpp
	       ../../include/EmbPython/EmpPython.h
	       ../../include/EmbPython/SprEPApp.h
	       ../../include/EmbPython/SprEPInterpreter.h
	       ../../include/EmbPython/SprEPUtility.h
	       ../../include/EmbPython/SprEPVersion.h
	       ../../include/EmbPython/Utility/*.h
)
list(APPEND SRCS ${generated_files})

# ------------------------------------------------------------------------------
#  Read compiler/linker option file (try to find in following order).
#	(1) File specified by option "-D OPTS=<file>".
#	(2) "${SPR_SRC_DIR}/CMakeOpts.txt".
#	(3) Default file: "${SPR_SRC_DIR}/CMakeOpts.txt.Lib.dist".
#
if(NOT STANDALONE)
    set(OPTS_FILE_NAME "CMakeOpts.txt")
    set(OPTS_FILE "${SPR_SRC_DIR}/${OPTS_FILE_NAME}")
    set(OPTS_DEFAULT_FILE "${SPR_SRC_DIR}/${OPTS_FILE_NAME}.dist")
    include_file(OPTS ${OPTS_FILE} ${OPTS_DEFAULT_FILE})
endif()

# ------------------------------------------------------------------------------
#  Main target.
#
add_library(${Target} STATIC ${SRCS})

# ------------------------------------------------------------------------------
#  Apply local settings.
#
if(Windows)
    set(CMAKE_CXX_FLAGS		"${COMP_FLAGS}")
    set(CMAKE_CXX_FLAGS_DEBUG	"${COMP_FLAGS_DEBUG}")
    set(CMAKE_CXX_FLAGS_RELEASE	"${COMP_FLAGS_RELEASE}")
    set(CMAKE_CXX_FLAGS_TRACE	"${COMP_FLAGS_TRACE}")
elseif(unix)
    foreach(_flag ${COMP_FLAGS})
        target_compile_options(${Target} PUBLIC ${_flag})
    endforeach()
    set(CMAKE_CXX_COMPILER "/usr/bin/g++")
endif()
foreach(_macro ${DEFINE_MACROS})
        target_compile_definitions(${Target} PUBLIC ${_macro})
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
    set(LIB_SUFFIX_DEBUG   D)
    set(LIB_SUFFIX_RELEASE  )
    set(LIB_SUFFIX_TRACE   T)
    set(_platform ${CMAKE_GENERATOR_PLATFORM})
    set(_output_name_debug   "${Target}${VS_VERSION}${LIB_SUFFIX_DEBUG}${_platform}")
    set(_output_name_release "${Target}${VS_VERSION}${LIB_SUFFIX_RELEASE}${_platform}")
    set(_output_name_trace   "${Target}${VS_VERSION}${LIB_SUFFIX_TRACE}${_platform}")
    set_target_properties("${Target}" PROPERTIES
	OUTPUT_NAME_DEBUG   ${_output_name_debug}
	OUTPUT_NAME_RELEASE ${_output_name_release}
	OUTPUT_NAME_TRACE   ${_output_name_trace}
	ARCHIVE_OUTPUT_DIRECTORY_DEBUG   ${SPR_SRC_DIR}/${Target}
	ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${SPR_SRC_DIR}/${Target}
	ARCHIVE_OUTPUT_DIRECTORY_TRACE   ${SPR_SRC_DIR}/${Target}
    )
elseif(unix)
    set(_output_name_debug   "lib${Target}.a")
    set(_output_name_release "lib${Target}.a")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${SPR_SRC_DIR}/${Target})
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${SPR_SRC_DIR}/${Target})
endif()

# ------------------------------------------------------------------------------
#  Add target to generate EmbPython sources.
#
##if(Windows)
##    add_custom_command(
##	OUTPUT ${generated_files}
##	COMMAND cmd /c RunSwig_EmbPython.bat
##	WORKING_DIRECTORY "${SPR_SRC_DIR}/EmbPython"
##	COMMENT [[  generating EmbPython sources ]]
##    )
##elseif(unix)
##endif()

set(CleanTarget EmbPython_RunSwig)
add_dependencies(${Target} ${CleanTarget})
if(STANDALONE)
    set(_external_dependencies)
else()
    set(_external_dependencies RunSwig)
endif()

set(_wrkdir ${CMAKE_CURRENT_SOURCE_DIR})
if(Windows)
    set(_tmp_cmnd cmd /c ${SPR_SRC_DIR}\\do_python.bat)
elseif(unix)
    set(_tmp_cmnd python)
endif()
set(_tmp_args ${SPR_SRC_DIR}/EmbPython/RunSwig_EmbPython.py)
file(TO_NATIVE_PATH ${_tmp_args} _tmp_args)

add_custom_target(${CleanTarget}
    BYPRODUCTS ${genereated_files}
    DEPENDS ${generated_files} ${_external_dependencies}
)
add_custom_command(
    OUTPUT ${generated_files}
    COMMAND set _SPRTOP_=${SPR_TOP_DIR} & ${_tmp_cmnd} ${_tmp_args}
    WORKING_DIRECTORY ${_wrkdir}
    COMMENT [[  generating EmbPython sources ]]
)

# ------------------------------------------------------------------------------
#  Copy generated library file to "${SPR_TOP_DIR}/generated/lib"
#
if(STANDALONE)
    set(_install_dir ${SPR_TOP_DIR}/generated/lib)
    if(Windows)
	if("${ARCHITECTURE}" STREQUAL "x64")
	    set(_install_dir ${_install_dir}/win64)
	else()
	    set(_install_dir ${_install_dir}/win32)
	endif()
    endif()
    file(TO_NATIVE_PATH ${_install_dir} _install_dir)
    if(Windows)
	set(_cp copy)
    elseif(unix)
	set(_cp cp)
    endif()
    set(_libdir ${SPR_SRC_DIR}/${Target})
    set(_cmnd_DEBUG   "${_cp} ${_libdir}/${_output_name_debug}   ${_install_dir}")
    set(_cmnd_RELEASE "${_cp} ${_libdir}/${_output_name_release} ${_install_dir}")
    set(_cmnd_TRACE   "${_cp} ${_libdir}/${_output_name_trace}   ${_install_dir}")
    set(_libfile_DEBUG   ${_cp} ${_libdir}/${_output_name_debug})
    set(_libfile_RELEASE ${_cp} ${_libdir}/${_output_name_release})
    set(_libfile_TRACE   ${_cp} ${_libdir}/${_output_name_trace})

    foreach(_tmp ${CMAKE_CONFIGURATION_TYPES})
	string(TOUPPER ${_tmp} _conf)
	string(TOLOWER ${_tmp} _conf_low)
	set(_cmnd _cmnd_${_conf})
	add_custom_command(TARGET ${Target}
	    POST_BUILD
	    COMMAND $<$<CONFIG:${_conf}>:${${_cmnd}}>
	    WORKING_DIRECTORY ${_wrkdir}
	    COMMENT [[  copying liibrary file ]]
	)
    endforeach()
endif()

# end: CMakeLists.txt
