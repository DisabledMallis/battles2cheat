add_library(${CMAKE_PROJECT_NAME}Lib SHARED)

target_include_directories(${CMAKE_PROJECT_NAME}Lib PRIVATE "${CMAKE_SOURCE_DIR}/Lib/OpenSSL/1.1.1m/include/")
target_include_directories(${CMAKE_PROJECT_NAME}Lib PRIVATE "${CMAKE_BINARY_DIR}/_deps/plh-src/")
target_include_directories(${CMAKE_PROJECT_NAME}Lib PRIVATE "${CMAKE_BINARY_DIR}/_deps/plh-src/capstone/include/")
target_include_directories(${CMAKE_PROJECT_NAME}Lib PRIVATE "${CMAKE_BINARY_DIR}/_deps/ch-src/lib/jsrt/")
target_include_directories(${CMAKE_PROJECT_NAME}Lib PRIVATE "${CMAKE_CURRENT_LIST_DIR}/include/")
target_include_directories(${CMAKE_PROJECT_NAME}Lib PRIVATE "${CMAKE_SOURCE_DIR}/shared/")
target_include_directories(${CMAKE_PROJECT_NAME}Lib PRIVATE "${CMAKE_SOURCE_DIR}/soup/include")
target_include_directories(${CMAKE_PROJECT_NAME}Lib PRIVATE "${CMAKE_SOURCE_DIR}/shared/gl3w/include/")

target_link_directories(${CMAKE_PROJECT_NAME}Lib PRIVATE "${CMAKE_SOURCE_DIR}/Lib/OpenSSL/1.1.1m/lib")
target_link_directories(${CMAKE_PROJECT_NAME}Lib PRIVATE "${CMAKE_BINARY_DIR}/_deps/plh-build/")
target_link_directories(${CMAKE_PROJECT_NAME}Lib PRIVATE "${CMAKE_BINARY_DIR}/_deps/plh-build/capstone/")
target_link_directories(${CMAKE_PROJECT_NAME}Lib PRIVATE "${CMAKE_BINARY_DIR}/_deps/ch-src/Build/VcBuild/bin/x64_${CMAKE_BUILD_TYPE}")

target_sources(${CMAKE_PROJECT_NAME}Lib PRIVATE
	"main.cpp"
	"../shared/memory.cpp"
	"decryptor.cpp"
	"player_info.cpp"
	"cheat_menu.cpp"
	"js_wrapper.cpp"
	"js_console.cpp"
	#"redshift_display.cpp"
	"inject_playground.cpp"
	"win_bot.cpp")

add_compile_definitions(CHEAT_ONLY=1)
add_compile_definitions(PREVENT_DUMPS=1)
add_compile_definitions(B2C_VERSION="1.13.1")
add_compile_definitions(GAME_VERSION="v1.3.3")

add_subdirectory(hooks)
add_subdirectory(render)
add_subdirectory(seshgen)
add_subdirectory(bfbproxy)
add_subdirectory(network)

target_link_libraries(${CMAKE_PROJECT_NAME}Lib PUBLIC ${CMAKE_PROJECT_NAME}Common PRIVATE PolyHook_2 Zydis Zycore opengl32 ChakraCore libcrypto libssl)

add_custom_command(
	TARGET ${CMAKE_PROJECT_NAME}Lib POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy
		${CMAKE_BINARY_DIR}/src/B2CLib.dll
		${CMAKE_CURRENT_LIST_DIR}/Release/B2CLib.dll
	COMMAND ${CMAKE_COMMAND} -E copy
		${CMAKE_BINARY_DIR}/_deps/ch-src/Build/VcBuild/bin/x64_${CMAKE_BUILD_TYPE}/ChakraCore.dll
		${CMAKE_CURRENT_LIST_DIR}/Release/ChakraCore.dll
	COMMAND ${CMAKE_COMMAND} -E copy
		${CMAKE_CURRENT_LIST_DIR}/../Lib/OpenSSL/1.1.1m/bin/libcrypto-1_1-x64.dll
		${CMAKE_CURRENT_LIST_DIR}/Release/libcrypto-1_1-x64.dll
	COMMAND ${CMAKE_COMMAND} -E copy
		${CMAKE_CURRENT_LIST_DIR}/../Lib/OpenSSL/1.1.1m/bin/libssl-1_1-x64.dll
		${CMAKE_CURRENT_LIST_DIR}/Release/libssl-1_1-x64.dll
)