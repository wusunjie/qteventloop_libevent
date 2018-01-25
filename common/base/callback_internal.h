/**
 * @file callback_internal.h
 * @brief
 *
 */

#ifndef CALLBACK_INTERNAL_H_
#define CALLBACK_INTERNAL_H_

#ifndef __cplusplus
#   error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "RefCountedObject.h"
#include <stddef.h>
#include "scoped_refptr.h"

namespace internal
{

/**
 * @brief
 *
 * @class BindStateBase
 */

class BindStateBase: public RefCountedObject
{
protected:
    virtual ~BindStateBase() {}
};

/**
 * @brief
 *
 * @class CallbackBase
 */

class CallbackBase
{
public:

    CallbackBase(const CallbackBase& other);

    CallbackBase& operator = (const CallbackBase& other);

    bool is_null() const;

    void Reset();

protected:

    typedef void(*InvokeFuncStorage)(void);

    bool Equals(const CallbackBase& other) const;

    explicit CallbackBase(BindStateBase* bind_state);

    ~CallbackBase();

    scoped_refptr<BindStateBase> bind_state_;
    InvokeFuncStorage polymorphic_invoke_;
};

template <typename T>
struct CallbackParamTraits
{
    typedef const T& ForwardType;
    typedef T StorageType;
};

template <typename T>
struct CallbackParamTraits<T&>
{
    typedef T& ForwardType;
    typedef T StorageType;
};

template <typename T, size_t n>
struct CallbackParamTraits<T[n]>
{
    typedef const T* ForwardType;
    typedef const T* StorageType;
};

template <typename T>
struct CallbackParamTraits<T[]>
{
    typedef const T* ForwardType;
    typedef const T* StorageType;
};

template <typename T>
T& CallbackForward(T& t)
{
    return t;
}

}  // namespace internal

#endif  // CALLBACK_INTERNAL_H_
/* EOF */
