# CMakeLists.txt for Physics

# target
set(Target "EmbPython")
message("** ${Target} **")

# sytem environment
set(Windows "${CMAKE_HOST_SYSTEM_NAME}" STREQUAL "Windows")
set(Linux   "${CMAKE_HOST_SYSTEM_NAME}" STREQUAL "Linux")

# find Springhead top directory
if("${TOPDIR}" STREQUAL "")
    set(TOPDIR "${CMAKE_SOURCE_DIR}/../..")
endif()
set(SPR_TOP_DIR  "${TOPDIR}")
set(SPR_SRC_DIR  "${TOPDIR}/core/src")
set(SPR_INC_DIR  "${TOPDIR}/core/include")
get_filename_component(TOPDIR ${TOPDIR} ABSOLUTE)
get_filename_component(SPR_TOP_DIR ${SPR_TOP_DIR} ABSOLUTE)
get_filename_component(SPR_SRC_DIR ${SPR_SRC_DIR} ABSOLUTE)
get_filename_component(SPR_INC_DIR ${SPR_INC_DIR} ABSOLUTE)
#message(STATUS "${Target}: SPR_TOP_DIR: [${SPR_TOP_DIR}]")
#message(STATUS "${Target}: SPR_SRC_DIR: [${SPR_SRC_DIR}]")
#message(STATUS "${Target}: SPR_INC_DIR: [${SPR_INC_DIR}]")

#===============================================================================
#  Define sources.
#	To ensure that GLOB to work, make nessesary (empty) files temporarily
#	which EmbPython_RunSwig will create later at build time.
#
set(SPR_PROJS Base Collision Creature FileIO Foundation
	      Framework Graphics HumanInterface Physics)
foreach(_tmp ${SPR_PROJS})
	file(TOUCH "EP${_tmp}.h" "EP${_tmp}.cpp")
endforeach()
file(GLOB SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
		*.i *.h *.cpp
	 	Utility/*.i Utility/*.cpp
		../../include/EmbPython/*.h
		../../include/EmbPython/Utility/*.h
		../Foundation/UTCriticalSection.cpp)
list(REMOVE_ITEM SRCS ../SprPythonDLL.cpp)		# Exclude
#  end of definition

#===============================================================================
#  Load local compile options.
#
set(OPTS_FILE_NAME "CMakeOpts.txt")
set(OPTS_FILE "${CMAKE_CURRENT_SOURCE_DIR}/${OPTS_FILE_NAME}")
if(EXISTS ${OPTS_FILE})
	include(${OPTS_FILE})
	set(CMAKE_CXX_FLAGS		${WIN_COPT_COMMON})
	set(CMAKE_CXX_FLAGS_DEBUG	${WIN_COPT_DEBUG})
	set(CMAKE_CXX_FLAGS_RELEASE	${WIN_COPT_RELEASE})
	set(CMAKE_CXX_FLAGS_TRACE	${WIN_COPT_TRACE})
	foreach(macros IN ITEMS ${WIN_COPT_MACROS})
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /D ${macros}")
	endforeach()
endif()

#===============================================================================
#  Main target.
#
add_library(${Target} STATIC ${SRCS})
	

# output library
#
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

# include directories
#
set(CMAKE_USE_RELATIVE_PATHS TRUE)
target_include_directories(${Target}
    PRIVATE ${SPR_INC_DIR}/Python
    PRIVATE ${SPR_INC_DIR}
    PRIVATE ${SPR_SRC_DIR}
    PRIVATE ${SPR_TOP_DIR}/dependency/src
)
if(DEFINED Boost_INCLUDE_DIRS)
    target_include_directories(${Target}
	PRIVATE ${Boost_INCLUDE_DIRS}
    )
endif()
if(DEFINED GLEW_INCLUDE_DIRS)
    target_include_directories(${Target}
	PRIVATE ${GLEW_INCLUDE_DIRS}
    )
endif()
if(DEFINED GLUT_INCLUDE_DIR)
    target_include_directories(${Target}
	PRIVATE ${GLUT_INCLUDE_DIR}
    )
endif()
if(DEFINED ADDITIONAL_INCDIR)
    target_include_directories(${Target}
	PRIVATE ${ADDITIONAL_INCDIR}
    )
endif()

# ------------------------------------------------------------------------------
#  Add target to generate EmbPython sources.
#
set(EmbPython_RunSwig "EmbPython_RunSwig")
if(${Windows})
    add_custom_target(${EmbPython_RunSwig} ALL
	COMMAND python EmbPython_clean.py
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
