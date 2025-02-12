cmake_minimum_required(VERSION 3.15)
project(WeatherAppCustom)

set(CMAKE_CXX_STANDARD 17)

# Set vcpkg toolchain file
set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")

# Find packages
find_package(OpenGL REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(GLEW REQUIRED)

# Add Windows Socket library for httplib
if(WIN32)
    set(PLATFORM_LIBS ws2_32 wsock32)
endif()

# ImGui source files
set(IMGUI_SOURCES
        ImGui/imgui.cpp
        ImGui/imgui_demo.cpp
        ImGui/imgui_draw.cpp
        ImGui/imgui_tables.cpp
        ImGui/imgui_widgets.cpp
        ImGui/imgui_impl_glfw.cpp
        ImGui/imgui_impl_opengl3.cpp
        )

# Main source files
set(SOURCE_FILES
        main.cpp
        WeatherApp.cpp
        CityWeather.cpp
        stb_image.cpp
        )

# Add executable
add_executable(${PROJECT_NAME}
        ${SOURCE_FILES}
        ${IMGUI_SOURCES}
        )

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/ImGui
        ${CMAKE_CURRENT_SOURCE_DIR}/Externals
        ${GLEW_INCLUDE_DIRS}
        )

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
        OpenGL::GL
        glfw
        GLEW::GLEW
        ${PLATFORM_LIBS}
        )

# Define WIN32_LEAN_AND_MEAN to avoid Windows.h conflicts
target_compile_definitions(${PROJECT_NAME} PRIVATE
        WIN32_LEAN_AND_MEAN
        _WIN32_WINNT=0x0601
        )

# Copy Externals folder to build directory
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/Externals DESTINATION ${CMAKE_BINARY_DIR})

# Copy credentials and favorites files
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/credentials.txt
        ${CMAKE_BINARY_DIR}/credentials.txt COPYONLY)

# Create empty favorites.txt if it doesn't exist
if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/favorites.txt)
    file(WRITE ${CMAKE_CURRENT_SOURCE_DIR}/favorites.txt "")
endif()

# Copy favorites.txt
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/favorites.txt
        ${CMAKE_BINARY_DIR}/favorites.txt COPYONLY)