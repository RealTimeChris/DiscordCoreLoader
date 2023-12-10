CMAKE_MINIMUM_REQUIRED(VERSION 3.20)
project(discord_core_loader)

set(CMAKE_CXX_STANDARD 20)
set(PROJECT_NAME "DiscordCoreLoader")
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)
FetchContent_Declare(
   Jsonifier
   GIT_REPOSITORY https://github.com/RealTimeChris/Jsonifier.git
   GIT_TAG dev
)
FetchContent_MakeAvailable(Jsonifier)

file(GLOB SOURCES CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/Source/*.cpp")

set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/../Install")

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake")

set(OPENSSL_ROOT_DIR "C:/vcpkg/installed/x64-windows")

set(simdjson_DIR "C:/vcpkg/installed/x64-windows/share/simdjson")

find_package(OpenSSL REQUIRED)

add_subdirectory(Executable)