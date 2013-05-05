include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/code
)

file(GLOB Sources code/StackDumper.cpp)

add_library(stackdumper ${Sources})
target_link_libraries(stackdumper
  bfd
)
