#ifndef UI_MAIN_COMPONENT_HPP
#define UI_MAIN_COMPONENT_HPP

#include <ftxui/component/checkbox.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/container.hpp>
#include <ftxui/component/receiver.hpp>
#include <ftxui/component/component.hpp>
#include <map>

#include "data/parsed_line.hpp"
#include "ui/log_displayer.hpp"
#include "ui/info_component.hpp"

using namespace ftxui;

class MainComponent : public ComponentBase {
 public:
  MainComponent(Receiver<std::wstring> receiver);
  Element Render() override;
  bool OnEvent(Event) override;

 private:
  Receiver<std::wstring> receiver_;
  void AddLine(std::wstring line);
  void ComputeTranslatedThreadID(ParsedLine& parsed_line);
  std::vector<ParsedLine> lines_;

  std::map<int, std::pair<std::wstring, std::map<int, std::wstring>>>
      translation_;

  void RegisterLogLevel(const std::wstring& log_level);
  std::map<std::wstring, bool> level_checkbox;

  void RegisterThreadId(const std::wstring& thread_id);
  struct ThreadFilter {
    Component container = Container::Vertical({});
    std::map<std::wstring, bool> checkboxes;
  };
  std::map<int, std::unique_ptr<ThreadFilter>> thread_filters_;
  std::vector<int> thread_filters_order_;

  int tab_selected_ = 0;
  std::vector<std::wstring> tab_entries_ = {
      L"Filter",
      L"Fullscreen",
      L"Info",
  };
  Component toggle_ = Toggle(&tab_entries_, &tab_selected_);

  Component container_level_filter_ = Container::Vertical({});
  Component container_thread_filter_ = Container::Horizontal({});
  std::shared_ptr<LogDisplayer> log_displayer_1_ = Make<LogDisplayer>();
  std::shared_ptr<LogDisplayer> log_displayer_2_ = Make<LogDisplayer>();
  Component info_component_ = Make<InfoComponent>();
};

#endif /* end of include guard: UI_MAIN_COMPONENT_HPP */
