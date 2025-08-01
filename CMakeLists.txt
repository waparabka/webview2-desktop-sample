cmake_minimum_required(VERSION 4.0)

project (w2ds) # project name

set(CMAKE_CXX_STANDARD 17) # cpp 17 standart
set(CMAKE_CXX_STANDARD_REQUIRED ON) # disables fallback to an older standart
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")
set(WEB_RESOURCE_DIR ${CMAKE_SOURCE_DIR}/third-party/ui/dist) # path to our web-ui
set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake") # vcpkg toolchain
set(CMAKE_PREFIX_PATH "C:/vcpkg/installed/x86-windows-static") # vcpkg prefix path

include(${CMAKE_SOURCE_DIR}/third-party/CMakeRC.cmake)
include(${CMAKE_SOURCE_DIR}/third-party/WebView2.cmake)

cmrc_add_resource_library(WebResources)
file(GLOB_RECURSE WEB_RESOURCES "${WEB_RESOURCE_DIR}/*" "${WEB_RESOURCE_DIR}/css/*" "${WEB_RESOURCE_DIR}/fonts/*" "${WEB_RESOURCE_DIR}/img/*" "${WEB_RESOURCE_DIR}/js/*")
cmrc_add_resources(WebResources WHENCE "${WEB_RESOURCE_DIR}" ${WEB_RESOURCES})

setup_webview2() # put nuget.exe in vcpkg folder -> https://dist.nuget.org/win-x86-commandline/latest/nuget.exe

include_directories(${WEBVIEW2_INCLUDE_DIRS})
link_directories(${WEBVIEW2_LIB_DIR})

add_executable(${PROJECT_NAME} WIN32 source/main.cpp source/webview_app.cpp)

foreach(flag_var # force static linking
    CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE
    CMAKE_C_FLAGS_MINSIZEREL CMAKE_C_FLAGS_RELWITHDEBINFO
    CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
    CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)

    set(${flag_var} "${${flag_var}} /MT")
endforeach()

# we can use vcpkg packages here, for example
find_package(fmt CONFIG REQUIRED) # vcpkg install fmt:x86-windows-static
find_package(spdlog CONFIG REQUIRED) # vcpkg install spdlog:x86-windows-static
find_package(cpprestsdk CONFIG REQUIRED) # vcpkg install cpprestsdk:x86-windows-static

# linking libs to project
target_link_libraries(${PROJECT_NAME} PRIVATE WebResources WebView2LoaderStatic.lib fmt::fmt-header-only spdlog::spdlog_header_only cpprestsdk::cpprest cpprestsdk::cpprestsdk_zlib_internal cpprestsdk::cpprestsdk_brotli_internal)
target_include_directories(${PROJECT_NAME} PRIVATE ${WEBVIEW2_INCLUDE_DIRS})

target_compile_definitions(${PROJECT_NAME} PRIVATE _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS)