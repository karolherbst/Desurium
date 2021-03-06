include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/code
)

file(GLOB Sources
  code/main.cpp
  code/CheckMCF.cpp
  code/CreateMCF.cpp
  code/MCFSetup.cpp
  code/PatchMCF.cpp
  code/SaveMCF.cpp
  code/TestMCF.cpp
  code/UtilFunction.cpp)

if(UNIX)
  set(PLATFORM_LIBRARIES dl)
endif()

add_executable(mcf_util ${Sources})
target_link_libraries(mcf_util
  mcfcore
  umcf
  util
  ${TINYXML_LIBRARIES}
  ${CMAKE_THREAD_LIBS_INIT}
  ${PLATFORM_LIBRARIES}
)

if(WIN32)
  SetSharedRuntime(mcf_util)
endif()

install_tool(mcf_util)
