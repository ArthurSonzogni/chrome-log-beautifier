#include "ui/main_component.hpp"

#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/screen/string.hpp>
#include <set>

using namespace ftxui;

MainComponent::MainComponent(Receiver<std::wstring> receiver)
    : receiver_(std::move(receiver)) {
  Add(Container::Vertical({
      toggle_,
      Container::Tab(
          {
              Container::Vertical({
                  Container::Horizontal({
                      container_level_filter_,
                      container_thread_filter_,
                  }),
                  log_displayer_1_,
              }),
              log_displayer_2_,
              info_component_,
          },
          &tab_selected_),
  }));
}

bool MainComponent::OnEvent(Event event) {
  while (receiver_->HasPending()) {
    std::wstring line;
    receiver_->Receive(&line);
    AddLine(line);
  }
  return ComponentBase::OnEvent(event);
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
    parsed_line.translated_thread_id = L" ";
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
  if (level_checkbox.count(log_level))
    return;

  level_checkbox[log_level] = true;
  container_level_filter_->Add(
      Checkbox(log_level.c_str(), &level_checkbox[log_level]));
}

void MainComponent::RegisterThreadId(const std::wstring& thread_id) {
  int key = (int)thread_id[0];
  auto& column = thread_filters_[key];
  if (!column) {
    column = std::make_unique<ThreadFilter>();
    container_thread_filter_->Add(column->container);
    thread_filters_order_.push_back(key);
  }

  if (column->checkboxes.count(thread_id))
    return;

  column->checkboxes[thread_id] = true;
  column->container->Add(Checkbox(thread_id, &column->checkboxes[thread_id]));
}

Element MainComponent::Render() {
  static int i = 0;
  std::set<std::wstring> allowed_level;
  std::set<std::wstring> allowed_thread;
  for (auto& [level, state] : level_checkbox) {
    if (state)
      allowed_level.insert(level);
  }
  for (auto key : thread_filters_order_) {
    for (auto& [thread_id, state] : thread_filters_[key]->checkboxes) {
      if (state)
        allowed_thread.insert(thread_id);
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
    checkboxes.push_back(thread_filters_[key]->container->Render());
    thread_filter_document.push_back(separator());
    thread_filter_document.push_back(vbox(checkboxes));
  }

  int current_line =
      (std::min(tab_selected_, 1) == 0 ? log_displayer_1_ : log_displayer_2_)
          ->selected();

  auto header = hbox({
      text(L"chrome-log-beautifier"),
      hcenter(toggle_->Render()),
      separator(),
      text(to_wstring(current_line)),
      text(L"/"),
      text(to_wstring(filtered_lines.size())),
      text(L"  ["),
      text(to_wstring(lines_.size())),
      text(L"]"),
      separator(),
      gauge(float(current_line) /
            float(std::max(1, (int)filtered_lines.size() - 1))) |
          color(Color::GrayDark),
      separator(),
      spinner(5, i++),
  });

  Element tab_menu;
  if (tab_selected_ == 0) {
    return  //
        vbox({
            header,
            separator(),
            hbox({
                window(text(L"Type"), container_level_filter_->Render()) |
                    notflex,
                text(L" "),
                window(text(L"Filter"), hbox(thread_filter_document)) | notflex,
                filler(),
            }) | notflex,
            log_displayer_1_->RenderLines(filtered_lines) | flex_shrink,
        });
  }

  if (tab_selected_ == 1) {
    return  //
        vbox({
            header,
            log_displayer_2_->RenderLines(filtered_lines) | flex_shrink,
            filler(),
        });
  }

  return  //
      vbox({
          header,
          separator(),
          filler(),
          info_component_->Render() | center,
          filler(),
      });
}
