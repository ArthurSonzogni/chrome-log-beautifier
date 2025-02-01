#ifndef UI_MAIN_COMPONENT_HPP
#define UI_MAIN_COMPONENT_HPP

#include <ftxui/component/component.hpp>
#include <ftxui/component/component.hpp>
#include "ftxui/component/component_base.hpp"
#include "ftxui/component/component_options.hpp"  // for InputOption
#include <ftxui/component/task.hpp>
#include <map>

#include "ui/log_displayer.hpp"
#include "ui/info_component.hpp"

#include "data/session.h"

using namespace ftxui;


class MainComponent : public ComponentBase {
 public:
  static std::mutex test_lock;
  static std::string test_data;
  MainComponent(Session& session, Closure&& screenExit);
  Element Render() override;
  bool OnEvent(Event) override;

  bool isF() const {
    return log_displayer_1_->Focusable();
  }

 private:
  Closure m_screen_exit_;
  std::string m_text = "Some content\nLne2\nLine4\n Finish line here";
  std::string m_search_selector;

  std::map<int, std::pair<std::wstring, std::map<int, std::wstring>>>
      translation_;

  int tab_selected_ = 0;
  std::vector<std::wstring> tab_entries_ = {
      L"Search",
      L"Subscription",
      L"Info",
  };
  Component toggle_ = Toggle(&tab_entries_, &tab_selected_);
  Component container_level_filter_ = Container::Vertical({});
  Component container_thread_filter_ = Container::Horizontal({});
  std::shared_ptr<LogDisplayer> log_displayer_1_;
  std::shared_ptr<LogDisplayer> log_displayer_2_;
  Component info_component_ = Make<InfoComponent>();
  Component container_search_selector_ = Input(&m_search_selector, "", InputOption{.multiline=false, .on_change=[&](){
  }, .on_enter = [&](){
    if (!m_search_selector.empty()) {
      m_session.fetch(m_search_selector);
    }
  }});
  Component btn_search_ = Button("Search", [&]{
        if (!m_search_selector.empty()) {
          m_session.fetch(m_search_selector);
        }
      }, ButtonOption::Ascii());
  Component text_box_ = Input(&m_text, InputOption{.multiline = true});
  Component btn_exit_ = Button("Exit", m_screen_exit_, ButtonOption::Ascii());
  Component m_btn_copy_ = Button("Copy", [](){}, ButtonOption::Ascii());
  Component m_btn_clear_ = Button("Clear", [&](){
        m_search_selector.clear();
      }, ButtonOption::Ascii());

  Component m_test = Renderer([&] {
    return window(text("Fetching status"),  text(m_session.isFetchInProgress()?"In progress":"Failed: " + m_session.getLastFetchStatus().m_message)| color(m_session.getLastFetchStatus().m_code!=0?Color::Red:Color::Yellow));
  }) | Maybe([&] { return m_session.isFetchInProgress() || m_session.getLastFetchStatus().m_code != 0; });

  Session& m_session;
};

#endif /* end of include guard: UI_MAIN_COMPONENT_HPP */
