include(GenerateProductVersion)
generate_product_version(
	PRODUCT_INFO
	NAME "${PROJECT_NAME}"
	FILE_DESCRIPTION "${DESCRIPTION}"
	ORIGINAL_FILENAME "discordcoreloader.exe"
	ICON "${CMAKE_SOURCE_DIR}/Documentation/Images/TheLogo.ico"
	COMPANY_NAME "RealTimeChris Inc."
	VERSION_MAJOR "1"
	VERSION_MINOR "0"
	VERSION_PATCH "0"
	VERSION_REVISION "0"
)

if (UNIX)
	set(CMAKE_CXX_FLAGS_RELEASE "-DNDEBUG -Ofast")
endif()

file(GLOB SOURCES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/Source/*.cpp")

if (WIN32)
	add_executable("${PROJECT_NAME}" "${SOURCES}" "${PRODUCT_INFO}")
elseif (UNIX)
	add_executable("${PROJECT_NAME}" "${SOURCES}" "${PRODUCT_INFO}")
	find_package(Threads REQUIRED)
endif()

file(GLOB HEADERS CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/Include/discordcoreloader/*.hpp")

set_target_properties(
	"${PROJECT_NAME}" PROPERTIES
	OUTPUT_NAME "DiscordCoreLoader"
	CXX_STANDARD_REQUIRED ON
	CXX_EXTENSIONS OFF
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/Output Files/$<CONFIG>"
)

target_include_directories(
	"${PROJECT_NAME}" PUBLIC
	"$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>"
	PRIVATE
	"$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/Include/>"
)

target_link_libraries(
	"${PROJECT_NAME}" PUBLIC
	$<$<TARGET_EXISTS:OpenSSL::Crypto>:OpenSSL::Crypto>
	$<$<TARGET_EXISTS:OpenSSL::SSL>:OpenSSL::SSL>
	$<$<TARGET_EXISTS:Threads::Threads>:Threads::Threads>
	$<$<TARGET_EXISTS:simdjson::simdjson>:simdjson::simdjson>
	$<$<TARGET_EXISTS:Jsonifier::Jsonifier>:Jsonifier::Jsonifier>
)

target_compile_features(
	"${PROJECT_NAME}" PUBLIC
	"cxx_std_20"
)

if (DEFINED ENV{EnableASAN})
	if(MSVC)
		target_compile_options("${PROJECT_NAME}"  PUBLIC "/fsanitize=address" "/Zi" "/Debug")
		target_link_options("${PROJECT_NAME}"  PUBLIC "/incremental:no" "/Debug")	
	else()
		target_compile_options("${PROJECT_NAME}"  PUBLIC "-fsanitize=address")
		target_link_options("${PROJECT_NAME}"  PUBLIC "-fsanitize=address")
	endif()
endif()

target_compile_options(
	"${PROJECT_NAME}"  PUBLIC
	"$<$<PLATFORM_ID:Windows>:/bigobj>"
	"$<$<PLATFORM_ID:Windows>:/MP>"
	"$<$<PLATFORM_ID:Windows>:/W4>"
	"$<$<PLATFORM_ID:Linux>:-mavx>"
	"$<$<PLATFORM_ID:Linux>:-mavx2>"
)

target_link_options(
	"${PROJECT_NAME}"  PUBLIC
	"$<$<PLATFORM_ID:Windows>:/DEBUG>"
)

install(
	FILES
	"${CMAKE_SOURCE_DIR}/Config.json"
	"${CMAKE_SOURCE_DIR}/Certs/Key.pem"
	"${CMAKE_SOURCE_DIR}/Certs/Cert.pem"
	DESTINATION "${CMAKE_SOURCE_DIR}/Output Files/$<CONFIG>"
)

install(
	TARGETS "${PROJECT_NAME}" 
	RUNTIME DESTINATION "${CMAKE_SOURCE_DIR}/Output Files/$<CONFIG>"
)
