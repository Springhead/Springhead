# CMakeLists.txt for Springhead

cmake_minimum_required(VERSION 3.1)

# ------------------------------------------------------------------------------
#  Definition of project.
#
set(ProjectName "Springhead")
message(STATUS "Project: ${ProjectName}")
project(${ProjectName} CXX)
set(CMAKE_SUPPRESS_REGENERATION true)
set(CMAKE_USE_RELATIVE_PATHS true)

# ------------------------------------------------------------------------------
#  Read default/customized parameter file (try to find in following order).
#	(1) "${CMAKE_SOURCE_DIR}/CMakeCustomize.txt".
#	(2) File specified by option "CUSTOM".
#	(3) Default file at "${TOPDIR}/CMakeCustomize.txt.sample".
#	(4) Default file at "${CMAKE_SOURCE_DIR}/CMakeCustomize.txt.sample".
#
set(CUSTOM_FNAME "CMakeCustomize.txt")
set(CUSTOM_FILE "")
if(EXISTS "${CMAKE_SOURCE_DIR}/${CUSTOM_FNAME}")
    # case (1)
    set(CUSTOM_FILE "${CMAKE_SOURCE_DIR}/${CUSTOM_FNAME}")
elseif(DEFINED CUSTOM)
    # case (2)
    set(CUSTOM_FILE "${CUSTOM}")
elseif(DEFINED TOPDIR AND EXISTS "${TOPDIR}/${CUSTOM_FNAME}.sample")
    # case (3)
    set(CUSTOM_FILE "${TOPDIR}/${CUSTOM_FNAME}.sample")
elseif(EXISTS "${CMAKE_SOURCE_DIR}/${CUSTOM_FNAME}.sample")
    # case (4)
    set(CUSTOM_FILE "${CMAKE_SOURCE_DIR}/${CUSTOM_FNAME}.sample")
endif()
if("${CUSTOM_FILE}" STREQUAL "")
    message("** \"CMakeCustom.txt\" NOT FOUND **")
    return()
endif()
message("** Using customize file: \"${CUSTOM_FILE}\"")
include(${CUSTOM_FILE})

# ------------------------------------------------------------------------------
#  Top directory of Springhead tree.
#	Top directory can set by two ways.
#	  (1) Define directory in the file "CMakeLists_customize.txt" 
#	      by set statement like "set(TOPDIR "<dir>")".
#	  (2) Invoke cmake command with option "-D TOPDIR=<dir>".
#	Otherwise, we assume that cmake is invoked at "<dir>/core/src".
#
if("${TOPDIR}" STREQUAL "")
    set(TOPDIR "${CMAKE_SOURCE_DIR}/../..")
endif()
get_filename_component(TOPDIR ${TOPDIR} ABSOLUTE)
set(SPR_TOP_DIR ${TOPDIR})
set(SPR_CORE_DIR "${TOPDIR}/core")
set(SPR_INC_DIR "${TOPDIR}/core/include")

# ------------------------------------------------------------------------------
#  Sytem environment.
#
include("${SPR_CORE_DIR}/make.system.cmake")
message(STATUS "Architecture: ${ARCHITECTURE}")

set(CMAKE_CONFIGURATION_TYPES Debug Release Trace)
set(CMAKE_GENERATOR_PLATFORM ${ARCHITECTURE})
set(CMAKE_BUILD_TYPE Release)

# ------------------------------------------------------------------------------
#  Read function definitions.
#
include("${SPR_CORE_DIR}/make.func.cmake")

# ------------------------------------------------------------------------------
#  Set compile options.
#
if(${Windows})
    message(STATUS "Host System: Windows")
    set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT ${ProjectName})
    set(CMAKE_CXX_FLAGS		${WIN_COPT_COMMON})
    set(CMAKE_CXX_FLAGS_DEBUG	${WIN_COPT_DEBUG})
    set(CMAKE_CXX_FLAGS_RELEASE	${WIN_COPT_RELEASE})
    set(CMAKE_CXX_FLAGS_TRACE	${WIN_COPT_TRACE})
elseif(${Linux})
    message(STATUS "Host System: Linux")
    set(SRC_DIR	"${Ssrc/PR_CORE_DIR}/src")
    set(INC_DIR	"${SPR_CORE_DIR}/include")
    set(LIB_DIR	"${SPR_CORE_DIR}/lib")
    set(CMAKE_CXX_FLAGS		${LINUX_COPT_COMMON})
    set(CMAKE_CXX_FLAGS_DEBUG	${LINUX_COPT_DEBUG})
    set(CMAKE_CXX_FLAGS_RELEASE	${LINUX_COPT_RELEASE})
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
#  Some other definitions.
#
if(${Windows})
    set(RunSwigDir ${CMAKE_SOURCE_DIR}/RunSwig)
    set(Python ${RunSwigDir}/python_adapter.bat)
    set(VS_VERSION "15.0")
    message(STATUS "Visual Studio Version: ${VS_VERSION}")
elseif(${Linux})
    set(Python python)
endif()

# ------------------------------------------------------------------------------
#  Definition of external packages.
#
if(DEFINED BOOST_ROOT)
    message("++ finding package: Boost")
    cmake_policy(SET CMP0074 NEW)
    find_package(Boost REQUIRED)
    if(Boost_FOUND)
	message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
    else()
	message("** Boost NOT FOUND")
    endif()
endif()
if(DEFINED GLEW_ROOT)
    message("++ finding package: GLEW")
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
    endif()
endif()
if(DEFINED GLUT_ROOT)
    message("++ finding package: GLUT")
    cmake_policy(SET CMP0074 NEW)
    find_package(GLUT REQUIRED)
    if(GLUT_FOUND)
	message(STATUS "GLUT_INCLUDE_DIR: ${GLUT_INCLUDE_DIR}")
    else()
	message("** GLUT NOT FOUND")
    endif()
endif()

# ------------------------------------------------------------------------------
#  Define depending projects.
#
add_subdirectory(Base)
add_subdirectory(Collision)
add_subdirectory(Creature)
add_subdirectory(FileIO)
add_subdirectory(Foundation)
add_subdirectory(Framework)
add_subdirectory(Graphics)
add_subdirectory(HumanInterface)
add_subdirectory(Physics)
add_subdirectory(RunSwig)

add_dependencies(Base RunSwig)
add_dependencies(Collision Foundation RunSwig)
add_dependencies(Creature Foundation Framework Physics RunSwig)
add_dependencies(FileIO Foundation RunSwig)
add_dependencies(Foundation Base RunSwig)
add_dependencies(Framework FileIO Foundation Graphics Physics RunSwig)
add_dependencies(Graphics Foundation RunSwig)
add_dependencies(HumanInterface Foundation RunSwig)
add_dependencies(Physics Collision Foundation RunSwig)

# ------------------------------------------------------------------------------
#  Main target.
#
set(Target ${ProjectName})
set(OUTDIR_BASE ${SPR_TOP_DIR}/generated/lib)
set(LIBNAME_DEBUG   ${VS_VERSION}D${CMAKE_GENERATOR_PLATFORM})
set(LIBNAME_RELEASE ${VS_VERSION}${CMAKE_GENERATOR_PLATFORM})
set(LIBNAME_TRACE   ${VS_VERSION}T${CMAKE_GENERATOR_PLATFORM})
set(LIBCMND_DEBUG   SpringheadLib.bat ${CMAKE_GENERATOR_PLATFORM} ${LIBNAME_DEBUG})
set(LIBCMND_RELEASE SpringheadLib.bat ${CMAKE_GENERATOR_PLATFORM} ${LIBNAME_RELEASE})
set(LIBCMND_TRACE   SpringheadLib.bat ${CMAKE_GENERATOR_PLATFORM} ${LIBNAME_TRACE})

# ------------------------------------------------------------------------------
#  Clean (only for RunSwig).
#
if ("${CMAKE_GENERATOR_PLATFORM}" STREQUAL "x64")
    set(LIBDIR ${SPR_TOP_DIR}/generated/lib/win64)
else ()
    set(LIBDIR ${SPR_TOP_DIR}/generated/lib/win32)
endif()
set(LIBPATH_DEBUG   ${LIBDIR}/Springhead${LIBNAME_DEBUG}.lib)
set(LIBPATH_RELEASE ${LIBDIR}/Springhead${LIBNAME_RELEASE}.lib)
set(LIBPATH_TRACE   ${LIBDIR}/Springhead${LIBNAME_TRACE}.lib)
set(CLEAN ${Python} ${CMAKE_SOURCE_DIR}/RunSwig/Clean.py ${CMAKE_SOURCE_DIR})

if(${Windows})
    add_custom_target(${ProjectName} ALL
	COMMAND cd ${CMAKE_SOURCE_DIR}
	COMMAND if "'$(Configuration)'" equ "'Debug'"   ${LIBCMND_DEBUG}
	COMMAND if "'$(Configuration)'" equ "'Release'" ${LIBCMND_RELEASE}
	COMMAND if "'$(Configuration)'" equ "'Trace'"   ${LIBCMND_TRACE}
	DEPENDS Base Collision Creature FileIO Foundation Framework Graphics HumanInterface Physics
	COMMENT [[  generating Springhead Library... ]]
    )
    add_custom_target(RunSwig_Clean
	COMMAND if "'$(Configuration)'" equ "'Debug'"   ${CLEAN} ${LIBPATH_DEBUG}
	COMMAND if "'$(Configuration)'" equ "'Release'" ${CLEAN} ${LIBPATH_RELEASE}
	COMMAND if "'$(Configuration)'" equ "'Trace'"   ${CLEAN} ${LIBPATH_TRACE}
	COMMENT [[  clearing RunSwig generated files and Springhead Library... ]]
    )
elseif(${Linux})
endif()

# ------------------------------------------------------------------------------
#  Install.
#
if(DEFINED SPRINGHEAD_INCLUDE_PREFIX	  AND
   DEFINED SPRINGHEAD_LIBRARY_DIR_DEBUG	  AND
   DEFINED SPRINGHEAD_LIBRARY_DIR_RELEASE)
    message("** INSTALL: Debug:   ${SPRINGHEAD_LIBRARY_DIR_DEBUG} **")
    message("** INSTALL: Release: ${SPRINGHEAD_LIBRARY_DIR_RELEASE} **")
    install(FILES ${LIBPATH_DEBUG}
	DESTINATION ${SPRINGHEAD_LIBRARY_DIR_DEBUG}
	PERMISSIONS OWNER_READ GROUP_READ WORLD_READ
	CONFIGURATIONS Debug
    )
    install(FILES ${LIBPATH_RELEASE}
	DESTINATION ${SPRINGHEAD_LIBRARY_DIR_RELEASE}
	PERMISSIONS OWNER_READ GROUP_READ WORLD_READ
	CONFIGURATIONS Release
    )
    message("** INSTALL: Headers: ${SPRINGHEAD_INCLUDE_PREFIX} **")
    install(DIRECTORY ${SPR_INC_DIR}
	DESTINATION ${SPRINGHEAD_INCLUDE_PREFIX}
    )
endif()

# end: CMakeLists.txt
