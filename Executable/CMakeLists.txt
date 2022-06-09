include(GenerateProductVersion)
generate_product_version(
	PRODUCT_INFO
	NAME "${EXE_NAME}"
	FILE_DESCRIPTION "${DESCRIPTION}"
	ORIGINAL_FILENAME "discordcoreloader.exe"
	ICON "${CMAKE_SOURCE_DIR}/Documentation/Images/TheLogo.ico"
	COMPANY_NAME "RealTimeChris Inc."
	VERSION_MAJOR "1"
	VERSION_MINOR "0"
	VERSION_PATCH "0"
	VERSION_REVISION "0"
)

file(GLOB SOURCES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/Source/*.cpp")

if (WIN32)
	add_executable("${EXE_NAME}" "${SOURCES}" "${PRODUCT_INFO}")
elseif (UNIX)
	add_executable("${EXE_NAME}" "${SOURCES}" "${PRODUCT_INFO}")
	find_package(Threads REQUIRED)
endif()

file(GLOB HEADERS CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/Include/discordcoreloader/*.hpp")

set_target_properties(
	"${EXE_NAME}" PROPERTIES
	OUTPUT_NAME "DiscordCoreLoader"
	CXX_STANDARD_REQUIRED ON
	CXX_EXTENSIONS OFF
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/Output Files/$<CONFIG>"
)

target_include_directories(
	"${EXE_NAME}" PUBLIC
	"$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>"
	PRIVATE
	"$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/Include/>"
)

include(FindNlohmannJson)
include(FindOpenSSL)

target_link_libraries(
	"${EXE_NAME}" PUBLIC
	$<$<TARGET_EXISTS:NLOHMANN::Json>:NLOHMANN::Json>
	$<$<TARGET_EXISTS:OPENSSL::Ssl>:OPENSSL::Ssl>
	$<$<TARGET_EXISTS:OPENSSL::Crypto>:OPENSSL::Crypto>
	$<$<TARGET_EXISTS:Threads::Threads>:Threads::Threads>
)

target_compile_features(
	"${EXE_NAME}" PUBLIC
	"cxx_std_20"
)

if (DEFINED ENV{EnableASAN})
	if(MSVC)
		target_compile_options("${EXE_NAME}" PUBLIC "/fsanitize=address" "/Zi" "/Debug")
		target_link_options("${EXE_NAME}" PUBLIC "/incremental:no" "/Debug")	
	else()
		target_compile_options("${EXE_NAME}" PUBLIC "-fsanitize=address")
		target_link_options("${EXE_NAME}" PUBLIC "-fsanitize=address")
	endif()
endif()

target_compile_options(
	"${EXE_NAME}" PUBLIC
	"$<$<PLATFORM_ID:Windows>:/bigobj>"
	"$<$<PLATFORM_ID:Windows>:/MP>"
	"$<$<PLATFORM_ID:Windows>:/W4>"
	"$<$<PLATFORM_ID:Linux>:-O3>"
)

target_link_options(
	"${EXE_NAME}" PUBLIC
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
	TARGETS "${EXE_NAME}"
	RUNTIME DESTINATION "${CMAKE_SOURCE_DIR}/Output Files/$<CONFIG>"
)
