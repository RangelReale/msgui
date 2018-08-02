# add sources
set(SOURCES
    src/UpDownListWidget.cpp
    src/WidgetListWidget.cpp
    src/GithubRelease.cpp
    src/highlighter/HL_CPP.cpp
    src/highlighter/HL_Error.cpp
    src/internal/WidgetListWidgetViewport.cpp
    src/external/history_line_edit.cpp
    
    include/msgwidget/UpDownListWidget.h
    include/msgwidget/WidgetListWidget.h
    include/msgwidget/GithubRelease.h
    include/msgwidget/highlighter/HL_CPP.h
    include/msgwidget/highlighter/HL_Error.h
    include/msgwidget/internal/WidgetListWidgetViewport.h
    include/msgwidget/external/history_line_edit.hpp
)

add_library(msgwidget STATIC ${SOURCES})

# set this target's include directories
target_include_directories(msgwidget PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/msgwidget/external>
    $<INSTALL_INTERFACE:include>
    PRIVATE src)
    
# link dependencies 

set (LIBS
    Qt5::Widgets
	Qt5::Network
)

target_link_libraries(msgwidget
    PUBLIC
        ${LIBS}
)

add_library(msgui::msgwidget ALIAS msgwidget)

# IDE source groups
FILE(GLOB SG_ROOT include/msgwidget/* src/*.cpp)
source_group(src\\internal REGULAR_EXPRESSION internal/.*)
source_group(src\\external REGULAR_EXPRESSION external/.*)
source_group(src\\highlighter REGULAR_EXPRESSION highlighter/.*)
source_group(src FILES ${SG_ROOT})
