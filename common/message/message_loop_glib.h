/**
 * @file message_loop_glib.h
 * @brief
 *
 */

#ifndef MESSAGE_LOOP_GLIB_H
#define MESSAGE_LOOP_GLIB_H

#ifndef __cplusplus
#   error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "message_loop_proxy.h"
#include "base/task_runner.h"
// #include "callback_forward.h"
#include "glib.h"

/**
 * @brief
 *
 * @class message_loop_glib
 */

class message_loop_glib: public message_loop_proxy
{

public:

    message_loop_glib();

    ~message_loop_glib();

    bool attach();

    bool detach();

    bool postTask(const Closure& task);

private:

    int m_event;

    guint m_source;

    task_runner m_runner;
};

#endif // MESSAGE_LOOP_GLIB_H
/* EOF */
