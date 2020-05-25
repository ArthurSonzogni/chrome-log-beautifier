#include <unistd.h>

#include <cstdio>
#include <fstream>
#include <ftxui/component/receiver.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/screen/string.hpp>
#include <string>
#include <thread>

#include "ui/main_component.hpp"

void LineProducer(int fd, Sender<std::wstring> sender) {
  std::string line;
  int buffer_size = 100;
  char buffer[buffer_size];
  while (true) {
    int c = read(fd, buffer, buffer_size);
    for (int i = 0; i < c; ++i) {
      if (buffer[i] == '\n') {
        sender->Send(to_wstring(line));
        line = "";
      } else {
        line.push_back(buffer[i]);
      }
    }

    if (c == 0) {
      using namespace std::chrono_literals;
      std::this_thread::sleep_for(0.5s);
    }
      using namespace std::chrono_literals;
      std::this_thread::sleep_for(0.1s);
  }
}

int main(int argument_count, char** arguments) {

  // Route log_fd to either stdin or the file argument.
  int log_fd = 0;
  if (argument_count == 2) {
    FILE* file = fopen(arguments[1], "r");
    log_fd = dup(fileno(file));
    fclose(file);
  } else {
    log_fd = dup(fileno(stdin));
  }
  // Reroute stdin to /dev/tty
  stdin = freopen("/dev/tty", "r", stdin);

  // Read the file from a thread.
  auto line_receiver = MakeReceiver<std::wstring>();
  std::thread line_producer_thread(LineProducer, log_fd,
                                   line_receiver->MakeSender());


  auto screen = ScreenInteractive::Fullscreen();
  MainComponent component(std::move(line_receiver));

  std::thread refresher([&] {
    while (true) {
      screen.PostEvent(Event::Custom);
      using namespace std::chrono_literals;
      std::this_thread::sleep_for(0.5s);
    }
  });

  screen.Loop(&component);

  close(log_fd);
  return EXIT_SUCCESS;
}
