# CMakeLists.txt for Springhead

cmake_minimum_required(VERSION 3.1)
#
# projects
set(ProjectName "Springhead")
message(STATUS "Project: ${ProjectName}")
project(${ProjectName} CXX)
set(CMAKE_SUPPRESS_REGENERATION true)

# sytem environment
set(CMAKE_MAKE_SYSTEM_PATH ${CMAKE_SOURCE_DIR}/../make.system.cmake)
include(${CMAKE_MAKE_SYSTEM_PATH})
message(STATUS "Architecture: ${ARCHITECTURE}")

set(CMAKE_CONFIGURATION_TYPES Debug Release Trace)
set(CMAKE_GENERATOR_PLATFORM ${ARCHITECTURE})
set(CMAKE_USE_RELATIVE_PATHS TRUE)

# function definitions
set(CMAKE_MAKE_FUNC_PATH ${CMAKE_SOURCE_DIR}/../make.func.cmake)
include(${CMAKE_MAKE_FUNC_PATH})

# compile options
if(${Windows})
    message(STATUS "Host System: Windows")
    set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT ${ProjectName})
    set(CMAKE_CXX_FLAGS "/Zi /W3 /sdl /MP /D _LIB /GF- /EHsc /Gy- /Qpar \
			 /arch:AVX /openmp /wd4996 /wd4244 /FC")
    set(CMAKE_CXX_FLAGS_DEBUG   "/JMC /Od /Ob0 /Oy- /RTC1 /MDd /fp:except ")
    set(CMAKE_CXX_FLAGS_RELEASE "/Ob2 /Ot /Oy /Gr")
    set(CMAKE_CXX_FLAGS_TRACE   "/Ob2 /Ot /D TRACE /Oy- /Gr")
else()
    set(TOP_DIR	${CMAKE_SOURCE_DIR})
    if(${Linux})
	message(STATUS "Host System: Linux")
	set(LDFLAGS.EXTRA "-L${TOP_DIR}/lib/linux/ -L/usr/lib \
		-L/usr/X11R6/lib -L/usr/lib/X11")
	set(LDFLAGS.GL "-lX11 -lXi -lXmu -lglui -lglut -lGLEW -lGLU -lGL -lm \
		-ldl -lboost_regex")
	set(INCLUDES "-I/usr/X11R6/include/ -I/usr/X11/include")
	set(ARFLAGS  "r")
    elseif(${Cygwin})
	message(STATUS "Host System: Cygwin")
    elseif(${MacOS})
	message(STATUS "Host System: MacOS")
    endif()
    set(WARNING_FLAGS "-Wall \
	-Wno-unused-variable \
	-Wno-unused-but-set-variable \
	-Wno-unused-result \
	-Wno-unused-value \
	-Wno-unused-local-typedefs \
	-Wno-unused-function \
	-Wno-reorder \
	-Wno-sign-compare \
	-Wno-unknown-pragmas")
    set(CMAKE_CXX_FLAGS	"${WARNING_FLAGS} -fPIC")
    set(CMAKE_CXX_FLAGS_DEBUG	"-g")
    set(CMAKE_CXX_FLAGS_RELEASE "-O2")
    set(LDFLAGS "-L${TOP_DIR}/src/Base/ \
	-L${TOP_DIR}/src/Collision/ \
	-L${TOP_DIR}/src/Creature/ \
	-L${TOP_DIR}/src/FileIO/ \
	-L${TOP_DIR}/src/Foundation/ \
	-L${TOP_DIR}/src/Framework/ \
	-L${TOP_DIR}/src/Graphics/ \
	-L${TOP_DIR}/src/HumanInterface/ \
	-L${TOP_DIR}/src/Physics/ \
	-lBase -lCollision -lCreature -lFileIO -lFoundation -lFramework \
	-lGraphics -lHumanInterface -lPhysics
	${LDFLAGS.EXTRA} \
	${LDFLAGS.GL}")
    set(INCLUDES "${INCLUDES} -I${TOP_DIR}/include -I${TOP_DIR}/src -I.")
endif()

# some defs
if(${Windows})
    set(RunSwigDir ${CMAKE_SOURCE_DIR}/RunSwig)
    set(Python ${RunSwigDir}/python_adapter.bat)
    set(VS_VERSION "15.0")
    message(STATUS "Visual Studio Version: ${VS_VERSION}")
elseif(${Linux})
    set(Python python)
elseif(${Cygwin})
elseif(${MacOS})
endif()

# depending projects
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
