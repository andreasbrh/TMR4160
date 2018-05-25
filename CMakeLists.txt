cmake_minimum_required(VERSION 3.9)

project(Datametoder C)
find_package(OpenGL REQUIRED)
set(CMAKE_C_STANDARD 99)
set(GLFW_DIR glfw)
INCLUDE_DIRECTORIES(header GLFW_DIR)

LINK_DIRECTORIES(dll_files glfw/glfw-3.2.1.bin.WIN64/lib-mingw-w64 glfw/glfw-3.2.1.bin.WIN64/lib-mingw-w64)
add_executable(Datametoder main.c Visualization/visualization.c phidget_connections.c pid_controller.c)
#glfw stuff
set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "Build the GLFW example programs")
set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "Build the GLFW test programs")
set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "Build the GLFW documentation")
set(GLFW_INSTALL OFF CACHE INTERNAL "Generate installation target")
target_include_directories(${PROJECT_NAME} PRIVATE "${GLFW_DIR}/include")
target_compile_definitions(${PROJECT_NAME} PRIVATE "GLFW_INCLUDE_NONE")
target_link_libraries(
        Datametoder
        phidget22
        glfw3
        glad
        ${OPENGL_LIBRARY}
)
#glad stuff
set(GLAD_DIR "glad")
add_library("glad" "${GLAD_DIR}/src/glad.c")
target_include_directories("glad" PRIVATE "${GLAD_DIR}/include")
target_include_directories(${PROJECT_NAME} PRIVATE "${GLAD_DIR}/include")
target_link_libraries(${PROJECT_NAME} "glad" "${CMAKE_DL_LIBS}")