find_package(Magnum REQUIRED
  GL
  Shaders
  )

if(NOT CORRADE_TARGET_ANDROID)
  find_package(Magnum REQUIRED Sdl2Application)
else()
  find_package(Magnum REQUIRED AndroidApplication)
endif()

set_directory_properties(PROPERTIES CORRADE_USE_PEDANTIC_FLAGS ON)

if(NOT CORRADE_TARGET_ANDROID)
  add_executable(MyApplication MyApplication.cpp)
else()
  add_library(MyApplication SHARED MyApplication.cpp)
endif()

#target_link_libraries(MyApplication PRIVATE
list(APPEND LIBRARIES
  Magnum::Application
  Magnum::GL
  Magnum::Magnum
  Magnum::Shaders
  )

# ImGui

if(NOT CORRADE_TARGET_ANDROID)
  set(IMGUI_DIR ${PROJECT_SOURCE_DIR}/third_party/repos/imgui)
  set(imgui_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imguifilesystem.cpp
    )

  list(APPEND imgui_SOURCES
    ${IMGUI_DIR}/examples/sdl_opengl2_example/imgui_impl_sdl_gl2.cpp
    )
  find_package(SDL2 REQUIRED)
  include_directories(${SDL2_INCLUDE_DIR})

  add_library(imgui STATIC ${imgui_SOURCES})
  set_target_properties(imgui PROPERTIES COMPILE_FLAGS "-w") # Suppress Warnings

  include_directories(SYSTEM
    ${IMGUI_DIR}/
    ${IMGUI_DIR}/examples/sdl_opengl2_example/
    ${IMGUI_DIR}/examples/sdl_opengl3_example/
    )

  list(APPEND LIBRARIES
    imgui
    )
endif()

target_link_libraries(MyApplication PRIVATE ${LIBRARIES})
