aux_source_directory(./src SRC_LIST)
aux_source_directory(./src/login LOGIN_SRC_LIST)
aux_source_directory(./src/register REGISTER_SRC_LIST)


add_library(UiLib STATIC
#../app/resource.qrc
    )

target_sources(UiLib PRIVATE
    include/login/loginwidget.h
    include/register/registerwidget.h
    include/mainwindow.h
    include/chatwidget.h
    include/contactswidget.h
    include/addfriendwindow.h
    include/contactsitem.h
    include/chatlistitem.h
    include/label.h
    include/contactstreemodel.h
    include/contactsitemdelegate.h
    include/contactstreeview.h
    include/searchlineedit.h
    include/chatpanelistwidget.h
    include/chatmsglistmodel.h
    include/chatmsgdelegate.h
    include/chatmsglistview.h

    ${SRC_LIST}
    ${LOGIN_SRC_LIST}
    ${REGISTER_SRC_LIST}
    
    forms/loginwidget.ui
    forms/mainwindow.ui
    forms/addfriendwindow.ui
    forms/chatlistitem.ui


    forms/chatwidget.ui
    forms/contactswidget.ui
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
    ModelLib
)