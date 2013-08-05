include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/code
)

if(WIN32)
  file(GLOB Sources code/StackDumper_win.cpp)
  set(PLATFORM_LIBRARIES )
else()
  file(GLOB Sources code/StackDumper_linux.cpp)
  set(PLATFORM_LIBRARIES bfd)
endif()

add_library(stackdumper ${Sources})
target_link_libraries(stackdumper
  ${PLATFORM_LIBRARIES}
)
