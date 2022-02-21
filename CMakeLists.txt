cmake_minimum_required(VERSION 3.16)
project("Extract Pal")
set(CMAKE_STATIC_LIBRARY_PREFIX "")

#set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Release>:Release>")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

add_subdirectory(thirdparty)

add_executable(extractpal src/extractpal.cpp src/main16b.cpp)
target_link_libraries(extractpal zlibstatic png_static)
add_executable(extractpal-true src/extractpal.cpp src/main32b.cpp)
target_link_libraries(extractpal-true zlibstatic png_static)
