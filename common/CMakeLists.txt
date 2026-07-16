add_library(CommonLib STATIC
)

target_sources(CommonLib PRIVATE
    include/common.h
    include/clicklabel.h
    include/logger.h
    src/clicklabel.cpp
    src/logger.cpp
)

target_include_directories(CommonLib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(CommonLib PUBLIC
    Qt::Core
    Qt::Widgets
)