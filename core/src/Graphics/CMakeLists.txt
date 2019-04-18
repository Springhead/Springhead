# CMakeLists.txt for Graphics

# target
set(Target "Graphics")
message("** ${Target} **")

# sytem environment
set(Windows "${CMAKE_HOST_SYSTEM_NAME}" STREQUAL "Windows")
set(Linux   "${CMAKE_HOST_SYSTEM_NAME}" STREQUAL "Linux")

# find Springhead top directory
set(SPR_TOP_DIR "")
if(${Windows})
	set(SPR_TOP_DIR_TEXT "C:/Windows/TEMP/CMakeTopDir.txt")
else()
	set(SPR_TOP_DIR_TEXT "/tmp/CMakeTopDir.txt")
endif()
if(EXISTS ${SPR_TOP_DIR_TEXT})
    file(READ ${SPR_TOP_DIR_TEXT} SPR_TOP_DIR)
endif()
if("${SPR_TOP_DIR}" STREQUAL "")
    # assume current directory is ".../Springhead/core/src"
    set(SPR_TOP_DIR ${CMAKE_SOURCE_DIR}/../..)
endif()
get_filename_component(SPR_TOP_DIR ${SPR_TOP_DIR} ABSOLUTE)
set(SPR_SRC_DIR ${SPR_TOP_DIR}/core/src)
set(SPR_INC_DIR ${SPR_TOP_DIR}/core/include)
#
file(TO_NATIVE_PATH "${SPR_TOP_DIR}" SPR_TOP_DIR)
file(TO_NATIVE_PATH "${SPR_SRC_DIR}" SPR_SRC_DIR)
file(TO_NATIVE_PATH "${SPR_INC_DIR}" SPR_INC_DIR)
#message(STATUS  ${Target}:SPR_TOP_DIR: [${SPR_TOP_DIR}])
#message(STATUS  ${Target}:SPR_SRC_DIR: [${SPR_SRC_DIR}])
#message(STATUS  ${Target}:SPR_INC_DIR: [${SPR_INC_DIR}])

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

# end: CMakeLists.txt
