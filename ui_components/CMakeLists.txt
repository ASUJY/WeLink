aux_source_directory(./src SRC_LIST)
aux_source_directory(./src/login LOGIN_SRC_LIST)
aux_source_directory(./src/register REGISTER_SRC_LIST)


add_library(UiLib STATIC)

target_sources(UiLib PRIVATE
    include/login/loginwidget.h
    include/register/registerwidget.h
    include/mainwindow.h
    include/chatlistwidget.h
    
    ${SRC_LIST}
    ${LOGIN_SRC_LIST}
    ${REGISTER_SRC_LIST}
    
    forms/loginwidget.ui
    forms/chatlistwidget.ui
    forms/mainwindow.ui
)

target_include_directories(UiLib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/login
    ${CMAKE_CURRENT_SOURCE_DIR}/include/register
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(UiLib PUBLIC
    Qt::Core
    Qt::Widgets
    Qt::Svg
    NetLib
    CommonLib
)