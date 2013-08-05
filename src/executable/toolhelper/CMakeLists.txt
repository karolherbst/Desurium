include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/RES
)

if(WITH_BREAKPAD)
  set(MDUMP mdump)
else()
  set(MDUMP stackdumper)
endif()

configure_file(RES/toolhelper.rc ${RC_DIR}/toolhelper.rc @ONLY)
file(GLOB
  Sources code/*.cpp
  ${RC_DIR}/toolhelper.rc
)
				
add_executable(toolhelper WIN32 ${Sources})
target_link_libraries(toolhelper
  ${MDUMP}
  util_bootloader
)

install(TARGETS toolhelper
        DESTINATION "${BIN_INSTALL_DIR}")
