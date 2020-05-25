cmake_minimum_required (VERSION 3.11)

include(FetchContent)

set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)
FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
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
  src/parsed_line.hpp
  src/parsed_line.cpp
  src/main.cpp
  src/ui/main_component.cpp
  src/ui/main_component.hpp
  src/ui/log_displayer.cpp
  src/ui/log_displayer.hpp
)
target_include_directories(chrome-log-beautifier PRIVATE src)

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
