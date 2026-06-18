
add_library(DBMagr STATIC
  dbmagr.cpp
  dbmagr.h
)

target_link_libraries(DBMagr PUBLIC Qt::Core Qt::Sql)


target_include_directories(
    ModelLib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
