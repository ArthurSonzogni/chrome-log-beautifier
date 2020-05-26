#include "ui/main_component.hpp"

#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/string.hpp>
#include <set>

using namespace ftxui;

MainComponent::MainComponent(Receiver<std::wstring> receiver)
    : receiver_(std::move(receiver)) {
  /**/ Add(&container_main_);
  /****/ container_main_.Add(&toggle_);
  /****/ container_main_.Add(&tab_);
  /******/ tab_.Add(&tab_filter_);
  /********/ tab_filter_.Add(&tab_filter_tools_);
  /**********/ tab_filter_tools_.Add(&container_level_filter_);
  /**********/ tab_filter_tools_.Add(&container_thread_filter_);
  /********/ tab_filter_.Add(&log_displayer_[0]);
  /******/ tab_.Add(&log_displayer_[1]);
  /******/ tab_.Add(&info_component_);

  //toggle_.focused_style = bold;
  //toggle_.selected_style = nothing;
  //toggle_.normal_style = dim;
  toggle_.entries = {
      L"Filter",
      L"Fullscreen",
      L"Info",
  };
}

bool MainComponent::OnEvent(Event event) {
  while (receiver_->HasPending()) {
    std::wstring line;
    receiver_->Receive(&line);
    AddLine(line);
  }
  return Component::OnEvent(event);
}

void MainComponent::AddLine(std::wstring line) {
  lines_.push_back(ParsedLine::Parse(line));
  ParsedLine& parsed_line = lines_.back();

  ComputeTranslatedThreadID(parsed_line);
  RegisterLogLevel(parsed_line.level);
  RegisterThreadId(parsed_line.translated_thread_id);
}

void MainComponent::ComputeTranslatedThreadID(ParsedLine& parsed_line) {
  if (parsed_line.process_id == 0) {
    parsed_line.translated_thread_id = L"None";
    return;
  }

  if (!translation_.count(parsed_line.process_id))
    translation_[parsed_line.process_id].first = U'A' + translation_.size();
  auto& translation_thread = translation_[parsed_line.process_id].second;
  if (!translation_thread.count(parsed_line.thread_id)) {
    translation_thread[parsed_line.thread_id] =
        to_wstring(translation_thread.size());
  }
  parsed_line.translated_thread_id =
      translation_[parsed_line.process_id].first +
      translation_thread[parsed_line.thread_id];
}

void MainComponent::RegisterLogLevel(const std::wstring& log_level) {
  auto& checkbox = level_checkbox[log_level];
  if (checkbox)
    return;

  checkbox = std::make_unique<CheckBox>();
  checkbox->label = log_level;
  checkbox->state = log_level != L"UNKNOWN";
  container_level_filter_.Add(checkbox.get());
}

void MainComponent::RegisterThreadId(const std::wstring& thread_id) {
  int key = (int)thread_id[0];
  auto& column = thread_filters_[key];
  if (!column) {
    column = std::make_unique<ThreadFilter>();
    container_thread_filter_.Add(&(column->container));
    thread_filters_order_.push_back(key);
  }

  auto& checkbox = column->checkboxes[thread_id];
  if (checkbox)
    return;

  checkbox = std::make_unique<CheckBox>();
  checkbox->label = thread_id;
  checkbox->state = true;
  column->container.Add(checkbox.get());
}

Element MainComponent::Render() {
  static int i = 0;
  std::set<std::wstring> allowed_level;
  std::set<std::wstring> allowed_thread;
  for (auto& [level, checkbox] : level_checkbox) {
    if (checkbox->state)
      allowed_level.insert(level);
  }
  for (auto key : thread_filters_order_) {
    for (auto& [id, checkbox] : thread_filters_[key]->checkboxes) {
      if (checkbox->state)
        allowed_thread.insert(id);
    }
  }
  std::vector<ParsedLine*> filtered_lines;
  filtered_lines.reserve(lines_.size());

  for(auto it = lines_.begin(); it != lines_.end(); ++it) {
    if (allowed_level.count(it->level) &&
        allowed_thread.count(it->translated_thread_id)) {
      filtered_lines.push_back(&(*it));
    }
  }

  Elements thread_filter_document;
  thread_filter_document.push_back(vbox({
    text(L"Process:") | bold,
    separator(),
    text(L"Thread:") | bold,
    filler()
  }));
  for (auto key : thread_filters_order_) {
    Elements checkboxes;
    wchar_t c = key;
    std::wstring c_str;
    c_str += c;
    checkboxes.push_back(text(c_str));
    checkboxes.push_back(separator());
    for (auto& [id, checkbox] : thread_filters_[key]->checkboxes)
      checkboxes.push_back(checkbox->Render());
    thread_filter_document.push_back(separator());
    thread_filter_document.push_back(vbox(checkboxes));
  }

  int current_line = log_displayer_[std::min(toggle_.selected, 1)].selected();

  auto header = hbox({
    text(L"chrome-log-beautifier"),
    filler(),
    toggle_.Render(),
    filler(),
    text(L"["),
    text(to_wstring(current_line)),
    text(L"] "),
    text(to_wstring(filtered_lines.size())),
    text(L"/"),
    text(to_wstring(lines_.size())),
    filler(),
    spinner(5, i++),
  });

  Element tab_menu;
  if (toggle_.selected == 0) {
    return  //
        vbox({
            header,
            separator(),
            hbox({
                window(text(L"Type"), container_level_filter_.Render()) |
                    notflex,
                text(L" "),
                window(text(L"Filter"), hbox(thread_filter_document)) | notflex,
                filler(),
            }) | notflex,
            log_displayer_[0].Render(filtered_lines) | notflex,
        });
  }

  if (toggle_.selected == 1) {
    return  //
        vbox({
            header,
            log_displayer_[1].Render(filtered_lines) | notflex,
            filler(),
        });
  }

  return  //
      vbox({
          header,
          separator(),
          filler(),
          info_component_.Render() | center,
          filler(),
      });
}
