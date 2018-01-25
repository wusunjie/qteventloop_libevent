#include "message_loop.h"

#include "message_loop_glib.h"

message_loop::message_loop(MESSAGE_LOOP_BACKEND backend)
    :m_proxy(NULL)
{
    switch (backend) {
    case MESSAGE_LOOP_GLIB:
        m_proxy = new (std::nothrow) message_loop_glib();
        break;
    default:
        break;
    }
}

bool message_loop::attach()
{
    if (m_proxy) {
        return m_proxy->attach();
    }
    return false;
}

bool message_loop::detach()
{
    if (m_proxy) {
        return m_proxy->detach();
    }
    return false;
}

bool message_loop::postTask(const Closure& task)
{
    if (m_proxy) {
        return m_proxy->postTask(task);
    }
    return false;
}

message_loop::~message_loop()
{
    delete m_proxy;
}

/* EOF */
