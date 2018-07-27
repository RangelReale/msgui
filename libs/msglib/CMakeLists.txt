# add sources
set(SOURCES
    src/Process.cpp
    src/Cmd.cpp
    src/CmdParser.cpp
    
    include/msglib/Process.h
    include/msglib/Cmd.h
    include/msglib/CmdParser.h
    include/msglib/CheckStringList.h
)

add_library(msglib STATIC ${SOURCES})

# set this target's include directories
target_include_directories(msglib PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    PRIVATE src)
    
# link dependencies 

set (LIBS
    Qt5::Widgets
#    mredit::mredit
)

target_link_libraries(msglib
    PUBLIC
        ${LIBS}
)

add_library(msgui::msglib ALIAS msglib)

# IDE source groups
FILE(GLOB SG_ROOT include/msglib/* src/*.cpp)
source_group(src FILES ${SG_ROOT})
