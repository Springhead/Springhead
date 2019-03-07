# CMakeLists.txt for Springhead

cmake_minimum_required(VERSION 3.1)
#
set(CMAKE_CONFIGURATION_TYPES Debug Release Trace)
set(CMAKE_GENERATOR_PLATFORM x64)
set(CMAKE_INSTALL_PREFIX F:/Project/Springhead_CMAKE/core/src/build)
set(CMAKE_USE_RELATIVE_PATHS TRUE)

# current platform
set(Windows ${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows")
set(Linux   ${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Linux")

# compile options
if(${Windows})
    message(STATUS "Host System: Windows")
    set (CMAKE_CXX_FLAGS "/Zi /W3 /sdl /MP /D _LIB /GF- /EHsc /Gy- /Qpar /arch:AVX /openmp /wd4996 /wd4244 /FC")
    set (CMAKE_CXX_FLAGS_DEBUG   "/JMC /Od /Ob0 /Oy- /RTC1 /MDd /fp:except ")
    set (CMAKE_CXX_FLAGS_RELEASE "/Ob2 /Ot /Oy /Gr")
    set (CMAKE_CXX_FLAGS_TRACE   "/Ob2 /Ot /D TRACE /Oy- /Gr")
elseif(${Linux})
endif()

# some defs
set(ProjectName "Springhead")
if(${Windows})
    set(RunSwigDir ${CMAKE_SOURCE_DIR}/RunSwig)
    set(Python ${RunSwigDir}/python_adapter.bat)
    set(VS_VERSION "15.0")
elseif(${Linux})
    set(Python python)
endif()

# projects
#
project(${ProjectName} CXX)
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

# main target
#
set(Target "Springhead")
set(OUTDIR_BASE ${CMAKE_SOURCE_DIR}/../../generated/lib)
set(LIBNAME_DEBUG   ${VS_VERSION}D${CMAKE_GENERATOR_PLATFORM})
set(LIBNAME_RELEASE ${VS_VERSION}${CMAKE_GENERATOR_PLATFORM})
set(LIBNAME_TRACE   ${VS_VERSION}T${CMAKE_GENERATOR_PLATFORM})
set(LIBCMND_DEBUG   SpringheadLib.bat ${CMAKE_GENERATOR_PLATFORM} ${LIBNAME_DEBUG})
set(LIBCMND_RELEASE SpringheadLib.bat ${CMAKE_GENERATOR_PLATFORM} ${LIBNAME_RELEASE})
set(LIBCMND_TRACE   SpringheadLib.bat ${CMAKE_GENERATOR_PLATFORM} ${LIBNAME_TRACE})

# clean (only for RunSwig)
#
if ("${CMAKE_GENERATOR_PLATFORM}" STREQUAL "x64")
    set(LIBDIR ${CMAKE_SOURCE_DIR}/../../generated/lib/win64)
else ()
    set(LIBDIR ${CMAKE_SOURCE_DIR}/../../generated/lib/win32)
endif()
set(LIBPATH_DEBUG   ${LIBDIR}/Springhead${LIBNAME_DEBUG}.lib)
set(LIBPATH_RELEASE ${LIBDIR}/Springhead${LIBNAME_RELEASE}.lib)
set(LIBPATH_TRACE   ${LIBDIR}/Springhead${LIBNAME_TRACE}.lib)
set(CLEAN ${Python} ${CMAKE_SOURCE_DIR}/RunSwig/Clean.py ${CMAKE_SOURCE_DIR})

if(${Windows})
    add_custom_target(${Target} ALL
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

# end: Springhead
