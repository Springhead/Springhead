#  makeopts.override.txt

# ------------------------------------------------------------------------------
#  At when this file is included, compiler/linker options are already set by
#  the file "CMakeOpts.txt" at the directory where cmake invoked.
#  This file overrides these options by the file "CMakeOpts.txt" at current
#  directory.
#
macro(override_var output var)
    if(DEFINED ${var})
	set(${output} ${${var}})
    endif()
endmacro()
macro(append_var output var)
    if(DEFINED ${var})
	list(APPEND ${output} ${${var}})
    endif()
endmacro()

set(_override_file "${CMAKE_CURRENT_SOURCE_DIR}/CMakeOpts.txt")
if(EXISTS ${_override_file})
    include(${_override_file})
    if(${Windows})
	# overrides
	override_var(CMAKE_CXX_FLAGS		WIN_COPT_COMMON)
	override_var(CMAKE_CXX_FLAGS_DEBUG	WIN_COPT_DEBUG)
	override_var(CMAKE_CXX_FLAGS_RELEASE	WIN_COPT_RELEASE)
	override_var(CMAKE_CXX_FLAGS_TRACE	WIN_COPT_TRACE)
	override_var(CMAKE_EXE_LINKER_FLAGS	    WIN_LINK_COMMON)
	override_var(CMAKE_EXE_LINKER_FLAGS_DEBUG   WIN_LINK_DEBUG)
	override_var(CMAKE_EXE_LINKER_FLAGS_RELEASE WIN_LINK_RELEASE)
	override_var(CMAKE_EXE_LINKER_FLAGS_TRACE   WIN_LINK_TRACE)
	# appends
	append_var(CMAKE_CXX_FLAGS		WIN_COPT_COMMON_APPEND)
	append_var(CMAKE_CXX_FLAGS_DEBUG	WIN_COPT_DEBUG_APPEND)
	append_var(CMAKE_CXX_FLAGS_RELEASE	WIN_COPT_RELEASE_APPEND)
	append_var(CMAKE_CXX_FLAGS_TRACE	WIN_COPT_TRACE_APPEND)
	append_var(CMAKE_EXE_LINKER_FLAGS	  WIN_LINK_COMMON_APPEND)
	append_var(CMAKE_EXE_LINKER_FLAGS_DEBUG   WIN_LINK_DEBUG_APPEND)
	append_var(CMAKE_EXE_LINKER_FLAGS_RELEASE WIN_LINK_RELEASE_APPEND)
	append_var(CMAKE_EXE_LINKER_FLAGS_TRACE   WIN_LINK_TRACE_APPEND)
    elseif(${Linux})
	# overrides
	override_var(LINUX_COPT		LINUX_COPT_REPLACE)
	override_var(LDFLAGS.EXTRA	LINUX_LDFLAGS.EXTRA_REPLACE)
	override_var(LDFLAGS.GL		LINUX_LDFLAGS.GL_REPLACE)
	override_var(INCLUDES		LINUX_INCLUDES_REPLACE)
	# appends
	append_var(LINUX_COPT		LINUX_COPT_APPEND)
	append_var(LDFLAGS.EXTRA	LINUX_LDFLAGS.EXTRA_APPEND)
	append_var(LDFLAGS.GL		LINUX_LDFLAGS.GL_APPEND)
	append_var(ADDITIONAL_INCDIR	LINUX_INCLUDES_APPEND)
	if(DEFINED LINUX_COPT_MACROS_APPEND)
	    list(APPEND LINUX_COPT_MACROS ${LINUX_CMAKE_MACROS_APPEND})
	endif()
    endif()
endif()

# end: makeopts.override.txt
