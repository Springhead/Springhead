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
    else()
	set(CMAKE_CONFIGURATION_TYPES Release)
	set(LIBTYPE STATIC)
    endif()
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
set(OPTS_FILE_NAME "CMakeOpts.txt")
set(OPTS_FILE "${SPR_SRC_DIR}/${OPTS_FILE_NAME}")
set(OPTS_DEFAULT_FILE "${SPR_SRC_DIR}/${OPTS_FILE_NAME}.dist")
include_file(OPTS ${OPTS_FILE} ${OPTS_DEFAULT_FILE})

# ------------------------------------------------------------------------------
#  Main target.
#
add_library(${Target} STATIC ${SRCS})

# ------------------------------------------------------------------------------
#  Apply local settings.
#
if(${Windows})
    set(CMAKE_CXX_FLAGS		"${COMP_FLAGS}")
    set(CMAKE_CXX_FLAGS_DEBUG	"${COMP_FLAGS_DEBUG}")
    set(CMAKE_CXX_FLAGS_RELEASE	"${COMP_FLAGS_RELEASE}")
    set(CMAKE_CXX_FLAGS_TRACE	"${COMP_FLAGS_TRACE}")
elseif(${unix})
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
if(${Windows})
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
elseif(${Linux})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${SPR_SRC_DIR}/${Target})
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${SPR_SRC_DIR}/${Target})
endif()

# ------------------------------------------------------------------------------
#  Add target to generate EmbPython sources.
#
if(${Windows})
    add_custom_command(
	OUTPUT ${generated_files}
	COMMAND cmd /c RunSwig_EmbPython.bat
	WORKING_DIRECTORY "${SPR_SRC_DIR}/EmbPython"
	COMMENT [[  generating EmbPython sources ]]
    )
elseif(${Linux})
endif()

# end: CMakeLists.txt
