

add_library(ModelLib STATIC
  user.hpp
  friend.hpp
  message.hpp
  usermodel.h usermodel.cpp
)

target_include_directories(
    ModelLib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(
    ModelLib PUBLIC
    DBMagr
)