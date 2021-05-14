#ifndef UI_LOG_DISPLAYER_HPP
#define UI_LOG_DISPLAYER_HPP

#include <ftxui/component/component.hpp>

#include "data/parsed_line.hpp"

using namespace ftxui;

class LogDisplayer : public ComponentBase {
 public:
  LogDisplayer() = default;
  Element RenderLines(std::vector<ParsedLine*> lines);
  bool OnEvent(Event) override;
  int selected() { return selected_; }

 private:
  int selected_ = 0;
  int size = 0;
};

#endif /* end of include guard: UI_LOG_DISPLAYER_HPP */
