#include "ui/log_displayer.hpp"

#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/string.hpp>
#include <ftxui/component/event.hpp>
#include <map>

#include <locale>
#include <codecvt>

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
    {L"NINJA", {color(Color::Blue), dim}},
};
}  // namespace

Element LogDisplayer::RenderLines(const std::vector<Topic>& topics) {
  size = topics.size();

  Elements list;
  size_t size_type = 5;
  size_t num_size = 6;
  //int size_file = 10;
  //int thread_size = 6;

  for (auto& it : topics) {
    size_type = std::max(size_type, it.m_topic_type.length());
  }


  auto header = hbox({
      text(L"Type") | ftxui::size(WIDTH, EQUAL, size_type),
      separator(),
      text(L"Num") | ftxui::size(WIDTH, EQUAL, num_size),
      separator(),
      text(L"Message") | flex,
      separator(),
      text(L"Time") | dim | ftxui::size(WIDTH, EQUAL, 14),
  });

  auto previous_type = topics.size() ? topics[0].m_topic_type : "";

  int index = 0;
  for (auto& it : topics) {
    bool is_focus = (index++ == selected_);
    if (previous_type != it.m_topic_type)
      list.push_back(separator());
    previous_type = it.m_topic_type;

    LogStyle ls = ((index % 2) == 0)?LogStyle{color(Color::GrayLight), dim}:LogStyle{color(Color::GrayDark), dim};
    Decorator line_decorator =  ls.level_decorator;//log_style[it->level].line_decorator;
    Decorator level_decorator = nothing; //log_style[it->level].level_decorator;

    if (is_focus) {

      using convert_type = std::codecvt_utf8<wchar_t>;
      std::wstring_convert<convert_type, wchar_t> converter;

      //use converter (.to_bytes: wstr->str, .from_bytes: str->wstr)
      m_seltext = "Content " + std::to_string(index - 1) + ": " + it.m_path;
      //converter.to_bytes( it->log );
      line_decorator = line_decorator | focus;
      if (Focused())
        line_decorator = line_decorator | focus | inverted;
    }

    Element document =  //
        hbox({
            text(it.m_topic_type)                              //
                | ftxui::size(WIDTH, EQUAL, size_type)  //
                | level_decorator                        //
            ,
            separator(),
            text(std::to_string(index - 1))                //
                | ftxui::size(WIDTH, EQUAL, num_size)  //
                | notflex,
            separator(),

            //hbox({
            //    text(it->file),
            //    text(it->line)  //
            //        | dim,
            //})                                          //
            //    | ftxui::size(WIDTH, EQUAL, size_file)  //
            //    | notflex,
            //separator(),

            text(it.m_path)  //
                | flex,

            separator(),

            hbox({
                text(to_wstring(0)),
                text(L":"),
                text(to_wstring(1)),
                text(L" "),
                text(to_wstring(0)),
                text(L"ms"),
            })                                   //
                | dim                            //
                | ftxui::size(WIDTH, EQUAL, 14)  //
                | notflex,
        }) |
        flex | line_decorator;
    list.push_back(document);
  }

  if (list.empty())
    list.push_back(text(L"(empty)"));

  return window(text(L"Log"), vbox({
                                  header,
                                  separator(),
                                  vbox(list) | yframe,
                              }));
}

bool LogDisplayer::OnEvent(Event event) {
  if (!Focused())
    return false;

  int old_selected = selected_;
  if (event.is_mouse() && event.mouse().button == Mouse::Left) {
    //auto m = CaptureMouse(event);
  }

  if (event == Event::ArrowUp || event == Event::Character('k'))
    selected_--;
  if (event == Event::ArrowDown || event == Event::Character('j'))
    selected_++;
  if (event == Event::Tab && size)
    selected_ = (selected_ + 1) % size;
  if (event == Event::TabReverse && size)
    selected_ = (selected_ + size - 1) % size;
  if (event == Event::PageDown)  {
    selected_ = selected_ + 10;
  }

  if (event == Event::PageUp)  {
    selected_ = selected_ - 10;
  }

  selected_ = std::max(0, std::min(size-1, selected_));

  if (selected_ != old_selected) {
    // hack
    m_parent.Render();
    return true;
  }

  return false;
}
