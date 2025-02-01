//
// Created by apavlov on 25.01.25.
//

#include <mutex>
#include <vector>
#include <memory>
#include <iostream>
#include "session.h"

std::string getSessionIdAsString(const SESSION_ID_T* session_id)
{
    using unique_cstr_t = std::unique_ptr<char, decltype(&free)>;
    if (auto sessionId = unique_cstr_t(session_id_to_string(session_id), &free))
    {
        return std::string(sessionId.get());
    }

    return std::string();
}

std::string topicType2Str(MESSAGE_TYPE_T mt) {
    switch (mt) {
        case MESSAGE_TYPE_UNDEFINED:
          return "UNDEFINED";
        case MESSAGE_TYPE_TOPIC_LOAD:
          return "TOPIC_LOAD";
        case MESSAGE_TYPE_DELTA:
          return "DELTA";
        case MESSAGE_TYPE_SUBSCRIBE:
          return "SUBSCRIBE";
        case MESSAGE_TYPE_UNSUBSCRIBE:
          return "UNSUBSCRIBE";
        case MESSAGE_TYPE_PING_SERVER:
          return "PING_SERVER";
        case MESSAGE_TYPE_PING_CLIENT:
          return "";
        case MESSAGE_TYPE_CREDENTIALS:
          return "CREDENTIALS";
        case MESSAGE_TYPE_CREDENTIALS_REJECTED:
          return "CREDENTIALS_REJECTED";
        case MESSAGE_TYPE_ABORT_NOTIFICATION:
          return "ABORT_NOTIFICATION";
        case MESSAGE_TYPE_CLOSE_REQUEST:
          return "CLOSE_REQUEST";
        case MESSAGE_TYPE_TOPIC_LOAD_ACK_REQD:
          return "TOPIC_LOAD_ACK_REQD";
        case MESSAGE_TYPE_DELTA_ACK_REQD:
          return "DELTA_ACK_REQD";
        case MESSAGE_TYPE_ACK:
          return "ACK";
        case MESSAGE_TYPE_FETCH:
          return "FETCH";
        case MESSAGE_TYPE_FETCH_REPLY:
          return "FETCH_REPLY";
        case MESSAGE_TYPE_TOPIC_STATUS_NOTIFICATION:
          return "STATUS_NOTIFICATION";
        case MESSAGE_TYPE_COMMAND_MESSAGE:
          return "COMMAND_MESSAGE";
        case MESSAGE_TYPE_COMMAND_TOPIC_LOAD:
          return "COMMAND_TOPIC_LOAD";
        case MESSAGE_TYPE_COMMAND_TOPIC_NOTIFICATION:
          return "COMMAND_TOPIC_NOTIFICATION";
        default:
          return "???";
    }
}

std::string gen_random(const int len) {
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    std::string tmp_s;
    tmp_s.reserve(len);

    for (int i = 0; i < len; ++i) {
        tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
    }

    return tmp_s;
}


Topic::Topic(const std::string& type, const std::string& path, const char* ptr, size_t len): m_topic_type(type), m_path(path) {
    m_buffer.reserve(len);
    if (ptr) {
        std::copy(ptr, ptr + len, std::back_inserter(m_buffer));
    }
}

/*
 * This is the callback that is invoked if the client can successfully
 * connect to Diffusion, and a session instance is ready for use.
 */
/*static int on_connected(SESSION_T *session) {
    global_session = session;
    std::invoke(global_connect, std::string());
    return HANDLER_SUCCESS;
}
*/
/*
 * This is the callback that is invoked if there is an error connection
 * to the Diffusion instance.
 */
/*static int on_error(SESSION_T *session, DIFFUSION_ERROR_T *error) {
    global_session = session;
    std::invoke(global_connect, std::string(error->message));
    return HANDLER_SUCCESS;
}*/

/*
 * This callback is used when the session state changes, e.g. when a session
 * moves from a "connecting" to a "connected" state, or from "connected" to
 * "closed".
 */
static void on_session_state_changed (
        SESSION_T *session,
        const SESSION_STATE_T old_state,
        const SESSION_STATE_T new_state)
{
}


static int on_fetch(SESSION_T *session, void *context) {
    auto s1 = static_cast<Session*>(context);
    s1->onFetchTopic(Topic("COMPL", s1->getSelector(), nullptr, 0));
    Session::getSession().onFetchCompleted(context);
    return HANDLER_SUCCESS;
}

static int on_topic(struct session_s *session, const TOPIC_MESSAGE_T *message) {
    if (message) {
        auto& s = Session::getSession();
        s.onFetchTopic(Topic(topicType2Str(message->type), message->name,
                             message->payload->data, message->payload->len));
        //buf_free(message->payload);
        return HANDLER_SUCCESS;
    }
    return HANDLER_SUCCESS;
    //return HANDLER_FAILURE;
}

static int on_fetch_error(SESSION_T * session, const DIFFUSION_ERROR_T *error) {
    if (error != nullptr) {
        auto& s1 = Session::getSession();
        s1.onFetchTopic(Topic("ERROR", error->message, nullptr, 0));
        Session::getSession().onFetchError(
            Error{static_cast<int>(error->code), std::string(error->message)});
        return HANDLER_SUCCESS;
    }

    return HANDLER_FAILURE;
}

static int on_fetch_status_message(SESSION_T *session,
                                   const SVC_FETCH_STATUS_RESPONSE_T *status,
                                   void *context)
{
    auto s1 = static_cast<Session*>(context);
    s1->onFetchTopic(Topic("FETCHSTAT", s1->getSelector() + status->topic_path, nullptr, 0));
    return HANDLER_SUCCESS;
}

static int on_fetch_discard(struct session_s *session, void *context)
{
    auto s1 = static_cast<Session*>(context);
    s1->onFetchTopic(Topic("DISK", s1->getSelector(), nullptr, 0));
    return HANDLER_SUCCESS;
}

/*void Session::fetch(const std::string& selector)
{
    if (m_session_handle)
    {
        FETCH_PARAMS_T params;
        params.on_fetch = &on_fetch;
        params.on_topic_message = &on_topic;
        params.selector = selector.c_str();
        params.on_error = &on_fetch_error;
        params.on_status_message = &on_fetch_status_message;
        params.on_discard = &on_fetch_discard;
        params.context = this;
        ::fetch(m_session_handle, params);
    }
}

void Session::subscribe(const std::string& selector)
{

}
*/


/*
void Session::Private::fetch(const std::string& selector, OnFetch&& callback)
{
    FETCH_PARAMS_T params;
    params.on_fetch = &on_fetch;
    params.on_topic_message = &on_topic;
    params.selector = selector.c_str();
    params.on_error = &on_fetch_error;
    params.on_status_message = &on_fetch_status_message;
    params.on_discard = &on_fetch_discard;
    params.context = this;
    global_fetch = std::move(callback);
    ::fetch(global_session, params);
}*/

Session& Session::getSession() {
    static Session ses;
    return ses;
}

Session::Session() : m_session(nullptr)
      , m_fetch_completed_callback(nullptr)
      , m_fetch_error_callback(nullptr)
{

}

Session::~Session()
{
    if (m_session) {
        DIFFUSION_ERROR_T error;
        memset(&error, 0, sizeof(error));
        session_close(m_session, &error);
        session_free(m_session);
    }

    if (m_credentials) {
        credentials_free(m_credentials);
    }
}

bool Session::connect(const std::string& url, const std::string& principal, const std::string& password, std::string& message) {
    static RECONNECTION_STRATEGY_T m_rec_strategy;
    m_url = url;
    m_principal = principal;
    m_password = password;
    m_credentials = credentials_create_password(m_password.c_str());
    static SESSION_LISTENER_T session_listener;
    session_listener.on_state_changed = &on_session_state_changed;
    session_listener.on_handler_error = nullptr;

    m_rec_strategy.retry_count = 3;
    m_rec_strategy.retry_delay = 1000;

    static DIFFUSION_ERROR_T error;
    memset(&error, 0, sizeof(error));

    //session_create_async(m_url.c_str(), m_principal.c_str(), global_credentials, &session_listener, &reconnection_strategy, &callbacks, &error);

    auto session = session_create(m_url.c_str(), m_principal.c_str(), m_credentials, &session_listener, &m_rec_strategy, &error);
    if(session != nullptr) {
        auto& s = getSession();
        s.m_session = session;
        /*char *sid_str = session_id_to_string(session->id);
        printf("Session created (state=%d, id=%s)\n",
               session_state_get(session),
               sid_str);
        free(sid_str);*/
    }
    else {
        message = error.message?error.message:"";
        free(error.message);
    }

    return getSession().m_session != nullptr;
}

void Session::fetch(const std::string& selector)
{
    if (m_session) {
        FETCH_PARAMS_T params;
        params.on_fetch = &on_fetch;
        params.on_topic_message = &on_topic;
        params.selector = selector.c_str();
        params.on_error = &on_fetch_error;
        params.on_status_message = &on_fetch_status_message;
        params.on_discard = &on_fetch_discard;
        params.context = this;
        m_selector = selector;
        ::fetch(m_session, params);
        std::cout << "fetch\n";
    }
    else {
        for (int i = 0; i < 10; ++i) {
          m_topics.emplace_back("TOPIC_LOAD",
                                gen_random(40), nullptr, 0);
        }
        onFetchCompleted(nullptr);
    }
}

void Session::onFetchTopic(Topic&& t) {
  std::lock_guard<std::mutex> lk(m_fetchMtx);
  m_topics.push_back(std::move(t));
}

void Session::onFetchError(Error error) {

  if (m_fetch_error_callback) {
    m_fetch_error_callback(error);
  }
}

void Session::onFetchCompleted(void*) {
    if (m_fetch_completed_callback) {
        m_fetch_completed_callback();
    }
}

void Session::setFetchCompletedCallback(FetchCompleted&& fc) {
    m_fetch_completed_callback = std::move(fc);
}

void Session::setFetchErrorCallback(FetchError&& fe) {
    m_fetch_error_callback = std::move(fe);
}