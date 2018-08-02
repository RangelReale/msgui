include_directories(include)

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/src/resources/version.h.in
  ${CMAKE_CURRENT_BINARY_DIR}/version.h
  @ONLY)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

if (APPLE)
    set(MACOSX_BUNDLE_BUNDLE_NAME MSGUI)
    set(MACOSX_BUNDLE_GUI_IDENTIFIER "com.rangelreale.msgui")
    set(MACOSX_BUNDLE_INFO_STRING "MSGUI - Metashell GUI")
    set(MACOSX_BUNDLE_ICON_FILE "msgui_icon.icns")
    set(MACOSX_BUNDLE_LONG_VERSION_STRING "${MSGUI_NUMBER}.${MSGUI_VERSION}.${MSGUI_BUILD_NUMBER}")
    set(MACOSX_BUNDLE_SHORT_VERSION_STRING ${MACOSX_BUNDLE_LONG_VERSION_STRING})
    set(MACOSX_BUNDLE_BUNDLE_VERSION ${MACOSX_BUNDLE_LONG_VERSION_STRING})
endif()

set(SOURCES
    src/main.cpp
    src/Application.cpp
    src/LogConfig.cpp
    src/MainWindow.cpp
    src/Settings.cpp
    src/ProjectSettings.cpp
    src/LogWindow.cpp
    src/Project.cpp
    src/CodeDialog.cpp
    src/Frame.cpp
    src/Backtrace.cpp
    src/Callgraph.cpp
    src/Log.cpp
    src/Error.cpp
    src/TemplateKind.cpp
    src/StartupCode.cpp
    src/TabEditor.cpp
    src/Util.cpp
    src/NewReleaseWindow.cpp

    include/msgui/interfaces.h
    include/msgui/MainWindow.h
    include/msgui/Application.h
    include/msgui/LogConfig.h
    include/msgui/Settings.h
    include/msgui/ProjectSettings.h
    include/msgui/LogWindow.h
    include/msgui/Project.h
    include/msgui/CodeDialog.h
    include/msgui/Frame.h
    include/msgui/Backtrace.h
    include/msgui/Callgraph.h
    include/msgui/Log.h
    include/msgui/Error.h
    include/msgui/TemplateKind.h
    include/msgui/StartupCode.h
    include/msgui/TabEditor.h
    include/msgui/Util.h
    include/msgui/NewReleaseWindow.h

    src/resources/resources.qrc
    src/resources/resources.rc
    ${CMAKE_CURRENT_BINARY_DIR}/version.h
    
    ../TODO.txt
)

if (APPLE)
    SET(SOURCES ${SOURCES} src/resources/msgui_icon.icns)
endif()

add_executable(msgui WIN32 MACOSX_BUNDLE
    ${SOURCES}
)

target_link_libraries(msgui 
	msgui::msglib 
	msgui::msgwidget
	Qt5::Widgets 
	Qt5::Xml
	Qt5::Network
	mredit::mredit
	Log4Qt::Log4Qt
)

if (APPLE)
    SET_SOURCE_FILES_PROPERTIES(src/resources/msgui_icon.icns
        PROPERTIES
        MACOSX_PACKAGE_LOCATION Resources
    )
    set_target_properties(
        msgui
        PROPERTIES
        RESOURCE "src/resources/msgui_icon.icns"
    )
endif()

if (NOT APPLE)
    install(TARGETS msgui DESTINATION bin)
endif()

# IDE source groups
FILE(GLOB SG_ROOT include/msgui/* src/*.cpp)
#source_group(src\\internal REGULAR_EXPRESSION internal/.*)
source_group(src FILES ${SG_ROOT})

OPTION(ENABLE_INSTALLER "Generate installer" OFF)

if (ENABLE_INSTALLER)
  if (WIN32)
      include(WinDeployQt)
      WinDeployQt(TARGET msgui)

      set(CMAKE_INSTALL_UCRT_LIBRARIES TRUE) # MSVC2015+
      include(InstallRequiredSystemLibraries)

      install(DIRECTORY "${CMAKE_BINARY_DIR}/windeployqt/" DESTINATION "bin")
  endif()

  if (APPLE)
    include(MacDeployQt)
    MacDeployQt(TARGET msgui)
  endif()

  set (CPACK_PACKAGE_NAME "MSGUI")
  set (CPACK_PACKAGE_VENDOR "Rangel Reale")
  set (CPACK_PACKAGE_DESCRIPTION_SUMMARY "MSGUI - Metashell GUI")
  set (CPACK_PACKAGE_VERSION_MAJOR "${MSGUI_NUMBER}")
  set (CPACK_PACKAGE_VERSION_MINOR "${MSGUI_VERSION}")
  set (CPACK_PACKAGE_VERSION_PATCH "${MSGUI_BUILD_NUMBER}")
  set (CPACK_PACKAGE_INSTALL_DIRECTORY "MSGUI")
  set (CPACK_RESOURCE_FILE_LICENSE ${CMAKE_SOURCE_DIR}/LICENSE.txt)

  SET(CPACK_PACKAGE_EXECUTABLES "msgui;MSGUI - Metashell GUI") 

  if (WIN32)
	SET(CPACK_NSIS_INSTALLED_ICON_NAME "bin\\\\msgui.exe")
	SET(CPACK_NSIS_DISPLAY_NAME "MSGUI - Metashell GUI")
  endif()

  include(CPack)

endif()
