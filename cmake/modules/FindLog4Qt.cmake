#.rst:
# FindLog4Qt
# -------
#
# Finds the Log4Qt library
#
# This will define the following variables:
#
#   Log4Qt_FOUND    - True if the system has the Log4Qt library
#   Log4Qt_VERSION  - The version of the Log4Qt library which was found
#
# and the following imported targets:
#
#   Log4Qt::Log4Qt   - The Log4Qt library
#

SET(Log4Qt_STATIC OFF CACHE BOOL "Whether to use a static library")

# find the include path
find_path(Log4Qt_INCLUDE_DIR
  NAMES log4qt.h
  PATH_SUFFIXES log4qt
)

include(FindPackageHandleStandardArgs)

# determine if should search separate debug and release libraries
set(Log4Qt_DEBUG_AND_RELEASE OFF)
if (WIN32)
	set(Log4Qt_DEBUG_AND_RELEASE ON)
endif()

if (Log4Qt_DEBUG_AND_RELEASE)
	# find release library
	find_library(Log4Qt_LIBRARY_RELEASE
	  NAMES log4qt
	)

	# find debug library
	find_library(Log4Qt_LIBRARY_DEBUG
	  NAMES log4qt_d
	)

	# only release is required
	find_package_handle_standard_args(Log4Qt
	  FOUND_VAR Log4Qt_FOUND
	  REQUIRED_VARS
		Log4Qt_LIBRARY_RELEASE
		Log4Qt_INCLUDE_DIR
	)

else()
	# find library
	find_library(Log4Qt_LIBRARY
	  NAMES log4qt
	)

	find_package_handle_standard_args(Log4Qt
	  FOUND_VAR Log4Qt_FOUND
	  REQUIRED_VARS
		Log4Qt_LIBRARY
		Log4Qt_INCLUDE_DIR
	)
endif()

if(Log4Qt_FOUND AND NOT TARGET Log4Qt::Log4Qt)
  set(CDEFS "")
  if (Log4Qt_STATIC)
    list(APPEND CDEFS LOG4QT_STATIC)
  endif()

  add_library(Log4Qt::Log4Qt UNKNOWN IMPORTED)
  set_target_properties(Log4Qt::Log4Qt PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${Log4Qt_INCLUDE_DIR}"
    INTERFACE_COMPILE_DEFINITIONS "${CDEFS}"
#    INTERFACE_COMPILE_OPTIONS "${Log4Qt_CFLAGS_OTHER}"
  )

  if (Log4Qt_DEBUG_AND_RELEASE)
	# release (must be first, so it is the default)
    set_property(TARGET Log4Qt::Log4Qt APPEND PROPERTY
      IMPORTED_CONFIGURATIONS RELEASE
    )
    set_target_properties(Log4Qt::Log4Qt PROPERTIES
      IMPORTED_LOCATION_RELEASE "${Log4Qt_LIBRARY_RELEASE}"
    )

	# debug (optional)
	if (Log4Qt_LIBRARY_DEBUG)
		set_property(TARGET Log4Qt::Log4Qt APPEND PROPERTY
		  IMPORTED_CONFIGURATIONS DEBUG
		)
		set_target_properties(Log4Qt::Log4Qt PROPERTIES
		  IMPORTED_LOCATION_DEBUG "${Log4Qt_LIBRARY_DEBUG}"
		)
	endif()

  else()
	# only release
	set_target_properties(Log4Qt::Log4Qt PROPERTIES
		IMPORTED_LOCATION "${Log4Qt_LIBRARY}"
    )	
  endif()
endif()

mark_as_advanced(
  Log4Qt_INCLUDE_DIR
  Log4Qt_LIBRARY
  Log4Qt_LIBRARY_RELEASE
  Log4Qt_LIBRARY_DEBUG
)