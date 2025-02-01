//
// Created by apavlov on 25.01.25.
//

#ifndef DMON_SESSION_H
#define DMON_SESSION_H

#include <string>
#include <functional>
#include <mutex>
#include <condition_variable>

#include "diffusion.h"

struct Error {
  int m_code;
  std::string m_message;
};

struct Topic {
  std::string m_topic_type;
  std::string m_path;
  std::vector<char> m_buffer;
  Topic(const std::string& type, const std::string& path, const char* ptr, size_t len);
  Topic(const Topic&) = default;
  Topic() =default;
  Topic(Topic&&) = default;
};

class Session {
public:
  using FetchCompleted = std::function<void()>;
  using FetchError = std::function<void(Error)>;

  static Session& getSession();

  Session();

  bool IsValid() const {
    return m_session != nullptr;
  }

  std::string getSelector() const {
    return m_selector;
  }

  bool connect(const std::string& url, const std::string& principal, const std::string& password, std::string& message);
  void fetch(const std::string& selector);
  void onFetchTopic(Topic&&);
  void onFetchError(Error error);
  void onFetchCompleted(void*);
  void setFetchCompletedCallback(FetchCompleted&&);
  void setFetchErrorCallback(FetchError&&);
  const std::vector<Topic>& getFetchedTopics() const {
    return m_topics;
  }
  ~Session();
  Session(const Session&) = delete;
  Session& operator=(const Session) = delete;
 private:
  std::string m_url;
  std::string m_principal;
  std::string m_password;
  SESSION_T* m_session;
  CREDENTIALS_T *m_credentials;
  std::vector<Topic> m_topics;
  FetchCompleted m_fetch_completed_callback;
  FetchError  m_fetch_error_callback;
  std::mutex m_fetchMtx;
  std::string m_selector;
};

#endif //DMON_SESSION_H
