#.rst:
# Findmredit
# -------
#
# Finds the mredit library
#
# This will define the following variables:
#
#   mredit_FOUND    - True if the system has the mredit library
#   mredit_VERSION  - The version of the mredit library which was found
#
# and the following imported targets:
#
#   mredit::mredit   - The mredit library
#

# find the include path
find_path(mredit_INCLUDE_DIR
  NAMES Editor.h
  PATH_SUFFIXES mredit
)

include(FindPackageHandleStandardArgs)

# determine if should search separate debug and release libraries
set(mredit_DEBUG_AND_RELEASE OFF)
if (WIN32)
	set(mredit_DEBUG_AND_RELEASE ON)
endif()

if (mredit_DEBUG_AND_RELEASE)
	# find release library
	find_library(mredit_LIBRARY_RELEASE
	  NAMES mredit
	)

	# find debug library
	find_library(mredit_LIBRARY_DEBUG
	  NAMES mredit
	)

	# only release is required
	find_package_handle_standard_args(mredit
	  FOUND_VAR mredit_FOUND
	  REQUIRED_VARS
		mredit_LIBRARY_RELEASE
		mredit_INCLUDE_DIR
	)

else()
	# find library
	find_library(mredit_LIBRARY
	  NAMES mredit
	)

	find_package_handle_standard_args(mredit
	  FOUND_VAR mredit_FOUND
	  REQUIRED_VARS
		mredit_LIBRARY
		mredit_INCLUDE_DIR
	)
endif()

if(mredit_FOUND AND NOT TARGET mredit::mredit)
  add_library(mredit::mredit UNKNOWN IMPORTED)
  set_target_properties(mredit::mredit PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${mredit_INCLUDE_DIR}"
#    INTERFACE_COMPILE_OPTIONS "${mredit_CFLAGS_OTHER}"
  )

  if (mredit_DEBUG_AND_RELEASE)
	# release (must be first, so it is the default)
    set_property(TARGET mredit::mredit APPEND PROPERTY
      IMPORTED_CONFIGURATIONS RELEASE
    )
    set_target_properties(mredit::mredit PROPERTIES
      IMPORTED_LOCATION_RELEASE "${mredit_LIBRARY_RELEASE}"
    )

	# debug (optional)
	if (mredit_LIBRARY_DEBUG)
		set_property(TARGET mredit::mredit APPEND PROPERTY
		  IMPORTED_CONFIGURATIONS DEBUG
		)
		set_target_properties(mredit::mredit PROPERTIES
		  IMPORTED_LOCATION_DEBUG "${mredit_LIBRARY_DEBUG}"
		)
	endif()

  else()
	# only release
	set_target_properties(mredit::mredit PROPERTIES
		IMPORTED_LOCATION "${mredit_LIBRARY}"
    )	
  endif()
endif()

mark_as_advanced(
  mredit_INCLUDE_DIR
  mredit_LIBRARY
  mredit_LIBRARY_RELEASE
  mredit_LIBRARY_DEBUG
)