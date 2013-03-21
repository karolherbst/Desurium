include_directories(
  ${GLOBAL_INCLUDE_DIRS}/cef_desura_includes
  ${CEF_INCLUDE_DIRS}
  ${CHROMIUM_SOURCE_DIR}/src
  ${GTK2_INCLUDE_DIRS}
)

file(GLOB Sources
  code/ChromiumBrowser.cpp
  code/Cookie.cpp
  code/JavaScriptExtender.cpp
  code/JavaScriptObject.cpp
  code/SchemeExtender.cpp
  code/SchemeRequest.cpp
  code/ChromiumBrowserEvents.cpp
  code/JavaScriptContext.cpp
  code/JavaScriptFactory.cpp
  code/MenuInfo.cpp
  code/SchemePost.cpp
)

add_definitions(-DBUILDING_CEF_SHARED -DIGNORE_C_API -std=c++98)

add_library(cef_desura SHARED ${Sources})
target_link_libraries(cef_desura
  ${CEF_LIBRARIES}
)

install(TARGETS cef_desura
        DESTINATION "${LIB_INSTALL_DIR}")
