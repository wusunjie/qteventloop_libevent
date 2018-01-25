/**
 * @file callback_forward.h
 * @brief
 *
 */

#ifndef CALLBACK_FORWARD_H_
#define CALLBACK_FORWARD_H_

#ifndef __cplusplus
#   error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

template <typename Sig>
class Callback;

typedef Callback<void(void)> Closure;

#endif  // CALLBACK_FORWARD_H
/* EOF */
