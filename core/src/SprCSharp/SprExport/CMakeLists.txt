# CMakeLists.txt for SprExport
#
set(SPR_EXPORT_VERSION 1.1.0)		# 2021/08/18 バージョン管理開始

# ------------------------------------------------------------------------------
#  Set target name.
#
set(Target "SprExport")
message("** ${Target} **")

# ------------------------------------------------------------------------------
#  Read setting parameters.
#
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/CMakeSettings.txt")
    include("${CMAKE_CURRENT_SOURCE_DIR}/CMakeSettings.txt")
endif()

# ------------------------------------------------------------------------------
#  Read compiler/linker option file (try to find in following order).
#
set(OPTS_FILE_NAME "CMakeOpts.txt")
set(OPTS_FILE "${CMAKE_CURRENT_SOURCE_DIR}/${OPTS_FILE_NAME}")
set(OPTS_DEFAULT_FILE "${SPR_SRC_DIR}/${OPTS_FILE_NAME}.dist")
include_file(OPTS ${OPTS_FILE} ${OPTS_DEFAULT_FILE})

# ------------------------------------------------------------------------------
#  Output directory.
#
set(Vers ${VS_VERSION})
set(Plat ${ARCHITECTURE})
set(Conf $<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>$<$<CONFIG:Trace>:Trace>)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${SPRCSHARP}/${Vers}/bin/${Plat}/${Conf})

# ------------------------------------------------------------------------------
#  Source files
#
set(SRCS CSUtility.h CSUtility.cpp wrapper.cpp module.wrapper.cpp)
set_source_files_properties(module.wrapper.cpp
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    PROPERTIES GENERATED TRUE
)
foreach(_proj ${SPR_PROJS})
    set(_name CS${_proj}.cpp)
    set(SRCS ${SRCS} ${_name})
    #  These files will be generated at runtime.
    set_source_files_properties(${_name}
	DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	PROPERTIES GENERATED TRUE
    )
endforeach()

# ------------------------------------------------------------------------------
#  Main target
#
add_library(${Target} SHARED
    ${SRCS}
)
message("** SRCS ${SRCS}")

target_include_directories(${Target} BEFORE
    PRIVATE ${SPR_INC_DIR} ${SPR_SRC_DIR} ${SPR_TOP_DIR}/dependency/src
)
target_link_directories(${Target}
    PRIVATE ${SPR_TOP_DIR}/dependency/lib/win64
    PRIVATE ${SPR_TOP_DIR}/dependency/lib/win32
)
set(_libs ${Empty})
foreach(_proj ${SPR_PROJS})
    set(_lib "${SPR_SRC_DIR}/${_proj}/${_proj}${VS_VERSION}${ARCHITECTURE}.lib")
    file(TO_NATIVE_PATH ${_lib} _lib)
    target_link_libraries(${Target} PRIVATE ${_lib})
    message("++ add library ${_lib}")
endforeach()

# ------------------------------------------------------------------------------
#  Compiler/linker options.
#
if(Windows)
    set(CMAKE_CXX_FLAGS		"${COMP_FLAGS}")
    set(CMAKE_CXX_FLAGS_DEBUG	"${COMP_FLAGS_DEBUG}")
    set(CMAKE_CXX_FLAGS_RELEASE	"${COMP_FLAGS_RELEASE}")
    set(CMAKE_CXX_FLAGS_TRACE	"${COMP_FLAGS_TRACE}")
    set(CMAKE_SHARED_LINKER_FLAGS		"${LINK_FLAGS}")
    set(CMAKE_SHARED_LINKER_FLAGS_DEBUG		"${LINK_FLAGS_DEBUG}")
    set(CMAKE_SHARED_LINKER_FLAGS_RELEASE	"${LINK_FLAGS_RELEASE}")
    set(CMAKE_SHARED_LINKER_FLAGS_TRACE		"${LINK_FLAGS_TRACE}")
elseif(unix)
    foreach(_flag ${COMP_FLAGS})
        target_compile_options(${Target} PUBLIC ${_flag})
    endforeach()
endif()
foreach(_macro ${DEFINE_MACROS})
        target_compile_definitions(${Target} PUBLIC ${_macro})
endforeach()
#
#  Parent needs these variables to be set.
set(CMAKE_SHARED_LINKER_FLAGS_TRACE ${CMAKE_SHARED_LINKER_FLAGS_TRACE} PARENT_SCOPE)

# end: CMakeLists.txt
