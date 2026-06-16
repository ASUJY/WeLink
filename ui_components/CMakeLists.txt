aux_source_directory(./src SRC_LIST)
aux_source_directory(./src/login LOGIN_SRC_LIST)
aux_source_directory(./src/register REGISTER_SRC_LIST)


add_library(UiLib STATIC
    ../app/resource.qrc)

target_sources(UiLib PRIVATE
    include/login/loginwidget.h
    include/register/registerwidget.h
    include/mainwindow.h
    include/chatpanewidget.h
    include/addfriendwindow.h
    include/contactspanewidget.h
    include/chatmainwidget.h
    include/contactsmainwidget.h
    include/contactslistviewgroup.h
    include/contactslistwidget.h
    include/contactsitem.h
    include/contactslistviewchild.h
    include/chatlistitem.h
    include/senderwidget.h
    include/label.h
    
    ${SRC_LIST}
    ${LOGIN_SRC_LIST}
    ${REGISTER_SRC_LIST}
    
    forms/loginwidget.ui
    forms/chatpanewidget.ui
    forms/mainwindow.ui
    forms/addfriendwindow.ui
    forms/contactspanewidget.ui
    forms/chatmainwidget.ui
    forms/contactsmainwidget.ui
    forms/contactslistviewgroup.ui
    forms/chatlistitem.ui
    forms/senderwidget.ui
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