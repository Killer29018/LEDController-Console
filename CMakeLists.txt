cmake_minimum_required(VERSION 3.10)

project (LEDController)

set (CMAKE_CXX_STANDARD 20)

# Import Libraries
find_package(OpenGL REQUIRED)

# Add Platform Specific Linker Options
set (linker "")
if (WIN32)
    set (linker "-lws2_32")
elseif (UNIX)
    set (linker "-ldl -lpthread")
endif()

# Change Output Directory
set (CMAKE_BINARY_DIR "/bin")
set (OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}${CMAKE_BINARY_DIR}")
if (CMAKE_BUILD_TYPE MATCHES Debug)
    message ("Building Debug")
    set (CMAKE_RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_DIRECTORY}/debug")
    set (linker "${linker} -g")
else ()
    message ("Building Release")
    set (CMAKE_RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_DIRECTORY}/release")
endif ()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-unknown-pragmas -Wno-ignored-attributes \
-Wno-overloaded-virtual -Wno-unused-private-field -Wno-overloaded-virtual")

file (GLOB_RECURSE cppFiles CONFIGURE_DEPENDS "src/*.cpp")
add_executable(${PROJECT_NAME} ${cppFiles})

# Add Subdirectories
add_subdirectory(vendor)

# Add libraries and include directories
target_include_directories(${PROJECT_NAME} PUBLIC
)

target_link_libraries(${PROJECT_NAME} PRIVATE ${linker} PUBLIC
    glm::glm
)

