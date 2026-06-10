

add_library(ModelLib STATIC
  user.hpp
)

target_include_directories(
    ModelLib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)