cmake_minimum_required(VERSION 3.17)
project(B2C VERSION 1.0 LANGUAGES CXX C)
set(CMAKE_CXX_STANDARD 20)

# Removes the check to verify someone has purchased B2C or not
add_compile_definitions(BYPASS_AUTH_CHECK=1)


add_subdirectory(src)