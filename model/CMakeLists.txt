

add_library(ModelLib STATIC
  user.hpp
  friend.hpp
  message.hpp
)

target_include_directories(
    ModelLib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)