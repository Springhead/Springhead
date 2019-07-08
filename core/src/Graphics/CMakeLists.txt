# CMakeLists.txt for Graphics

# target
set(Target "Graphics")
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

#===============================================================
#  Define sources and headers to include this project.
#	If you specify headers under core/include directory,
#	you must add directory path explicitly. The variable
#	reference ${SPR_INC_DIR} indicates ".../core/include".
#
#	If GLOB collects unnessessary files, replace "" of the
#	last element of EXCLUDES_SRCS by those file names.
#---------------------------------------------------------------
file(GLOB SRCS *.cpp *.h)
file(GLOB COMMON_HDRS 
	${SPR_INC_DIR}/SprGraphics.h
	${SPR_INC_DIR}/${Target}/*.h
)
file(GLOB EXCLUDE_SRCS 
	GRDrawEllipsoid.h
)
#  end of definition
#===============================================================
list(APPEND SRCS ${COMMON_HDRS})
list(LENGTH EXCLUDE_SRCS LEN_EXCLUDE_SRCS)
if(${LEN_EXCLUDE_SRCS} GREATER 0)
	list(REMOVE_ITEM SRCS ${EXCLUDE_SRCS})
endif()
add_library(${Target} STATIC ${SRCS})

# output library
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

# integrate intermediate directory
#	${CMAKE_BINARY_DIR}/${Target}/${Target}.dir
#	linked to (for Windows)
#	  => ${SPR_SRC_DIR}/${Target}/${ARCHITECTURE}/${VS_VERSION}/${Target}.dir
#	or (for unix)
#	  => ${SPR_SRC_DIR}/${Target}/${ARCHITECTURE}/${Target}.dir
#
set(INTDIR ${Target}.dir)
set(ACTDIR ${SPR_SRC_DIR}/${Target}/${ARCHITECTURE})
if(${Windows})
    set(ACTDIR ${ACTDIR}/${VS_VERSION})
endif()
set(LINKLIST
    "${INTDIR}=${ACTDIR}/${INTDIR}" ;
)
set(_tmp_cmnd "python ${SPR_SRC_DIR}/cmake_preconfig.py -v ${LINKLIST}")
execute_process(
    COMMAND cmd /c ${_tmp_cmnd}
    RESULT_VARIABLE CMD_RESULT
    OUTPUT_VARIABLE CMD_OUTPUT
    ERROR_VARIABLE CMD_ERROR
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
)
if(${CMD_RESULT})
    message("Error: ${CMD_ERROR}")
endif()
if(NOT "${CMD_OUTPUT}" STREQUAL "")
    string(REPLACE "\n" "\n     " CMD_OUTPUT ${CMD_OUTPUT})
endif()
string(STRIP ${CMD_OUTPUT} CMD_OUTPUT)
message("     ${CMD_OUTPUT}")

# end: CMakeLists.txt
