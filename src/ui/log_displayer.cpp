#include "ui/log_displayer.hpp"

#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/string.hpp>
#include <map>

namespace {
struct LogStyle {
  Decorator level_decorator;
  Decorator line_decorator;
};

std::map<std::wstring, LogStyle> log_style = {
    {L"UNKNOWN", {nothing, dim}},
    {L"GTEST", {color(Color::Green), color(Color::Green)}},
    {L"FATAL", {color(Color::Red), bold}},
    {L"ERROR", {color(Color::RedLight), bold}},
    {L"WARNING", {color(Color::Yellow), nothing}},
    {L"INFO", {color(Color::Blue), dim}},
    {L"VERBOSE1", {color(Color::Cyan), dim}},
    {L"VERBOSE2", {color(Color::White), dim}},
    {L"VERBOSE3", {color(Color::GrayLight), dim}},
    {L"VERBOSE4", {color(Color::GrayDark), dim}},
};
}  // namespace

Element LogDisplayer::Render(std::vector<ParsedLine> lines) {
  size = lines.size();

  Elements list;
  int size_level = 5;
  int size_file = 10;
  int thread_size = 6;

  for (auto& it : lines) {
    size_level = std::max(size_level, (int)it.level.size());
    size_file = std::max(size_file, (int)it.file.size());
    thread_size = std::max(thread_size, (int)it.translated_thread_id.size());
  }

  auto header = hbox({
      text(L"Level") | ftxui::size(WIDTH, EQUAL, size_level) | notflex,
      separator(),
      text(L"Thread") | ftxui::size(WIDTH, EQUAL, thread_size) | notflex,
      separator(),
      text(L"File") | ftxui::size(WIDTH, EQUAL, size_file),
      separator(),
      text(L"Message") | flex | ftxui::size(WIDTH, EQUAL, 10),
      filler(),
      separator(),
      text(L"Time") | dim | ftxui::size(WIDTH, EQUAL, 14) | notflex,
  });

  int previous_type = lines.size() ? lines[0].type : -1;

  int index = 0;
  for (auto& it : lines) {
    bool is_focus = (index++ == selected_);
    if (previous_type != it.type)
      list.push_back(separator());
    previous_type = it.type;

    Decorator line_decorator = log_style[it.level].line_decorator;
    Decorator level_decorator = log_style[it.level].level_decorator;

    if (is_focus) {
      line_decorator = line_decorator | focus;
      if (Focused())
        line_decorator = line_decorator | focus | inverted;
    }

    Element document =  //
        hbox({
            text(it.level) | ftxui::size(WIDTH, EQUAL, size_level) |
                level_decorator | notflex,

            separator(),

            text(it.translated_thread_id) | ftxui::size(WIDTH, EQUAL, thread_size) | notflex,
            separator(),

            hbox({
                text(it.file),
                text(it.line) | dim,
            }) | ftxui::size(WIDTH, EQUAL, size_file),

            separator(),

            hbox(paragraph(it.log)) | ftxui::size(WIDTH, EQUAL, 100) | flex,

            filler(),
            separator(),
            hbox({
                text(to_wstring(it.minute)),
                text(L":"),
                text(to_wstring(it.seconds)),
                text(L" "),
                text(to_wstring(it.milliseconds)),
                text(L"ms"),
            }) | dim |
                ftxui::size(WIDTH, EQUAL, 14) | notflex,
        }) |
        flex | line_decorator;
    list.push_back(document);
  }

  return window(text(L"Log"), vbox({
                                  header,
                                  separator(),
                                  vbox(list) | frame,
                              }));
}

bool LogDisplayer::OnEvent(Event event) {
  if (!Focused())
    return false;

  int old_selected = selected_;
  if (event == Event::ArrowUp || event == Event::Character('k'))
    selected_--;
  if (event == Event::ArrowDown || event == Event::Character('j'))
    selected_++;
  if (event == Event::Tab && size)
    selected_ = (selected_ + 1) % size;
  if (event == Event::TabReverse && size)
    selected_ = (selected_ + size - 1) % size;

  selected_ = std::max(0, std::min(size-1, selected_));

  if (selected_ != old_selected) {
    return true;
  }

  return false;
}
