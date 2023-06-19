# CMakeList.txt : CMake project for TheButton, include source and define
# project specific logic here.
#

option(BUILD_EXAMPLE "Build example" OFF)
option(BUILD_CSHARP "Build C#" OFF)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
add_subdirectory(screen_capture_lite)
find_package(OpenCV CONFIG REQUIRED)

# Add source to this project's executable.
add_executable (TheButton "main.cpp" "NumberExtractor.cpp")
add_executable (TheButtonTests "tests.cpp" "NumberExtractor.cpp" "catch.cpp")

target_link_libraries (TheButton PRIVATE ${OpenCV_LIBS} screen_capture_lite_static Dwmapi.lib )
target_include_directories(TheButton PRIVATE screen_capture_lite/include)
target_link_libraries (TheButtonTests PRIVATE ${OpenCV_LIBS})

target_compile_definitions(TheButton PRIVATE "SOURCE_DIR=\"${CMAKE_SOURCE_DIR}\"")
target_compile_definitions(TheButtonTests PRIVATE "SOURCE_DIR=\"${CMAKE_SOURCE_DIR}\"")

set_property(TARGET TheButton PROPERTY CXX_STANDARD 20)
set_property(TARGET TheButtonTests PROPERTY CXX_STANDARD 20)

include(CheckIPOSupported)
check_ipo_supported(RESULT result OUTPUT output)
if(result)
    set_property(TARGET TheButton PROPERTY INTERPROCEDURAL_OPTIMIZATION $<NOT:$<CONFIG:Debug>>)
endif()


# TODO: Add tests and install targets if needed.
