
# aux_source_directory(. SRC_LIST)

qt_add_executable(${PROJECT_NAME} WIN32 MACOSX_BUNDLE
    main.cpp
    appcore.h
    appcore.cpp
    resource.qrc
    resource.qrc
)

set(CONFIG_FILE_PATH ${CMAKE_SOURCE_DIR}/net/init.conf)
# 构建成功后复制文件
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CONFIG_FILE_PATH}
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
    COMMENT "Copying init.conf to executable directory"
)


target_link_libraries(${PROJECT_NAME} PUBLIC
    Qt::Core
    Qt::Widgets
    Qt::Svg
    Qt::Network
    CommonLib
    NetLib
    UiLib
    ModelLib
    DBMagr
)

# 安装和部署（可选）
install(TARGETS ${PROJECT_NAME}
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
qt_generate_deploy_app_script(
    TARGET ${PROJECT_NAME}
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})