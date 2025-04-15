cmake_minimum_required(VERSION 3.30)
include(FetchContent)
project(test LANGUAGES CXX)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(WINDOWS_SDK_INCLUDE "C:/Program Files (x86)/Windows Kits/10/Include/10.0.22621.0")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DCURL_STATICLIB")
set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /ENTRY:WinMainCRTStartup")


include_directories(
        ${WINDOWS_SDK_INCLUDE}/winrt
        ${WINDOWS_SDK_INCLUDE}/cppwinrt
)
find_package(CURL REQUIRED)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
FetchContent_MakeAvailable(json)

set(wxBUILD_SHARED OFF)

add_subdirectory(wxWidgets-3.2.6)

add_executable(test main.cpp
        Windows.h
        Windows.cpp
        Main.h
        SearchPanel.cpp
        SearchPanel.h
        NetworkController.cpp
        NetworkController.h
        BleSender.cpp
        BleSender.h
        Queue.cpp
        Queue.h)

target_link_libraries(test CURL::libcurl nlohmann_json::nlohmann_json)
target_link_libraries(test wx::net wx::core wx::base)
target_link_libraries(test ole32 runtimeobject)