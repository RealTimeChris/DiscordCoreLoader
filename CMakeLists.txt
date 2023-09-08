CMAKE_MINIMUM_REQUIRED(VERSION 3.20)
project(DiscordCoreLoader)

set(CMAKE_CXX_STANDARD 23)
set(PROJECT_NAME "DiscordCoreLoader")
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB SOURCES CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/Source/*.cpp")

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake")

set(OPENSSL_ROOT_DIR "C:/vcpkg/installed/x64-windows")

include(FetchContent)	
	FetchContent_Declare(
	   jsonifier
	   GIT_REPOSITORY https://github.com/RealTimeChris/jsonifier.git
	   GIT_TAG dev
	)
	FetchContent_MakeAvailable(jsonifier)

find_package(OpenSSL REQUIRED)

add_subdirectory(Executable)