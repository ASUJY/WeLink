cmake_minimum_required(VERSION 3.19)

# 工程名称 WeLink, 语言C++
project(WeLink LANGUAGES CXX)

# QT库最低要求6.5版本
# 需要Core模块
# Widgets模块(界面控件)
# Svg模块(Svg图形支持)
# Network模块
find_package(Qt6 6.9 REQUIRED COMPONENTS Core Widgets Svg Network Sql)

# 开启自动处理 Qt 资源文件（.qrc）
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/ui_components/forms)

add_subdirectory(model)
add_subdirectory(common)
add_subdirectory(net)
add_subdirectory(ui_components)
add_subdirectory(app)
add_subdirectory(DBMagr)