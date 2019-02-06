find_package(Magnum REQUIRED
  GL
  Shaders
  Trade
  SceneGraph
  MeshTools
  Trade
  )
list(APPEND LIBRARIES
  Magnum::Application
  Magnum::GL
  Magnum::Magnum
  Magnum::Shaders
  Magnum::Trade
  Magnum::SceneGraph
  Magnum::MeshTools
  )

if(NOT CORRADE_TARGET_ANDROID)
  find_package(Magnum REQUIRED Sdl2Application)
else()
  find_package(Magnum REQUIRED AndroidApplication)
endif()

set_directory_properties(PROPERTIES CORRADE_USE_PEDANTIC_FLAGS ON)

list(APPEND SOURCES
  main.cpp
  Application.cpp
  Ui.cpp
  DirectoryWatcher.cpp
  GuiGfxPlane.cpp
  ImageManager.cpp
  #VoronoiVisualizer.cpp
  )

if(NOT CORRADE_TARGET_ANDROID)
  add_executable(MyApplication ${SOURCES})
else()
  add_library(MyApplication SHARED ${SOURCES})
endif()

set_target_properties(MyApplication PROPERTIES COMPILE_FLAGS "-w -Wunused-parameter") # Suppress Warnings

# Corrade
find_package(Corrade REQUIRED
  Interconnect
  )
list(APPEND LIBRARIES
  Corrade::Interconnect
  )

# Corrade Resources
corrade_add_resource(Textures_RESOURCES ${PROJECT_SOURCE_DIR}/data/textures/textures.conf)
list(APPEND SOURCES ${Textures_RESOURCES})

# EntityPlus
set(EntityPlus_DIR ${PROJECT_SOURCE_DIR}/third_party/repos/EntityPlus)
add_library(EntityPlus INTERFACE)
#set_target_properties(EntityPlus PROPERTIES COMPILE_FLAGS "-w -Wunused-variable") # Suppress Warnings

target_include_directories(EntityPlus INTERFACE ${EntityPlus_DIR})

include_directories(SYSTEM ${EntityPlus_DIR})

list(APPEND LIBRARIES EntityPlus)

# Simple File Watcher
set(SFW_DIR ${PROJECT_SOURCE_DIR}/third_party/simplefilewatcher)
file(GLOB SFW_SOURCES ${SFW_DIR}/source/*.cpp)

add_library(SFW STATIC ${SFW_SOURCES})
set_target_properties(SFW PROPERTIES COMPILE_FLAGS "-w -Wold-style-cast -Wunused-variable") # Suppress Warnings
#target_include_directories(SFW PUBLIC ${SFW_DIR}/includes)
include_directories(SYSTEM ${SFW_DIR}/includes)
list(APPEND LIBRARIES SFW)
# UI
#find_package(MagnumExtras REQUIRED Ui)
#list(APPEND LIBRARIES MagnumExtras::Ui)

# ImGui

#if(NOT CORRADE_TARGET_ANDROID)
if(FALSE) #TODO: Make imgui work with Magnum
  set(IMGUI_DIR ${PROJECT_SOURCE_DIR}/third_party/repos/imgui)
  set(imgui_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imguifilesystem.cpp
    )

  list(APPEND imgui_SOURCES
    #${IMGUI_DIR}/examples/opengl3_example/imgui_impl_glfw_gl3.cpp
    ${IMGUI_DIR}/examples/sdl_opengl2_example/imgui_impl_sdl_gl2.cpp
    )

  find_package(SDL2 REQUIRED)
  find_package(GLEW REQUIRED)
  include_directories(${SDL2_INCLUDE_DIR})
  add_library(imgui STATIC ${imgui_SOURCES})
  set_target_properties(imgui PROPERTIES COMPILE_FLAGS "-w") # Suppress Warnings
  include_directories(SYSTEM
    ${IMGUI_DIR}/
    ${IMGUI_DIR}/examples/sdl_opengl2_example/
    )

  list(APPEND LIBRARIES
    imgui
    )
endif()


# Boost
#find_package( Boost COMPONENTS filesystem REQUIRED )
#include_directories( ${Boost_INCLUDE_DIR} )
#list(APPEND LIBRARIES ${Boost_LIBRARIES} )

target_link_libraries(MyApplication PRIVATE ${LIBRARIES})
