cmake_minimum_required(VERSION 3.16)

project(JaydesSDL3 
    VERSION 1.0.0
    DESCRIPTION "SDL3 Application"
    LANGUAGES CXX
)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set build type if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Platform-specific configurations
if(WIN32)
    set(SDL3_ARCH "x64")
    if(CMAKE_SIZEOF_VOID_P EQUAL 4)
        set(SDL3_ARCH "x86")
    endif()
    if(CMAKE_SYSTEM_PROCESSOR MATCHES "ARM64")
        set(SDL3_ARCH "arm64")
    endif()
endif()

if(WIN32)
    add_executable(JaydesSDL3 main.cpp)
else()
    add_executable(JaydesSDL3 main.cpp)
endif()
# Set target properties
set_target_properties(JaydesSDL3 PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
    OUTPUT_NAME "Convenience"
)

# Include directories
target_include_directories(JaydesSDL3 PRIVATE 
    ${CMAKE_SOURCE_DIR}/include
)

# Include audio directory
target_include_directories(JaydesSDL3 PUBLIC 
    ${CMAKE_SOURCE_DIR}/audio
)

# Include images directory
target_include_directories(JaydesSDL3 PUBLIC 
    ${CMAKE_SOURCE_DIR}/images
)

# Link directories
if(WIN32)
    target_link_directories(JaydesSDL3 PRIVATE 
        ${CMAKE_SOURCE_DIR}/lib/${SDL3_ARCH}
    )
endif()

# Link libraries
if(WIN32)
    target_link_libraries(JaydesSDL3 PRIVATE 
        SDL3
        SDL3_test
        SDL3_image
        SDL3_ttf
    )
else()
    # For other platforms, try to find SDL3 using find_package
    find_package(SDL3 REQUIRED)
    target_link_libraries(JaydesSDL3 PRIVATE SDL3::SDL3)
endif()


if(WIN32)
    # Copy SDL3.dll to output directory on Windows
        add_custom_command(TARGET JaydesSDL3 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/lib/${SDL3_ARCH}/SDL3.dll"
        $<TARGET_FILE_DIR:JaydesSDL3>
        COMMENT "Copying SDL3.dll to output directory"
        )
        add_custom_command(TARGET JaydesSDL3 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/lib/${SDL3_ARCH}/SDL3_image.dll"
        $<TARGET_FILE_DIR:JaydesSDL3>
        COMMENT "Copying SDL3_image.dll to output directory"
        )
        add_custom_command(TARGET JaydesSDL3 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/lib/${SDL3_ARCH}/SDL3_ttf.dll"
        $<TARGET_FILE_DIR:JaydesSDL3>
        COMMENT "Copying SDL3_ttf.dll to output directory"
        )
        # Copy audio folder to output directory
        add_custom_command(TARGET JaydesSDL3 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/audio"
        "$<TARGET_FILE_DIR:JaydesSDL3>/audio"
        COMMENT "Copying audio folder to output directory"
        )
        
        # Copy images folder to output directory
        add_custom_command(TARGET JaydesSDL3 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/images"
        "$<TARGET_FILE_DIR:JaydesSDL3>/images"
        COMMENT "Copying images folder to output directory"
        )
        add_custom_command(TARGET JaydesSDL3 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/font"
        "$<TARGET_FILE_DIR:JaydesSDL3>/font"
        COMMENT "Copying font folder to output directory"
        )
endif()

# Add custom target to run the executable after build
add_custom_target(run
    COMMAND $<TARGET_FILE:JaydesSDL3>
    DEPENDS JaydesSDL3
    WORKING_DIRECTORY $<TARGET_FILE_DIR:JaydesSDL3>
    COMMENT "Running JaydesSDL3 executable"
    USES_TERMINAL
)

# Optional: Add a target that builds AND runs automatically
add_custom_target(build_and_run
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target JaydesSDL3
    COMMAND $<TARGET_FILE:JaydesSDL3>
    WORKING_DIRECTORY $<TARGET_FILE_DIR:JaydesSDL3>
    COMMENT "Building and running JaydesSDL3"
    USES_TERMINAL
)

# Set output directory
set_target_properties(JaydesSDL3 PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin
)

# Enable debug information for Debug builds
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(JaydesSDL3 PRIVATE DEBUG=1)
    if(MSVC)
        target_compile_options(JaydesSDL3 PRIVATE /W4)
    else()
        target_compile_options(JaydesSDL3 PRIVATE -Wall -Wextra -g)
    endif()
endif()

# Print configuration info
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ standard: ${CMAKE_CXX_STANDARD}")
if(WIN32)
    message(STATUS "SDL3 architecture: ${SDL3_ARCH}")
endif()