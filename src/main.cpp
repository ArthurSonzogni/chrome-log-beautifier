#include <unistd.h>
#include <stdlib.h>



#include <fstream>
#include <ftxui/component/receiver.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/screen/string.hpp>
#include <string>
#include <thread>

#include "args.h"
#include <iostream>

#include "ui/main_component.hpp"
#include "data/session.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"

#include <chrono>

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

// selector https://docs.diffusiondata.com/docs/6.1.5/manual/html-single/diffusion_single.html#topic_selector_unified

ARG_OPTS_T arg_opts[] = {
    ARG_OPTS_HELP,
    {'u', "url", "Diffusion server URL", ARG_OPTIONAL, ARG_HAS_VALUE, "ws://localhost:8080"},
    {'p', "principal", "Principal (username) for the connection", ARG_OPTIONAL, ARG_HAS_VALUE, "client"},
    {'c', "credentials", "Credentials (password) for the connection", ARG_OPTIONAL, ARG_HAS_VALUE, "password"},
    {'d', "delay", "Delay between reconnection attempts, in ms", ARG_OPTIONAL, ARG_HAS_VALUE, "2000" },
    {'r', "retries", "Reconnection retry attempts", ARG_OPTIONAL, ARG_HAS_VALUE, "5" },
    {'t', "timeout", "Reconnection timeout for a disconnected session", ARG_OPTIONAL, ARG_HAS_VALUE, NULL },
    {'s', "sleep", "Time to sleep before disconnecting (in seconds).", ARG_OPTIONAL, ARG_HAS_VALUE, "5" },
    {'f', "selector", "Diffusion selector - fetch topics by path, \"?Phoenix/Outcomes/.*//\"", ARG_OPTIONAL, ARG_HAS_VALUE, nullptr},
    END_OF_ARG_OPTS
};

int main(int argc, char** argv) {

  try
  {
    auto logger = spdlog::basic_logger_mt("basic_logger", "logs/infolog.txt", true);
    spdlog::set_default_logger(logger);
  }
  catch (const spdlog::spdlog_ex &ex)
  {
    std::cerr << "Log init failed: " << ex.what() << std::endl;
    return 1;
  }

  spdlog::set_level(spdlog::level::debug); // Set global log level to debug

  spdlog::info("Welcome to spdlog!");
  spdlog::error("Some error message with arg: {}", 1);

  spdlog::warn("Easy padding in numbers like {:08d}", 12);
  spdlog::critical("Support for int: {0:d};  hex: {0:x};  oct: {0:o}; bin: {0:b}", 42);
  spdlog::info("Support for floats {:03.2f}", 1.23456);
  spdlog::info("Positional args are {1} {0}..", "too", "supported");
  spdlog::info("{:<30}", "left aligned");

  spdlog::debug("This message should be displayed..");


  /*
    * Standard command-line parsing.
    */
    HASH_T *options = parse_cmdline(argc, argv, arg_opts);
    if(options == nullptr || hash_get(options, "help") != nullptr) {
        show_usage(argc, argv, arg_opts);
        return EXIT_FAILURE;
    }

    const char *url = static_cast<const char*>(hash_get(options, "url"));
    const char *principal = static_cast<const char*>(hash_get(options, "principal"));
    //CREDENTIALS_T *credentials = nullptr;
    const char *password = static_cast<const char*>(hash_get(options, "credentials"));
    //if(password != NULL) {
    //        credentials = credentials_create_password(password);
    //}

    //long retry_delay = std::atol(static_cast<const char*>(hash_get(options, "delay")));
    //long retry_count = std::atoi(static_cast<const char*>(hash_get(options, "retries")));
    long reconnect_timeout;
    if(hash_get(options, "timeout") != NULL) {
      reconnect_timeout = std::atol(static_cast<const char*>(hash_get(options, "timeout")));
    }
    else {
      reconnect_timeout = -1;
    }

    //const unsigned int sleep_time = std::atol(static_cast<const char*>(hash_get(options, "sleep")));
    //const char* selector = static_cast<const char*>(hash_get(options,"selector"));

    spdlog::info("application has started url {} principal {} password {}", url, principal, reconnect_timeout);
    auto& session = Session::getSession();
    std::string connectError;
    if (!session.connect(url, principal, password, connectError)) {
      std::cout << "Can not connect " << connectError << std::endl;
      //return 1;
    }

    //assert(session.IsValid());

  //auto line_receiver = MakeReceiver<std::wstring>();
  //auto line_sender = line_receiver->MakeSender();

  auto screen = ScreenInteractive::Fullscreen();

  session.setFetchCompletedCallback([&screen]() {
    screen.PostEvent(Event::Special("fetch"));
  });

  session.setFetchErrorCallback([&screen](Error error) {
    screen.PostEvent(Event::Special("fetch"));
  });

  //std::atomic<bool> exit{false};

  auto component = std::make_shared<MainComponent>(session, screen.ExitLoopClosure());

  /*std::thread producer = std::thread([&exit, &screen](){
    int counter{0};
    while(!exit) {

      using namespace std::chrono_literals;
      std::this_thread::sleep_for(2s);
      counter = counter + 2;
      {
        std::lock_guard<std::mutex> lk(MainComponent::test_lock);
        MainComponent::test_data = "New Data " + std::to_string(counter) + "\n" + "Some text later";
      }
      screen.PostEvent(Event::Custom);
    }
  });
   */

  // Read from the log file.
  //std::thread line_producer_thread(LineProducer, log_fd, std::move(line_sender),
  //                                 &screen);

  screen.Loop(component);
  //exit = true;
  //producer.join();
  spdlog::info("finished");

  return EXIT_SUCCESS;
}
