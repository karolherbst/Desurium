include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/RES
)

if(WITH_BREAKPAD)
  set(MDUMP mdump)
else()
  set(MDUMP stackdumper)
endif()

configure_file(RES/repair.rc ${RC_DIR}/repair.rc @ONLY)
file(GLOB Sources
  code/*.cpp
  ${RC_DIR}/repair.rc
)

add_definitions(
  -Dgamecore_LOG
  -DDONTUSE_PCH
  -DDESURA_EXE
  -DDESURA_CLIENT
)
				
add_executable(utilhelper WIN32 ${Sources})
target_link_libraries(utilhelper
  ${MDUMP}
  util_bootloader
)

install(TARGETS utilhelper
        DESTINATION "${BIN_INSTALL_DIR}")
