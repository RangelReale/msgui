#
# CMake wrapper to call macdeployqt in OSX
#

function(MacDeployQt)
	cmake_parse_arguments(_deploy
		"COMPILER_RUNTIME;FORCE"
		"TARGET;DEPEND_TARGET"
		"INCLUDE_MODULES;EXCLUDE_MODULES"
		${ARGN}
		)

	if(NOT _deploy_TARGET)
		message(FATAL_ERROR "A TARGET must be specified")
	endif()
	if(NOT _deploy_DEPEND_TARGET)
		set(_deploy_DEPEND_TARGET ${_deploy_TARGET})
	endif()

	foreach(mod ${_deploy_INCLUDE_MODULES})
		string(TOLOWER ${mod} mod)
		string(REPLACE "qt5::" "" mod ${mod})
		list(APPEND _ARGS "--${mod}")
	endforeach()
	foreach(mod ${_deploy_EXCLUDE_MODULES})
		string(TOLOWER ${mod} mod)
		string(REPLACE "qt5::" "" mod ${mod})
		list(APPEND _ARGS "--no-${mod}")
	endforeach()

	find_program(_deploy_PROGRAM macdeployqt
		PATHS $ENV{QTDIR}/bin/)
	if(_deploy_PROGRAM)
		message(STATUS "Found ${_deploy_PROGRAM}")
	else()
		message(FATAL_ERROR "Unable to find macdeployqt")
	endif()

	add_custom_target(macdeployqt ALL ${_deploy_PROGRAM} ${_ARGS}
		$<TARGET_FILE:${_deploy_TARGET}>
		DEPENDS ${_deploy_DEPEND_TARGET}
		COMMENT "Preparing Qt runtime dependencies")
endfunction()


