add_library(NetLib STATIC)

target_sources(
    NetLib PRIVATE
    include/abstractnet.h
    include/communicationmediator.h
    include/tcpclient.h
    include/tcpclientmediator.h
    src/tcpclient.cpp
    src/tcpclientmediator.cpp
)

target_include_directories(
    NetLib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(
    NetLib PUBLIC
    Qt::Core
    Qt::Network
)