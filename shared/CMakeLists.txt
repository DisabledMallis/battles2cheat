add_library(${CMAKE_PROJECT_NAME}Common STATIC)

target_include_directories(${CMAKE_PROJECT_NAME}Common PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/gl3w/include/")
target_include_directories(${CMAKE_PROJECT_NAME}Common PRIVATE "${CMAKE_SOURCE_DIR}/Lib/OpenSSL/1.1.1m/include/")

target_sources(${CMAKE_PROJECT_NAME}Common PUBLIC
	"gl3w/src/gl3w.c"
	"auth.cpp"
	"files.cpp"
	"logger.cpp"
	"strconvert.cpp"
	"web.cpp"
	"exception.cpp"
	"config.cpp"
	"dialogs.cpp"
	"shared_thread.cpp"
)

add_subdirectory(imgui)
add_subdirectory(benc)
add_subdirectory(utils)

target_link_libraries(${CMAKE_PROJECT_NAME}Common PRIVATE Dbghelp)