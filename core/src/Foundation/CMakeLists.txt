# CMakeLists.txt for Foundation

# target
set(Target "Foundation")
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

# sources
#   "${Target}Stub.cpp" should always be included to source lists.
#   Trick GLOB by dummy file if it not exists.
if("${StubSrc}" STREQUAL "")
    file(WRITE ${Target}Stub.cpp "")
endif()
file(GLOB SRCS "*.cpp" ${Target}Stub.cpp)
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
if(DEFINED GLUI_INCLUDE_DIR)
    target_include_directories(${Target}
	PRIVATE ${GLUI_INCLUDE_DIR}
    )
endif()

# for Windows (Visual Studio)
#   make $(IntDir) be linked to
#	${SPR_SRC_DIR}/${Target}/${ARCHITECTURE}/${VS_VERSION}/${Target}.dir
#
if(${Windows})
    set(INTDIR "${Target}.dir")
    set(ACTDIR ${SPR_SRC_DIR}/${Target}/${ARCHITECTURE}/${VS_VERSION}/${INTDIR})
    file(TO_NATIVE_PATH "${ACTDIR}" ACTDIR)
    #message("++ ${ACTDIR} ++")
    #message("++ ${INTDIR} ++")
    execute_process(
	COMMAND cmd /c if not exist "${ACTDIR}\\" mkdir "${ACTDIR}"
	COMMAND cmd /c if exist "${INTDIR}" rd /s /q "${INTDIR}"
	RESULT_VARIABLE CMD_STATUS
	OUTPUT_VARIABLE CMD_OUTPUT
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )
    execute_process(
	COMMAND	cmd /c mklink /J "${INTDIR}" "${ACTDIR}"
	RESULT_VARIABLE CMD_STATUS
	OUTPUT_VARIABLE CMD_OUTPUT
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )
    #message(STATUS "CMD_STATUS: ${CMD_STATUS}")
    #message(STATUS "CMD_OUTPUT: ${CMD_OUTPUT}")
    #message(STATUS "${INTDIR} <<===>> ${ACTDIR}")
    unset(INTDIR)
    unset(ACTDIR)
endif()

# end: CMakeLists.txt
