include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/code
)

file(GLOB Sources code/OverlayMain.cpp)

if(64BIT)
  add_library(desuraoverlay64 SHARED ${Sources})
  install(TARGETS desuraoverlay64
          DESTINATION "${LIB_INSTALL_DIR}/overlay64/")
endif()
#target_link_libraries(desuraoverlay )

if(32BIT_SUPPORT OR NOT 64BIT)
  add_library(desuraoverlay32 SHARED ${Sources})
  if(32BIT_SUPPORT AND 64BIT)
    set_target_properties(desuraoverlay32 PROPERTIES COMPILE_FLAGS -m32)
    set_target_properties(desuraoverlay32 PROPERTIES LINK_FLAGS -m32)
  endif()
  install(TARGETS desuraoverlay32
        DESTINATION "${LIB_INSTALL_DIR}/overlay32/")
endif()
