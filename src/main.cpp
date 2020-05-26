#include <unistd.h>

#include <cstdio>
#include <fstream>
#include <ftxui/component/receiver.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/screen/string.hpp>
#include <string>
#include <thread>

#include "ui/main_component.hpp"

// Read from the file descriptor |fd|. Produce line of log and send them to
// |sender|. The |screen| wakes up when this happens.
void LineProducer(int fd,
                  Sender<std::wstring> sender,
                  ScreenInteractive* screen) {
  std::string line;
  int buffer_size = 1 << 10;
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

    // Refresh the screen:
    screen->PostEvent(Event::Custom);

    if (c == 0) {
      using namespace std::chrono_literals;
      std::this_thread::sleep_for(0.5s);
    }
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
  // Reroute stdin to /dev/tty to handle user input.
  stdin = freopen("/dev/tty", "r", stdin);

  auto line_receiver = MakeReceiver<std::wstring>();
  auto line_sender = line_receiver->MakeSender();

  auto screen = ScreenInteractive::Fullscreen();
  MainComponent component(std::move(line_receiver));

  // Read from the log file.
  std::thread line_producer_thread(LineProducer, log_fd, std::move(line_sender),
                                   &screen);
  screen.Loop(&component);

  close(log_fd);
  return EXIT_SUCCESS;
}
