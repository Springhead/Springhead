#  makeopts.override.txt

# ------------------------------------------------------------------------------
#  At when this file is included, compiler/linker options are already set
#  by the file "CMakeOpts.txt" at the directory where cmake invoked.
#  This file modifies these options by the file "CMakeAddOpts.txt"
#  at current directory.
#
set(_override_file "${CMAKE_CURRENT_SOURCE_DIR}/CMakeAddOpts.txt")
if(EXISTS ${_override_file})
    include(${_override_file})
    if(${Windows})
	### overrides
	##override_var(CMAKE_CXX_FLAGS		WIN_COPT_COMMON)
	##override_var(CMAKE_CXX_FLAGS_DEBUG	WIN_COPT_DEBUG)
	##override_var(CMAKE_CXX_FLAGS_RELEASE	WIN_COPT_RELEASE)
	##override_var(CMAKE_CXX_FLAGS_TRACE	WIN_COPT_TRACE)
	##override_var(CMAKE_EXE_LINKER_FLAGS	    WIN_LINK_COMMON)
	##override_var(CMAKE_EXE_LINKER_FLAGS_DEBUG   WIN_LINK_DEBUG)
	##override_var(CMAKE_EXE_LINKER_FLAGS_RELEASE WIN_LINK_RELEASE)
	##override_var(CMAKE_EXE_LINKER_FLAGS_TRACE   WIN_LINK_TRACE)
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
	### overrides
	##override_var(LINUX_COPT		LINUX_COPT_REPLACE)
	##override_var(LINUX_LDFLAGS	LINUX_LDFLAGS_REPLACE)
	##override_var(INCLUDES		LINUX_INCLUDES_REPLACE)
	# prepends
	prepend(LINUX_COPT_INCDIR	LINUX_INCDIRS_PREPEND)
	prepend(LINUX_LDFLAGS		LINUX_LDFLAGS_PREPEND)
	# appends
	append(LINUX_COPT_INCDIR	LINUX_INCDIRS_APPEND)
	append(LINUX_LDFLAGS		LINUX_LDFLAGS_APPEND)
	append(LINUX_COPT_MACROS	LINUX_COPT_MACROS_APPEND)
    endif()
endif()

# end: makeopts.override.txt
