find_package(Git REQUIRED)

macro(add_git_module target uri branch)
    if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${target})
		message(STATUS "Cloning ${target} from ${uri}")
		execute_process(COMMAND ${GIT_EXECUTABLE} clone --branch ${branch} --depth 1
            ${uri} ${CMAKE_CURRENT_SOURCE_DIR}/${target} ERROR_QUIET)
        if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${target})
            message(FATAL_ERROR "Dependency error: Failed to download ${target}")
        endif()
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --depth 1 ${ARGN}
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${target})
        if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${target}.patch)
            execute_process(COMMAND ${GIT_EXECUTABLE} apply ${CMAKE_CURRENT_SOURCE_DIR}/${target}.patch
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${target})
        endif()
    endif()
endmacro()

option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
set(SKIP_INSTALL_ALL TRUE)
set(ENABLE_BZIP2 OFF)
set(ENABLE_LZMA OFF)

# zlib Library
add_git_module(zlib git://github.com/madler/zlib.git v1.2.11)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/zlib EXCLUDE_FROM_ALL)
if (MSVC)
    set(ZLIB_LIBRARY ${CMAKE_CURRENT_BINARY_DIR}/zlib/Release/zlibstatic.lib)
else()
    set(ZLIB_LIBRARY ${CMAKE_CURRENT_BINARY_DIR}/zlib/libzlibstatic.a)
endif()
set(ZLIB_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/zlib)
file(READ ${CMAKE_CURRENT_SOURCE_DIR}/zlib/zlib.h _zlib_h_contents)
string(REGEX REPLACE ".*#define[ \t]+ZLIB_VERSION[ \t]+\"([-0-9A-Za-z.]+)\".*" "\\1" ZLIB_VERSION_STRING ${_zlib_h_contents})
find_package(ZLIB REQUIRED)
set_target_properties(ZLIB::ZLIB PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${ZLIB_INCLUDE_DIRS};${CMAKE_CURRENT_BINARY_DIR}/zlib")
add_dependencies(ZLIB::ZLIB zlibstatic)

# libpng Library
add_git_module(libpng git://git.code.sf.net/p/libpng/code libpng16)
set(PNG_TESTS OFF)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libpng EXCLUDE_FROM_ALL)
target_include_directories(png_static PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/libpng ${CMAKE_CURRENT_BINARY_DIR}/zlib ${CMAKE_CURRENT_BINARY_DIR}/libpng)
