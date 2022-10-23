# This file locates the Nlohmann-Json library, using a provided path for searching.
#
# Usage:
#	Set the following directories: NLOHMANN_INCLUDE_DIR
# Where:
#	NLOHMANN_INCLUDE_DIR = The directory containing the public headers.
# What it produces:
#	NLOHMANN::Json
if (NOT EXISTS ${NLOHMANN_INCLUDE_DIR})
	list(GET VCPKG_INSTALLED_DIRS 0 NLOHMANN_INCLUDE_DIR)
	set(NLOHMANN_INCLUDE_DIR "${NLOHMANN_INCLUDE_DIR}/include")
	message("THE OUTPUT: ${NLOHMANN_INCLUDE_DIR}")
endif()
find_file(
	NLOHMANN_JSON_LIBRARY
	NAMES "nlohmann/json.hpp" 
	PATHS "${NLOHMANN_INCLUDE_DIR}" NO_DEFAULT_PATH
)
if(EXISTS "${NLOHMANN_JSON_LIBRARY}" AND EXISTS "${NLOHMANN_INCLUDE_DIR}")
	message(STATUS "Found Nlohmann-Json: TRUE")
	add_library(NLOHMANN::Json INTERFACE IMPORTED GLOBAL)
	target_include_directories(NLOHMANN::Json INTERFACE "${NLOHMANN_INCLUDE_DIR}")
	return()
endif()
if (NOT EXISTS ${NLOHMANN_INCLUDE_DIR})
	list(GET VCPKG_INSTALLED_DIRS 1 NLOHMANN_INCLUDE_DIR)
	set(NLOHMANN_INCLUDE_DIR "${NLOHMANN_INCLUDE_DIR}/include")
	message("${NLOHMANN_INCLUDE_DIR}")
endif()
find_file(
	NLOHMANN_JSON_LIBRARY
	NAMES "nlohmann/json.hpp" 
	PATHS "${NLOHMANN_INCLUDE_DIR}" NO_DEFAULT_PATH
)
if(EXISTS "${NLOHMANN_JSON_LIBRARY}" AND EXISTS "${NLOHMANN_INCLUDE_DIR}")
	message(STATUS "Found Nlohmann-Json: TRUE")
	add_library(NLOHMANN::Json INTERFACE IMPORTED GLOBAL)
	target_include_directories(NLOHMANN::Json INTERFACE "${NLOHMANN_INCLUDE_DIR}")
	return()
else()
	message(FATAL_ERROR "Found Nlohmann-Json: FALSE")
endif()