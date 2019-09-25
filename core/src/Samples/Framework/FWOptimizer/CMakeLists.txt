# CMakeLists.txt for Springhead
#
#   Change parametes in "Customization section" below to customize compile
#   and/or link condition.

cmake_minimum_required(VERSION 3.1)

# ------------------------------------------------------------------------------
#  Definition of project.
#
set(ProjectName "FWOptimizer")
message(STATUS "Project: ${ProjectName}")
project(${ProjectName} CXX)
set(CMAKE_SUPPRESS_REGENERATION true)
set(CMAKE_USE_RELATIVE_PATHS true)
#
set(Target ${ProjectName})

# ******************************************************************************
#  Customization section.
# ******************************************************************************
set(EMPTY)

#  Binary(working) directory name used when you cmake Springhead library.
#	In order to optimize build process between Springhead projects and
#	your application program, you should adopt take "out-of-source"
#	build method.
#
set(OOS_BLD_DIR "build")

#  Configuration type(s).
#	Remove unneccessary configuration type(s).
set(CMAKE_CONFIGURATION_TYPES Debug Release Trace)

#  Source/include file names to incorporate to the application program.
#
file(GLOB SRCS *.cpp *.h)

#  Source/include file name(s) to EXCLUDE from compilation.
#	Replace '${EMPTY}' with file name(s).
#	Wild card is NOT available here.
#	We recomend to use absolute path if file exists outside of
#	current source directory (${CMAKE_SOURCE_DIR}).
set(EXCLUDE_SRCS ${EMPTY})

#  Springhead projects to incorporate in the application program.
#	Remove unneccessary project name(s).
#	Do not include 'RunSwig' in the list.
set(SPR_PROJS Base Collision FileIO Foundation Framework Graphics HumanInterface Physics)

#  Additional include path(s).
#	Replace '${EMPTY}' with absolute path(s).
#	Current directory can be refered as '${CMAKE_SOURCE_DIR}'.
set(ADDITIONAL_INCDIR ${EMPTY})
#set(ADDITIONAL_INCDIR "F:/Project/ExternalPackage/glui-2.37/include")

#  Additional library path(s).
#	Replace '${EMPTY}' with absolute path(s).
set(ADDITIONAL_LIBDIR ${EMPTY})
#set(ADDITIONAL_LIBDIR
#	"F:/Project/ExternalPackage/freeglut/lib/x64"
#	"F:/Project/Springhead/dependency/lib/win64")

#  Additional library name(s) required.
#	Replace '${EMPTY}' with file name(s).
set(ADDITIONAL_LIBS XINPUT9_1_0.LIB)

#  Default library name(s) to EXCLUDE from linking.
#	Replace '${EMPTY}' with file name(s).
set(EXCLUDE_LIBS XINPUT.LIB)

# ******************************************************************************
#  End of customization section.
# ******************************************************************************

# ------------------------------------------------------------------------------
#  Determine top directory of Springhead tree.
#	If cmake option "-D TOPDIR=<absolute-dir-path>" is specified,
#	<absolute-dir-path> is the one.
#	Otherwise, "${CMAKE_SOURCE_DIR}/CMakeTopdir.txt" SHOULD contain the
#	definition of top directory like;
#	    set(TOPDIR "<absolute-dir-path>")
#	For DailyBuild:
#	    Cmake option "-D TOPDIR_FILE=<absolute-file-path>" specifies
#	    the definition file instead of default one.
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
    set(CMAKE_CXX_FLAGS		"${WIN_COPT_COMMON}")
    set(CMAKE_CXX_FLAGS_DEBUG	"${WIN_COPT_DEBUG}")
    set(CMAKE_CXX_FLAGS_RELEASE	"${WIN_COPT_RELEASE}")
    set(CMAKE_CXX_FLAGS_TRACE	"${WIN_COPT_TRACE}")
    foreach(macros IN ITEMS ${WIN_COPT_MACROS})
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /D ${macros}")
    endforeach()
    set(CMAKE_EXE_LINKER_FLAGS		"${WIN_LINK_COMMON}")
    set(CMAKE_EXE_LINKER_FLAGS_DEBUG	"${WIN_LINK_DEBUG}")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE	"${WIN_LINK_RELEASE}")
    set(CMAKE_EXE_LINKER_FLAGS_TRACE	"${WIN_LINK_TRACE}")
elseif(${Linux})
    message(STATUS "Host System: Linux")
    set(LIB_DIR	"${SPR_CORE_DIR}/lib")
    set(CMAKE_CXX_FLAGS		"${LINUX_COPT_COMMON}")
    set(CMAKE_CXX_FLAGS_DEBUG	"${LINUX_COPT_DEBUG}")
    set(CMAKE_CXX_FLAGS_RELEASE	"${LINUX_COPT_RELEASE}")
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
if(${Windows})
    set(RunSwigDir ${SPR_SRC_DIR}/RunSwig)
    set(Python ${RunSwigDir}/python_adapter.bat)
    file(TO_NATIVE_PATH "${RunSwigDir}" RunSwigDir)
    file(TO_NATIVE_PATH "${Python}" Python)
    message(STATUS "Visual Studio Version: ${VS_VERSION}")
elseif(${Linux})
    set(Python python)
endif()

# ------------------------------------------------------------------------------
#  Main target.
#
list(LENGTH EXCLUDE_SRCS LEN_EXCLUDE_SRCS)
if(${LEN_EXCLUDE_SRCS} GREATER 0)
	foreach(_tmp ${EXCLUDE_SRCS})
		get_filename_component(_abspath ${_tmp} ABSOLUTE)
		message("++ exclude: ${_abspath}")
		list(REMOVE_ITEM SRCS ${_abspath})
	endforeach()
endif()
if(${Windows})
    if("${ARCHITECTURE}" STREQUAL "x64")
	add_executable(${Target} ${SRCS})
    else()
	add_executable(${Target} WIN32 ${SRCS})
    endif()
endif()
set(OUTDIR_BASE ${SPR_TOP_DIR}/generated/bin)

# ------------------------------------------------------------------------------
#  Add target to synchronize configuration with ${SRR_SRC_DIR}.
#
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

# ------------------------------------------------------------------------------
#  Define depending projects.
#
set(DEP_Base)
set(DEP_Collision Foundation)
set(DEP_Creature Foundation Framework Physics)
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
add_dependencies(RunSwig ${SYNC})
add_dependencies(${Target} ${SPR_PROJS} ${SYNC})

# ------------------------------------------------------------------------------
#  Include directories.
#
include_directories(${Target}
    PRIVATE ${CMAKE_SOURCE_DIR}
    PRIVATE ${SPR_INC_DIR}
    PRIVATE ${SPR_SRC_DIR}
    PRIVATE ${Boost_INCLUDE_DIRS}
    PRIVATE ${GLEW_INCLUDE_DIRS}
    PRIVATE ${GLUT_INCLUDE_DIR}
    PRIVATE ${ADDITIONAL_INCDIR}
    PRIVATE ${SPR_TOP_DIR}/dependency/src
)

# ------------------------------------------------------------------------------
#  Library directories.
#
if("${ARCHITECTURE}" STREQUAL "x64")
    set(DEPENDENCY_LIBDIR ${SPR_TOP_DIR}/dependency/lib/win64)
else()
    set(DEPENDENCY_LIBDIR ${SPR_TOP_DIR}/dependency/lib/win32)
endif()
target_link_directories(${Target}
    PRIVATE ${ADDITIONAL_LIBDIR}
    PRIVATE ${DEPENDENCY_LIBDIR}
)

# ------------------------------------------------------------------------------
#  Additional libraries.
#
target_link_libraries(${Target}
    ${SPR_PROJS}
    ${ADDITIONAL_LIBS}
)
# ------------------------------------------------------------------------------
#  Exclude default libraries.
#
if(DEFINED EXCLUDE_LIBS)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /NODEFAULTLIB:\"${EXCLUDE_LIBS}\"")
endif()

# ------------------------------------------------------------------------------
#  Set working directory to VS debugger.
#
set_target_properties(${Target}
    PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

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
