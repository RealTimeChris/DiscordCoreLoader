# This file locates the OpenSSL libraries, using a couple of provided paths for searching.
#
# Usage:
# Set the following directories: OPENSSL_RELEASE_ROOT, OPENSSL_DEBUG_ROOT, OPENSSL_INCLUDE_DIR
# Where:
#	OPENSSL_RELEASE_ROOT = The directory containing the RELEASE version of the library, or library's linker file.
#	OPENSSL_DEBUG_ROOT = The directory containing the DEBUG version of the library, or library's linker file.
#	OPENSSL_INCLUDE_DIR = The directory containing the public headers.
# What it produces:
#	OPENSSL::Crypto and OPENSLL::Ssl
if (NOT EXISTS "${OPENSSL_INCLUDE_DIR}")
	list(GET VCPKG_INSTALLED_DIRS -1 OPENSSL_INCLUDE_DIR)
	set(OPENSSL_INCLUDE_DIR "${OPENSSL_INCLUDE_DIR}/include")
endif()
if (NOT EXISTS "${OPENSSL_RELEASE_ROOT}")
	list(GET VCPKG_INSTALLED_DIRS -1 OPENSSL_RELEASE_ROOT)
	set(OPENSSL_RELEASE_ROOT "${OPENSSL_RELEASE_ROOT}/lib")
endif()
if (NOT EXISTS "${OPENSSL_DEBUG_ROOT}")
	list(GET VCPKG_INSTALLED_DIRS -1 OPENSSL_DEBUG_ROOT)
	set(OPENSSL_DEBUG_ROOT "${OPENSSL_DEBUG_ROOT}/debug/lib")
	message("${OPENSSL_DEBUG_ROOT}")
endif()
if (UNIX)
	set(LIB_SUFFIX ".a")
	set(LIB_PREFIX "lib")
else()
	set(LIB_SUFFIX ".lib")
	set(LIB_PREFIX "")
endif()
find_library(
	LIBCRYPTO_RELEASE_LIBRARY 
	NAMES "libcrypto${LIB_SUFFIX}" 
	PATHS "${OPENSSL_RELEASE_ROOT}" NO_DEFAULT_PATH
)
find_library(
	LIBCRYPTO_DEBUG_LIBRARY
	NAMES "libcrypto${LIB_SUFFIX}"
	PATHS "${OPENSSL_DEBUG_ROOT}" NO_DEFAULT_PATH
)
find_library(
	LIBSSL_RELEASE_LIBRARY 
	NAMES "libssl${LIB_SUFFIX}" 
	PATHS "${OPENSSL_RELEASE_ROOT}" NO_DEFAULT_PATH
)
find_library(
	LIBSSL_DEBUG_LIBRARY 
	NAMES "libssl${LIB_SUFFIX}" 
	PATHS "${OPENSSL_DEBUG_ROOT}" NO_DEFAULT_PATH
)
if (EXISTS "${LIBSSL_RELEASE_LIBRARY}" AND EXISTS "${LIBSSL_DEBUG_LIBRARY}" AND EXISTS "${LIBCRYPTO_RELEASE_LIBRARY}" 
	AND EXISTS "${LIBCRYPTO_DEBUG_LIBRARY}" AND EXISTS "${OPENSSL_INCLUDE_DIR}")
	message(STATUS "Found OpenSSL: TRUE")
	set(OPENSSL_INCLUDE_DIR )
	set(OPENSSL_RELEASE_ROOT )
	set(OPENSSL_DEBUG_ROOT)
endif()
if (NOT EXISTS "${OPENSSL_INCLUDE_DIR}")
	list(GET VCPKG_INSTALLED_DIRS 0 OPENSSL_INCLUDE_DIR)
	set(OPENSSL_INCLUDE_DIR "${OPENSSL_INCLUDE_DIR}/include")
endif()
if (NOT EXISTS "${OPENSSL_RELEASE_ROOT}")
	list(GET VCPKG_INSTALLED_DIRS 0 OPENSSL_RELEASE_ROOT)
	set(OPENSSL_RELEASE_ROOT "${OPENSSL_RELEASE_ROOT}/lib")
	message("${OPENSSL_RELEASE_ROOT}")
endif()
if (NOT EXISTS "${OPENSSL_DEBUG_ROOT}")
	list(GET VCPKG_INSTALLED_DIRS 0 OPENSSL_DEBUG_ROOT)
	set(OPENSSL_DEBUG_ROOT "${OPENSSL_DEBUG_ROOT}/debug/lib")
	message("${OPENSSL_DEBUG_ROOT}")
endif()
if (UNIX)
	set(LIB_SUFFIX ".so.3")
	set(LIB_PREFIX "lib")
else()
	set(LIB_SUFFIX ".dll")
	set(LIB_PREFIX "")
endif()
cmake_path(GET LIBSSL_RELEASE_LIBRARY PARENT_PATH LIBSSL_RELEASE_FILE_PATH)
find_file(
	LIBSSL_RELEASE_DLL
	NAMES "libssl-3-x64${LIB_SUFFIX}" "libssl${LIB_SUFFIX}"
	PATHS "${LIBSSL_RELEASE_FILE_PATH}/" "${LIBSSL_RELEASE_FILE_PATH}/../bin/" "${LIBSSL_RELEASE_FILE_PATH}/../lib/" "${OPENSSL_RELEASE_ROOT}"
	NO_DEFAULT_PATH
)
cmake_path(GET LIBSSL_DEBUG_LIBRARY PARENT_PATH LIBSSL_DEBUG_FILE_PATH)
find_file(
	LIBSSL_DEBUG_DLL
	NAMES "libssl-3-x64${LIB_SUFFIX}" "libssl${LIB_SUFFIX}"
	PATHS "${LIBSSL_DEBUG_FILE_PATH}/" "${LIBSSL_DEBUG_FILE_PATH}/../bin/" "${LIBSSL_DEBUG_FILE_PATH}/../lib/" "${OPENSSL_DEBUG_ROOT}"
	NO_DEFAULT_PATH
)
cmake_path(GET LIBCRYPTO_RELEASE_LIBRARY PARENT_PATH LIBCRYPTO_RELEASE_FILE_PATH)
find_file(
	LIBCRYPTO_RELEASE_DLL
	NAMES "libcrypto-3-x64${LIB_SUFFIX}" "libcrypto${LIB_SUFFIX}"
	PATHS "${LIBCRYPTO_RELEASE_FILE_PATH}/" "${LIBCRYPTO_RELEASE_FILE_PATH}/../bin/" "${LIBCRYPTO_RELEASE_FILE_PATH}/../lib/" "${OPENSSL_RELEASE_ROOT}"
	NO_DEFAULT_PATH
)
cmake_path(GET LIBCRYPTO_DEBUG_LIBRARY PARENT_PATH LIBCRYPTO_DEBUG_FILE_PATH)
find_file(
	LIBCRYPTO_DEBUG_DLL
	NAMES "libcrypto-3-x64${LIB_SUFFIX}" "libcrypto${LIB_SUFFIX}"
	PATHS "${LIBCRYPTO_DEBUG_FILE_PATH}/" "${LIBCRYPTO_DEBUG_FILE_PATH}/../bin/" "${LIBCRYPTO_DEBUG_FILE_PATH}/../lib/" "${OPENSSL_DEBUG_ROOT}"
	NO_DEFAULT_PATH
)
if (EXISTS "${LIBSSL_RELEASE_DLL}" AND EXISTS "${LIBSSL_DEBUG_DLL}" AND EXISTS "${LIBCRYPTO_RELEASE_DLL}" 
	AND EXISTS "${LIBCRYPTO_DEBUG_DLL}")
	add_library(OPENSSL::Crypto SHARED IMPORTED GLOBAL)
	set_target_properties(
		OPENSSL::Crypto PROPERTIES 
		IMPORTED_LOCATION_RELEASE "${LIBCRYPTO_RELEASE_DLL}" IMPORTED_LOCATION_DEBUG "${LIBCRYPTO_DEBUG_DLL}"
		IMPORTED_IMPLIB_RELEASE "${LIBCRYPTO_RELEASE_LIBRARY}" IMPORTED_IMPLIB_DEBUG "${LIBCRYPTO_DEBUG_LIBRARY}"
	)
	target_include_directories(OPENSSL::Crypto INTERFACE "${OPENSSL_INCLUDE_DIR}")
	add_library(OPENSSL::Ssl SHARED IMPORTED GLOBAL)
	set_target_properties(
		OPENSSL::Ssl PROPERTIES 
		IMPORTED_LOCATION_RELEASE "${LIBSSL_RELEASE_DLL}" IMPORTED_LOCATION_DEBUG "${LIBSSL_DEBUG_DLL}"
		IMPORTED_IMPLIB_RELEASE "${LIBSSL_RELEASE_LIBRARY}" IMPORTED_IMPLIB_DEBUG "${LIBSSL_DEBUG_LIBRARY}"
	)
	target_include_directories(OPENSSL::Ssl INTERFACE "${OPENSSL_INCLUDE_DIR}")	
	message(STATUS "Found OpenSSL Dlls: TRUE")
else()
	add_library(OPENSSL::Ssl STATIC IMPORTED GLOBAL)
	set_target_properties(
		OPENSSL::Ssl PROPERTIES 
		IMPORTED_LOCATION_RELEASE "${LIBSSL_RELEASE_LIBRARY}" IMPORTED_LOCATION_DEBUG "${LIBSSL_DEBUG_LIBRARY}"
	)
	target_include_directories(OPENSSL::Ssl INTERFACE "${OPENSSL_INCLUDE_DIR}")
	add_library(OPENSSL::Crypto STATIC IMPORTED GLOBAL)
	set_target_properties(
		OPENSSL::Crypto PROPERTIES 
		IMPORTED_LOCATION_RELEASE "${LIBCRYPTO_RELEASE_LIBRARY}" IMPORTED_LOCATION_DEBUG "${LIBCRYPTO_DEBUG_LIBRARY}"
	)
	target_include_directories(OPENSSL::Crypto INTERFACE "${OPENSSL_INCLUDE_DIR}")
	unset(LIBCRYPTO_RELEASE_DLL CACHE)
	unset(LIBCRYPTO_DEBUG_DLL CACHE)
	unset(LIBSSL_RELEASE_DLL CACHE)
	unset(LIBSSL_DEBUG_DLL CACHE)
	message(STATUS "Found OpenSSL Dlls: FALSE - linking statically")
endif()