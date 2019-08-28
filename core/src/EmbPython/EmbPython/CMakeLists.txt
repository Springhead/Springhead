# CMakeLists.txt for Springhead

cmake_minimum_required(VERSION 3.1)

# ------------------------------------------------------------------------------
#  Definition of project.
#
set(ProjectName "EmbPython")
message(STATUS "Project: ${ProjectName}")
project(${ProjectName} CXX)
set(CMAKE_SUPPRESS_REGENERATION true)
set(CMAKE_USE_RELATIVE_PATHS true)

# ------------------------------------------------------------------------------
#  Determine top directory of Springhead tree.
#	If cmake option "-D TOPDIR=<absolute-dir-path>" is specified,
#	    <abs-dir-path> is the one.
#	Else if the file "${CMAKE_SOURCE_DIR}/CMakeTopDir.txt" exists,
#	    this file should contain the definition of top dorectory like
#	    set(TOPDIR "<absolute-dir-path>")
#	Otherwise, "${CMAKE_SOURCE_DIR}/CMakeTopDir.txt" SHOULD contain the
#	    "${CMAKE_SOURCE_DIR}/../.." is the top directory/
#
set(TOPDIR_FILE "CMakeTopdir.txt")
if(DEFINED TOPDIR)
    if(NOT EXISTS "${TOPDIR}")
	message("Error: File \"${TOPDIR}\" not found")
	return()
    endif()
elseif(EXISTS "${CMAKE_SOURCE_DIR}/${TOPDIR_FILE}")
    include(${TOPDIR_FILE})
else()
    set(TOPDIR "${CMAKE_SOURCE_DIR}/../../../..")
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

#set(CMAKE_BUILD_TYPE Release)
#set(CMAKE_CONFIGURATION_TYPES ${CMAKE_BUILD_TYPE} CACHE STRING "" FORCE)
set(CMAKE_CONFIGURATION_TYPES Debug Release Trace)
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
set(OPTS_FILE "${CMAKE_SOURCE_DIR}/${OPTS_FILE_NAME}")
set(OPTS_DEFAULT_FILE "${SPR_SRC_DIR}/${OPTS_FILE_NAME}.dist")
include_file(OPTS ${OPTS_FILE} ${OPTS_DEFAULT_FILE})

# ------------------------------------------------------------------------------
#  Set compiler/linker options.
#
if(${Windows})
    message(STATUS "Host System: Windows")
    set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT ${ProjectName})
    set(CMAKE_CXX_FLAGS		${WIN_COPT_COMMON})
    set(CMAKE_CXX_FLAGS_DEBUG	${WIN_COPT_DEBUG})
    set(CMAKE_CXX_FLAGS_RELEASE	${WIN_COPT_RELEASE})
    set(CMAKE_CXX_FLAGS_TRACE	${WIN_COPT_TRACE})
    foreach(macros IN ITEMS ${WIN_COPT_MACROS})
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /D ${macros}")
    endforeach()
elseif(${Linux})
    message(STATUS "Host System: Linux")
    set(LIB_DIR	"${SPR_CORE_DIR}/lib")
    set(CMAKE_CXX_FLAGS		${LINUX_COPT_COMMON})
    set(CMAKE_CXX_FLAGS_DEBUG	${LINUX_COPT_DEBUG})
    set(CMAKE_CXX_FLAGS_RELEASE	${LINUX_COPT_RELEASE})
    foreach(macros IN ITEMS ${WIN_COPT_MACROS})
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D ${macros}")
    endforeach()
    set(LDFLAGS.EXTRA	"-L${LIB_DIR}/linux/ -L/usr/lib \
			 -L/usr/X11R6/lib -L/usr/lib/X11")
    set(LDFLAGS.GL	"-lX11 -lXi -lXmu -lglui -lglut -lGLEW -lGLU -lGL -lm \
			 -ldl -lboost_regex")
    set(INCLUDES	"-I/usr/X11R6/include/ -I/usr/X11/include")
    set(ARFLAGS		"r")
    set(LDFLAGS "-L${SRC_DIR}/Base/ \
		 -L${SRC_DIR}/Collision/ \
		 -L${SRC_DIR}/Creature/ \
		 -L${SRC_DIR}/FileIO/ \
		 -L${SRC_DIR}/Foundation/ \
		 -L${SRC_DIR}/Framework/ \
		 -L${SRC_DIR}/Graphics/ \
		 -L${SRC_DIR}/HumanInterface/ \
		 -L${SRC_DIR}/Physics/ \
		 -lBase -lCollision -lCreature -lFileIO -lFoundation -lFramework \
		 -lGraphics -lHumanInterface -lPhysics
		 ${LDFLAGS.EXTRA} \
		 ${LDFLAGS.GL}")
    set(INCLUDES "${INCLUDES} -I${INC_DIR} -I${SRC_DIR} -I.")
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
#  Find external packages if specified.
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
#  Additional include directories.
#
#if(DEFINED CMAKE_PREFIX_PATH)
#    set(ADDITIONAL_INCDIR "C:/Package/glui-2.37/include")
#endif()

# ------------------------------------------------------------------------------
#  Some other definitions.
#
if(${Windows})
    #set(RunSwigDir ${CMAKE_SOURCE_DIR}/RunSwig)
    set(RunSwigDir ${TOPDIR}/core/src/RunSwig)
    set(Python ${RunSwigDir}/python_adapter.bat)
#    if(NOT DEFINED VS_VERSION)
#	execute_process(
#	    COMMAND ${Python} get_vs_version.py
#	    OUTPUT_VARIABLE VS_VERSION
#	)
#    endif()
    message(STATUS "Visual Studio Version: ${VS_VERSION}")
elseif(${Linux})
    set(Python python)
endif()

# ------------------------------------------------------------------------------
#  Define sources.
#	To ensure that GLOB to work, make nessesary (empty) files temporarily
#	which EmbPython_RunSwig will create later at build time.
#
set(SPR_PROJS Base Collision Creature FileIO Foundation Framework Graphics
	      HumanInterface Physics)
foreach(_tmp ${SPR_PROJS})
	file(TOUCH "../EP${_tmp}.h" "../EP${_tmp}.cpp")
endforeach()
file(GLOB SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
		../*.i ../*.h ../*.cpp
		../EPUtility.h ../EPUtility.cpp ../EPInterpreter.cpp
	 	../Utility/PyPTM.i ../Utility/EPObject.i)
##list(APPEND SRCS "EPUtility.h" "EPUtility.cpp" "EPInterpreter.cpp"
##		 "Utility/PyPTM.i" "Utility/EPObject.i")
list(REMOVE_ITEM SRCS ../SprPythonDLL.cpp)		# Exclude

# ------------------------------------------------------------------------------
#  Main target.
#
set(Target ${ProjectName})
add_library(${Target} STATIC ${SRCS})

# output directory and library name
if(${Windows})
    set_target_properties("${Target}" PROPERTIES
	OUTPUT_NAME_DEBUG   "${Target}${VS_VERSION}D${CMAKE_GENERATOR_PLATFORM}"
	OUTPUT_NAME_RELEASE "${Target}${VS_VERSION}${CMAKE_GENERATOR_PLATFORM}"
	OUTPUT_NAME_TRACE   "${Target}${VS_VERSION}T${CMAKE_GENERATOR_PLATFORM}"
	ARCHIVE_OUTPUT_DIRECTORY_DEBUG   ${SPR_SRC_DIR}/${Target}
	ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${SPR_SRC_DIR}/${Target}
	ARCHIVE_OUTPUT_DIRECTORY_TRACE   ${SPR_SRC_DIR}/${Target}
    )
elseif(${Linux})
endif()

# ------------------------------------------------------------------------------
#  Include directories.
#
include_directories(${Target}
    PRIVATE ${CMAKE_SOURCE_DIR}
    PRIVATE ${SPR_INC_DIR}
    PRIVATE ${SPR_INC_DIR}/EmbPython
    PRIVATE ${SPR_SRC_DIR}
    PRIVATE ${SPR_SRC_DIR}/EmbPython/Utility
    PRIVATE ${Boost_INCLUDE_DIRS}
)

# ------------------------------------------------------------------------------
#  Add target to generate EmbPython sources.
#
set(EmbPython_RunSwig "EmbPython_RunSwig")
if(${Windows})
    add_custom_target(${EmbPython_RunSwig} ALL
	COMMAND python EmbPython_clean.py -v
	COMMAND RunSwig_EmbPython.bat
	WORKING_DIRECTORY "${SPR_SRC_DIR}/EmbPython"
	COMMENT [[ generating EmbPython sources ]]
)
elseif(${Linux})
endif()

# ------------------------------------------------------------------------------
#  Define dependencies between targets.
#
add_dependencies(${Target} ${EmbPython_RunSwig})

# end: CMakeLists.txt
