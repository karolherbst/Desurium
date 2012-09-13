include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/code
  ${TINYXML_INCLUDE_DIR}
)

file(GLOB Sources code/*.cpp )

add_executable(${BRANDING_PRODUCT_NAME} ${Sources})
target_link_libraries(${BRANDING_PRODUCT_NAME}
  dl
  mdump
  rt
  sqlite3x
  util
  ${TINYXML_LIBRARIES}
  ${BZIP2_LIBRARIES}
  ${CURL_LIBRARIES}
  ${BREAKPAD_EXCEPTION_HANDLER_LIBRARIES}
  ${SQLITE_LIBRARIES}
  ${wxWidgets_LIBRARIES}
  ${LIBNOTIFY_LIBRARIES}
)

install(TARGETS ${BRANDING_PRODUCT_NAME}
        DESTINATION "${LIB_INSTALL_DIR}")
