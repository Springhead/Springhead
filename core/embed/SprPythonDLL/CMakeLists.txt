# CMakeLists.txt for Springhead

cmake_minimum_required(VERSION 3.1)

# ------------------------------------------------------------------------------
#  Definition of project.
#
set(ProjectName "SprPythonDLL")
message(STATUS "Project: ${ProjectName}")
project(${ProjectName} CXX)
set(CMAKE_SUPPRESS_REGENERATION true)
set(CMAKE_USE_RELATIVE_PATHS true)
#
set(OOS_BLD_DIR "build")

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
    set(TOPDIR "${CMAKE_SOURCE_DIR}/../../..")
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
    set(CMAKE_CXX_FLAGS		${WIN_COPT_COMMON})
    set(CMAKE_CXX_FLAGS_DEBUG	${WIN_COPT_DEBUG})
    set(CMAKE_CXX_FLAGS_RELEASE	${WIN_COPT_RELEASE})
    set(CMAKE_CXX_FLAGS_TRACE	${WIN_COPT_TRACE})
    foreach(macros IN ITEMS ${WIN_COPT_MACROS})
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /D ${macros}")
    endforeach()
    set(CMAKE_SHARED_LINKER_FLAGS	  ${WIN_LINK_COMMON})
    set(CMAKE_SHARED_LINKER_FLAGS_DEBUG	  ${WIN_LINK_DEBUG})
    set(CMAKE_SHARED_LINKER_FLAGS_RELEASE ${WIN_LINK_RELEASE})
    set(CMAKE_SHARED_LINKER_FLAGS_TRACE	  ${WIN_LINK_TRACE})
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
    set(RunSwigDir ${SPR_SRC_DIR}/RunSwig)
    set(Python ${RunSwigDir}/python_adapter.bat)
    file(TO_NATIVE_PATH "${RunSwigDir}" RunSwigDir)
    file(TO_NATIVE_PATH "${Python}" Python)
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
#  Main target.
#
set(Target ${ProjectName})
if(${Windows})
    set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT ${ProjectName})
endif()

set(SRCS SprPythonDLL.cpp)
add_library(${Target} SHARED ${SRCS})

set(INSTALL_DIRECTORY_BASE ${SPR_TOP_DIR}/generated/bin)
if ("${CMAKE_GENERATOR_PLATFORM}" STREQUAL "x64")
    set(LIBRARY_INSTALL_DIRECTORY ${INSTALL_DIRECTORY_BASE}/win64)
else ()
    set(LIBRARY_INSTALL_DIRECTORY ${INSTALL_DIRECTORY_BASE}/win32)
endif()
set(LIBRARY_OUTPUT_DIRECTORY  ${LIBRARY_INSTALL_DIRECTORY}/Python)
set(LIBNAME_DEBUG   SprD)
set(LIBNAME_RELEASE Spr)
set(LIBNAME_TRACE   SprT)

set_target_properties(${Target} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG   ${LIBRARY_OUTPUT_DIRECTORY}
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${LIBRARY_OUTPUT_DIRECTORY}
    RUNTIME_OUTPUT_DIRECTORY_TRACE   ${LIBRARY_OUTPUT_DIRECTORY}
    RUNTIME_OUTPUT_NAME_DEBUG   ${LIBNAME_DEBUG}
    RUNTIME_OUTPUT_NAME_RELEASE ${LIBNAME_RELEASE}
    RUNTIME_OUTPUT_NAME_TRACE   ${LIBNAME_TRACE}
    SUFFIX ".pyd"
)

# ------------------------------------------------------------------------------
#  Add target to synchronize configuration with ${SRR_SRC_DIR}.
#
set(SPR_PROJS Base Collision Creature FileIO Foundation Framework Graphics
	      HumanInterface Physics)
set(SPR_PROJS_PLUS ${SPR_PROJS} EmbPython)
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
set(DEP_EmbPython EmbPython_RunSwig)
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
)

# ------------------------------------------------------------------------------
#  Additional libraries.
#
target_link_libraries(${Target}
    ${SPR_PROJS_PLUS}
    #${ADDITIONAL_LIBS}
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
    #PRIVATE ${ADDITIONAL_LIBDIR}
    PRIVATE ${DEPENDENCY_LIBDIR}
)

# ------------------------------------------------------------------------------
#  Install.
#
if(${Windows})
    add_custom_target(${ProjectName}_INSTALL ALL
	COMMAND if "'$(Configuration)'" equ "'Debug'"   copy ${LIBNAME_DEBUG}.pyd   ..
	COMMAND if "'$(Configuration)'" equ "'Release'" copy ${LIBNAME_RELEASE}.pyd ..
	COMMAND if "'$(Configuration)'" equ "'Trace'"   copy ${LIBNAME_TRACE}.pyd   ..
	WORKING_DIRECTORY ${LIBRARY_OUTPUT_DIRECTORY}
	DEPENDS ${Target}
	COMMENT [[ install .pyd ]]
    )
endif()

# end: CMakeLists.txt
