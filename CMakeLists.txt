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

add_executable(chrome-log-beautifier src/main.cpp)
target_include_directories(chrome-log-beautifier PRIVATE src)

target_link_libraries(chrome-log-beautifier
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component
)

set_target_properties(chrome-log-beautifier PROPERTIES CXX_STANDARD 17)

install(TARGETS chrome-log-beautifier RUNTIME DESTINATION "bin")
