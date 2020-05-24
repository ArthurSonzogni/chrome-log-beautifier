#include <iostream>
#include <regex>
#include <set>
#include <string>
#include <fstream>
#include <optional>

#include "ftxui/component/checkbox.hpp"
#include "ftxui/component/component.hpp"
#include "ftxui/component/container.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"
#include "ftxui/screen/screen.hpp"
#include "ftxui/screen/string.hpp"

using namespace ftxui;

struct LogStyle{
  Decorator level_decorator;
  Decorator line_decorator;
};

std::map<std::wstring, LogStyle> log_style = {
    {L"GTEST"    , {color(Color::Green)  , color(Color::Green)}} ,
    {L"FATAL"    , {color(Color::Red)    , bold}}                ,
    {L"ERROR"    , {color(Color::Red)    , bold}}                ,
    {L"WARNING"  , {color(Color::Yellow) , nothing}}             ,
    {L"INFO"     , {color(Color::White)  , dim}}                 ,
    {L"VERBOSE1" , {color(Color::White)  , dim}}                 ,
    {L"VERBOSE2" , {color(Color::White)  , dim}}                 ,
    {L"VERBOSE3" , {color(Color::White)  , dim}}                 ,
    {L"VERBOSE4" , {color(Color::White)  , dim}}                 ,
};

struct ParsedLine {
  int process_id = 0;
  int thread_id = 0;

  int month = 0;
  int day = 0;

  int hour = 0;
  int minute = 0;
  int seconds = 0;
  int milliseconds = 0;

  std::wstring unknown;

  std::wstring level;

  std::wstring file;
  std::wstring line;
  std::wstring log;

  int type;
};

std::optional<ParsedLine> ParseLine(std::wstring line) {
  {
    std::wregex regex(
        LR"(^\[(\d+):(\d+):(\d\d)(\d\d)\/(\d\d)(\d\d)(\d\d)\.(\d+):(\d+):(\w+):(.+)\((\d+)\)\] (.*)$)");
    std::wsmatch match;
    if (std::regex_match(line, match, regex)) {
      ParsedLine out;
      int i = 1;

      out.process_id = std::stoi(match[i++].str());
      out.thread_id = std::stoi(match[i++].str());
      out.month = std::stoi(match[i++].str());
      out.day = std::stoi(match[i++].str());
      out.hour = std::stoi(match[i++].str());
      out.minute = std::stoi(match[i++].str());
      out.seconds = std::stoi(match[i++].str());
      out.milliseconds = std::stoi(match[i++].str());
      out.unknown = match[i++].str();
      out.level = match[i++].str();
      out.file = match[i++].str();
      out.line = L"(" + match[i++].str() + L")";
      out.log = match[i++].str();

      out.type = 1;
      return out;
    }
  }

  {
    std::wregex regex(LR"(^\[(..........)\] (.*)$)");
    std::wsmatch match;
    if (std::regex_match(line, match, regex)) {
      ParsedLine out;

      out.level = L"GTEST";
      out.line = L"";
      out.log = L"[" + match[1].str() + L"]";
      match[2].str();

      out.type = 2;
      return out;
    }
  }

  return std::nullopt;
}

class MyComponent : public Component {
 private:

 public:
  MyComponent(std::vector<ParsedLine> lines): lines_(lines) {

    Add(&log_level_checkbox_container_);
    std::set<std::wstring> log_level;
    for(auto& it : lines_)
      log_level.insert(it.level);
    for(auto& it : log_level) {
      auto checkbox = std::make_unique<CheckBox>();
      checkbox->label = it;
      checkbox->state = true;
      log_level_checkbox_container_.Add(checkbox.get());
      log_level_checkbox.push_back(std::move(checkbox));
    }
  }

  Container log_level_checkbox_container_ = Container::Vertical();
  
  std::vector<std::unique_ptr<CheckBox>> log_level_checkbox;

  Element Render() override {
    Elements list;
    int size_level = 0;
    int size_file = 0;

    std::set<std::wstring> allowed_level;
    for(auto& it : log_level_checkbox) {
      if (it->state)
        allowed_level.insert(it->label);
    }
    std::vector<ParsedLine> filtered_lines;
    for(auto& it : lines_) {
      if (allowed_level.count(it.level))
        filtered_lines.push_back(it);
    }

    for (auto& it : filtered_lines) {
      size_level = std::max(size_level, (int)it.level.size());
      size_file = std::max(size_file, (int)it.file.size());
    }

    auto header = hbox({
        text(L"Level") | size(WIDTH, EQUAL, size_level) | notflex,
        separator(),
        text(L"File") | size(WIDTH, EQUAL, size_file),
        separator(),
        text(L"Message") | flex | size(WIDTH, EQUAL, 10),
        filler(),
        separator(),
        text(L"Time") | dim | size(WIDTH, EQUAL, 14) | notflex,
    });

    int previous_type = -1;

    list.push_back(header);

    for (auto& it : filtered_lines) {
      if (previous_type != it.type)
        list.push_back(separator());
      previous_type = it.type;

      Decorator line_decorator = log_style[it.level].line_decorator;
      Decorator level_decorator = log_style[it.level].level_decorator;
      Element document =  //
          hbox({
              text(it.level) | size(WIDTH, EQUAL, size_level) | level_decorator |
                  notflex,

              separator(),

              hbox({
                text(it.file),
                text(it.line) | dim,
              }) | size(WIDTH, EQUAL, size_file),


              separator(),

              hbox(paragraph(it.log)) | size(WIDTH, EQUAL, 100) | flex,

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
                  size(WIDTH, EQUAL, 14) | notflex,
          }) |
          flex | line_decorator;
      list.push_back(document);
    }

    return vbox({
        hbox({
          window(text(L"Log level"), log_level_checkbox_container_.Render()),
          filler(),
        }),
        window(text(L"Log"),
          vbox(list) | frame
        ) | notflex,
        filler(),
    });
  }

  std::vector<ParsedLine> lines_;
};

int main(int argument_count, char** arguments) {
  std::vector<std::wstring> lines;

  // Read file, line by line.
  std::wstring line;
  std::wifstream file(arguments[1]);
  while (std::getline(file, line))
    lines.push_back(line);

  std::vector<Element> list;

  std::wregex log_line_regex(
      LR"(^\[(\d+):(\d+):(\d\d)(\d\d)\/(\d\d)(\d\d)(\d\d)\.(\d+):(\d+):(\w+):(.+)\((\d+)\)\] (.*)$)");

  // Parse every lines.
  std::vector<ParsedLine> parsed_lines;
  for (auto& line : lines) {
    if (auto parsed_line = ParseLine(line))
      parsed_lines.push_back(*parsed_line);
  }

  auto screen = ScreenInteractive::Fullscreen();
  MyComponent component(parsed_lines);
  screen.Loop(&component);

  return EXIT_SUCCESS;
}
