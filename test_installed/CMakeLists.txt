cmake_minimum_required(VERSION 3.14)
project(CsdInstalledTest LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Find dependencies first - the installed config references fmt.
find_package(fmt REQUIRED)
find_package(spdlog REQUIRED)
find_package(Csd REQUIRED)

add_executable(csd_installed_test main.cpp)
target_link_libraries(csd_installed_test PRIVATE Csd::Csd)

enable_testing()
add_test(NAME csd_installed_test COMMAND csd_installed_test)
