#ifndef UI_MAIN_COMPONENT_HPP
#define UI_MAIN_COMPONENT_HPP

#include <ftxui/component/checkbox.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/container.hpp>
#include <ftxui/component/receiver.hpp>
#include <ftxui/component/toggle.hpp>
#include <map>

#include "data/parsed_line.hpp"
#include "ui/log_displayer.hpp"
#include "ui/info_component.hpp"

using namespace ftxui;

class MainComponent : public Component {
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
  std::map<std::wstring, std::unique_ptr<CheckBox>> level_checkbox;

  void RegisterThreadId(const std::wstring& thread_id);
  struct ThreadFilter {
    Container container = Container::Vertical();
    std::map<std::wstring, std::unique_ptr<CheckBox>> checkboxes;
  };
  std::map<int, std::unique_ptr<ThreadFilter>> thread_filters_;
  std::vector<int> thread_filters_order_;

  /**/ Container container_main_ = Container::Vertical();
  /****/ Toggle toggle_;
  /****/ Container tab_ = Container::Tab(&toggle_.selected);
  /******/ Container tab_filter_ = Container::Vertical();
  /********/ Container tab_filter_tools_ = Container::Horizontal();
  /**********/ Container container_level_filter_ = Container::Vertical();
  /**********/ Container container_thread_filter_ = Container::Horizontal();
  /*******/  LogDisplayer log_displayer_[2];
  /*******/  InfoComponent info_component_;
};

#endif /* end of include guard: UI_MAIN_COMPONENT_HPP */
