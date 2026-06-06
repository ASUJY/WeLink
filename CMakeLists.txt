cmake_minimum_required(VERSION 3.19)
project(WeLink LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Svg Network)

set(CMAKE_AUTORCC ON)

qt_standard_project_setup()

qt_add_executable(WeLink
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    loginwidget.h loginwidget.cpp loginwidget.ui
    resource.qrc
    mainwindow.ui
    clicklabel.h clicklabel.cpp
    registerwidget.h registerwidget.cpp

    appcore.h appcore.cpp

    net/abstractnet.h
    net/tcpclient.h net/tcpclient.cpp
    net/tcpclientmediator.h net/tcpclientmediator.cpp
    init.conf
    net/communicationmediator.h
)

target_link_libraries(WeLink
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt::Svg
        Qt::Network
)

install(TARGETS WeLink
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET WeLink
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})