include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/code
  ${CMAKE_CURRENT_SOURCE_DIR}/RES
)

if(WITH_BREAKPAD)
  set(MDUMP mdump)
else()
  set(MDUMP stackdumper)
endif()

configure_file(RES/service_exe.rc ${RC_DIR}/service_exe.rc @ONLY)
file(GLOB
  Sources code/*.cpp
  ${RC_DIR}/service_exe.rc
)

add_executable(desura_service ${Sources} ${COMMON_SOURCE})
target_link_libraries(desura_service
  ${MDUMP}
  Psapi
  wintrust
)

install(TARGETS desura_service
        DESTINATION "${BIN_INSTALL_DIR}")
