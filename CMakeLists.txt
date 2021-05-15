cmake_minimum_required (VERSION 3.11)

include(FetchContent)

set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)
FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
  GIT_TAG 7daeac25c003b14898f9ff9c6731ceb83ec55fd3
)

FetchContent_GetProperties(ftxui)
if(NOT ftxui_POPULATED)
  FetchContent_Populate(ftxui)
  add_subdirectory(${ftxui_SOURCE_DIR} ${ftxui_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

project(chrome-log-beautifier
  LANGUAGES CXX
  VERSION 1.0.0
)

add_executable(chrome-log-beautifier
  src/data/parsed_line.cpp
  src/data/parsed_line.hpp
  src/main.cpp
  src/ui/log_displayer.cpp
  src/ui/log_displayer.hpp
  src/ui/main_component.cpp
  src/ui/main_component.hpp
  src/ui/info_component.hpp
  gen/ui/info_component.cpp
)


#-------------------------------------------------------------------------------
# Environment variable available in C++ source
#-------------------------------------------------------------------------------

execute_process(
  COMMAND git rev-list --count HEAD
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE git_position
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

execute_process(
  COMMAND git rev-parse HEAD
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE git_hash
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

string(TIMESTAMP today "%Y-%m-%d ")
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/info_component.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/gen/ui/info_component.cpp
)

target_include_directories(chrome-log-beautifier
  PRIVATE src
  PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/gen
)

target_link_libraries(chrome-log-beautifier
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component
)

set_target_properties(chrome-log-beautifier PROPERTIES CXX_STANDARD 17)

# Add as many warning as possible:
if (MSVC)
  target_compile_options(chrome-log-beautifier PRIVATE "/wd4244")
  target_compile_options(chrome-log-beautifier PRIVATE "/wd4267")
  target_compile_options(chrome-log-beautifier PRIVATE "/wd4996")
  target_compile_options(chrome-log-beautifier PRIVATE "/wd4305")
else()
  target_compile_options(chrome-log-beautifier PRIVATE "-Wall")
  target_compile_options(chrome-log-beautifier PRIVATE "-Werror")
  target_compile_options(chrome-log-beautifier PRIVATE "-Wno-sign-compare")
endif()


install(TARGETS chrome-log-beautifier RUNTIME DESTINATION "bin")

set(CPACK_GENERATOR "DEB")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Arthur Sonzogni")
include(CPack)
