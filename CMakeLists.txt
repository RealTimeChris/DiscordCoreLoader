CMAKE_MINIMUM_REQUIRED(VERSION 3.20)
project(DiscordCoreLoader)

set(CMAKE_CXX_STANDARD 20)
set(PROJECT_NAME "DiscordCoreLoader")
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB SOURCES CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/Source/*.cpp")

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake")

include(FetchContent)
	FetchContent_Declare(
   Jsonifier
   GIT_REPOSITORY https://github.com/RealTimeChris/Jsonifier.git
   GIT_TAG dev
)
FetchContent_MakeAvailable(Jsonifier)

set(OPENSSL_ROOT_DIR "C:/vcpkg/installed/x64-windows")

set(simdjson_DIR "C:/vcpkg/installed/x64-windows/share/simdjson")

find_package(OpenSSL REQUIRED)

find_package(simdjson REQUIRED)

add_subdirectory(Executable)