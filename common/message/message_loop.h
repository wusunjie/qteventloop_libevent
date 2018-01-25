/**
 * @file message_loop.h
 * @brief
 *
 */

#ifndef MESSAGE_LOOP_H
#define MESSAGE_LOOP_H

#ifndef __cplusplus
#   error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

// #include "callback_forward.h"
#include "base/callback.h"

class message_loop_proxy;

/**
 * @brief
 *
 * @class message_loop
 */

class message_loop
{

public:

    enum MESSAGE_LOOP_BACKEND
    {
        MESSAGE_LOOP_GLIB
    };

    message_loop(MESSAGE_LOOP_BACKEND backend);

    bool attach();

    bool detach();

    bool postTask(const Closure&);

    ~message_loop();

private:

    message_loop_proxy *m_proxy;
    message_loop(const message_loop&);
    void operator = (const message_loop&);

};

#endif // MESSAGE_LOOP_H
/* EOF */
