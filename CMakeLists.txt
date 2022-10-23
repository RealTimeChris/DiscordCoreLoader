CMAKE_MINIMUM_REQUIRED(VERSION 3.20)
project(DiscordCoreLoader)

set(CMAKE_CXX_STANDARD 20)
set(PROJECT_NAME "DiscordCoreLoader")
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB SOURCES CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/Source/*.cpp")

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake")

set(OPENSSL_ROOT_DIR "C:/vcpkg/installed/x64-windows")

set(nlohmann_json_DIR "C:/vcpkg/installed/x64-windows/share/nlohmann_json")

set(simdjson_DIR "C:/vcpkg/installed/x64-windows/share/simdjson")

find_package(OpenSSL REQUIRED)

find_package(simdjson REQUIRED)

find_package(nlohmann_json REQUIRED)

add_subdirectory(Executable)