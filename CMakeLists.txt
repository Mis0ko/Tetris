# Define the project name
project(Tetris LANGUAGES CXX)

#set(CMAKE_CXX_STANDARD 14)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set the cmake minimum version to 3.5.1
cmake_minimum_required(VERSION 3.7)

# Add all c source files under the src directory
file(GLOB SOURCES "src/*.cpp" "src/*.h")
add_executable(${PROJECT_NAME} ${SOURCES})

# Add all headers files under the include directory
#target_include_directories(${PROJECT_NAME} PRIVATE include)

# Add SDL2 library
find_package(SDL2 REQUIRED)
target_link_libraries(${PROJECT_NAME} SDL2::SDL2 SDL2::SDL2main)

# Add SDL2_image library
find_package(SDL2-image REQUIRED)
target_link_libraries(${PROJECT_NAME} SDL2::SDL2_image)

# Add SDL2_ttf library
find_package(SDL2-ttf REQUIRED)
target_link_libraries(${PROJECT_NAME} SDL2::SDL2_ttf)

# Add SDL2_mixer library
find_package(SDL2-mixer REQUIRED)
target_link_libraries(${PROJECT_NAME} SDL2::SDL2_mixer)

