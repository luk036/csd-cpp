cmake_minimum_required(VERSION 3.14...3.22)

project(CsdStandalone LANGUAGES CXX)

# --- Import tools ----

include(../cmake/tools.cmake)

# ---- Dependencies ----

include(../cmake/CPM.cmake)

CPMAddPackage(
  GITHUB_REPOSITORY jarro2783/cxxopts
  VERSION 3.2.1
  OPTIONS "CXXOPTS_BUILD_EXAMPLES NO" "CXXOPTS_BUILD_TESTS NO" "CXXOPTS_ENABLE_INSTALL YES"
)

CPMAddPackage(NAME Csd SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/..)

# ---- Create standalone executable ----

add_executable(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/source/main.cpp)

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 17 OUTPUT_NAME "Csd")

target_link_libraries(${PROJECT_NAME} Csd::Csd cxxopts::cxxopts)

# ---- Create standalone executable with spdlog example ----

add_executable(${PROJECT_NAME}_spdlog ${CMAKE_CURRENT_SOURCE_DIR}/source/main_with_spdlog.cpp)

set_target_properties(${PROJECT_NAME}_spdlog PROPERTIES CXX_STANDARD 17 OUTPUT_NAME "Csd_spdlog")

target_link_libraries(${PROJECT_NAME}_spdlog Csd::Csd cxxopts::cxxopts spdlog::spdlog)
