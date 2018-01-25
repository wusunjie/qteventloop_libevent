/**
 * @file message_loop_proxy.h
 * @brief
 *
 */

#ifndef MESSAGE_LOOP_PROXY_H
#define MESSAGE_LOOP_PROXY_H

#ifndef __cplusplus
#   error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

// #include "callback_forward.h"
#include "base/callback.h"

/**
 * @brief
 *
 * @class message_loop_proxy
 */

class message_loop_proxy
{

public:

    virtual ~message_loop_proxy() {}

    virtual bool attach() = 0;

    virtual bool detach() = 0;

    virtual bool postTask(const Closure&) = 0;

};

#endif // MESSAGE_LOOP_PROXY_H
/* EOF */
