# CMakeLists.txt for Collision

# target
set(Target "Collision")
message("** ${Target} **")

# sytem environment
if("$ENV{SPR_TOP_DIR}" STREQUAL "")
    # assume current directory is ".../Springhead/core/src"
    set(SPR_TOP_DIR ${CMAKE_SOURCE_DIR}/../..)
else()
    set(SPR_TOP_DIR $ENV{SPR_TOP_DIR})
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
FILE(GLOB SRCS "*.cpp")
add_library(${Target} STATIC ${SRCS})

# output library
if(${Windows})
    set_target_properties("${Target}" PROPERTIES
	OUTPUT_NAME_DEBUG   "${Target}${VS_VERSION}D${CMAKE_GENERATOR_PLATFORM}"
	OUTPUT_NAME_RELEASE "${Target}${VS_VERSION}${CMAKE_GENERATOR_PLATFORM}"
	OUTPUT_NAME_TRACE   "${Target}${VS_VERSION}T${CMAKE_GENERATOR_PLATFORM}"
	ARCHIVE_OUTPUT_DIRECTORY_DEBUG   ${PROJECT_SOURCE_DIR}/${Target}
	ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/${Target}
	ARCHIVE_OUTPUT_DIRECTORY_TRACE   ${PROJECT_SOURCE_DIR}/${Target}
    )
elseif(${Linux})
endif()

# include directories
set(CMAKE_USE_RELATIVE_PATHS TRUE)
target_include_directories(${Target}
    PRIVATE ${SPR_INC_DIR}
    PRIVATE ${SPR_SRC_DIR}
    PRIVATE ${SPR_TOP_DIR}/dependency/src
)

# end: CMakeLists.txt
