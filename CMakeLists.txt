cmake_minimum_required(VERSION 3.0.0)
project(GameBoyEmu VERSION 0.1.0)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake)

include(CTest)
enable_testing()

include_directories(include)
include_directories(imgui)

file(GLOB source_files "src/*.c" "src/*.cc")

execute_process(COMMAND cd rom && make OUTPUT_FILE STDOUT)

add_executable(GameBoyEmu 
  main.cpp  
  ${source_files})

set(LIB_DIR "lib")

set(IMGUI_DIR "${LIB_DIR}/imgui")

set(GLFW_DIR "${LIB_DIR}/glfw")

find_package( OpenGL REQUIRED )

include_directories( ${OPENGL_INCLUDE_DIRS} )

add_subdirectory( ${GLFW_DIR} )

target_link_libraries(GameBoyEmu ${OPENGL_LIBRARIES})

target_link_libraries(GameBoyEmu glfw)

include_directories( ${GLFW_DIR}/include)

add_library(imgui OBJECT "${IMGUI_DIR}/imgui.cpp"
                  "${IMGUI_DIR}/imgui_demo.cpp"
                  "${IMGUI_DIR}/imgui_draw.cpp"
                  "${IMGUI_DIR}/imgui_widgets.cpp"
                  "${IMGUI_DIR}/imgui_tables.cpp"
                  
                  "${IMGUI_DIR}/imgui_impl_glfw.cpp"
                  "${IMGUI_DIR}/imgui_impl_opengl3.cpp"
)

target_include_directories(imgui PUBLIC "${IMGUI_DIR}")
target_include_directories(${PROJECT_NAME} PUBLIC "${IMGUI_DIR}")

target_link_libraries(GameBoyEmu imgui)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
