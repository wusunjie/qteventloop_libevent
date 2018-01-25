/**
 * @file bind_helper.h
 * @brief
 *
 */

#ifndef BIND_HELPERS_H_
#define BIND_HELPERS_H_

#ifndef __cplusplus
#   error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "callback.h"

namespace internal
{

/**
 * @brief
 *
 * @class UnretainedWrapper
 */

template <typename T>
class UnretainedWrapper
{
public:
    explicit UnretainedWrapper(T* o) : ptr_(o)
    {
    }

    T* get() const { return ptr_; }

private:
    T* ptr_;
    UnretainedWrapper(const UnretainedWrapper&);
    void operator = (const UnretainedWrapper&);
};

/**
 * @brief
 *
 * @class ConstRefWrapper
 */

template <typename T>
class ConstRefWrapper
{
public:
    explicit ConstRefWrapper(const T& o) : ptr_(&o)
    {
    }

    const T& get() const { return *ptr_; }

private:
    const T* ptr_;
    ConstRefWrapper(const ConstRefWrapper&);
    void operator = (const ConstRefWrapper&);
};

template <typename T>
struct IgnoreResultHelper
{
    explicit IgnoreResultHelper(T functor) : functor_(functor)
    {
    }

    T functor_;
};

template <typename T>
struct IgnoreResultHelper<Callback<T> >
{
    explicit IgnoreResultHelper(const Callback<T>& functor) : functor_(functor)
    {
    }

    const Callback<T>& functor_;
};

template <typename T>
struct UnwrapTraits
{
    typedef const T& ForwardType;
    static ForwardType Unwrap(const T& o)
    {
        return o;
    }
};

template <typename T>
struct UnwrapTraits<UnretainedWrapper<T> >
{
    typedef T* ForwardType;
    static ForwardType Unwrap(UnretainedWrapper<T> unretained)
    {
        return unretained.get();
    }
};

template <typename T>
struct UnwrapTraits<ConstRefWrapper<T> >
{
    typedef const T& ForwardType;
    static ForwardType Unwrap(ConstRefWrapper<T> const_ref)
    {
        return const_ref.get();
    }
};

template <typename T>
static inline internal::UnretainedWrapper<T> Unretained(T* o)
{
    return internal::UnretainedWrapper<T>(o);
}

template <typename T>
static inline internal::ConstRefWrapper<T> ConstRef(const T& o)
{
    return internal::ConstRefWrapper<T>(o);
}

template <typename T>
static inline internal::IgnoreResultHelper<T> IgnoreResult(T data)
{
    return internal::IgnoreResultHelper<T>(data);
}

template <typename T>
static inline internal::IgnoreResultHelper<Callback<T> >
IgnoreResult(const Callback<T>& data)
{
    return internal::IgnoreResultHelper<Callback<T> >(data);
}

}  // namespace internal

#endif  // BIND_HELPERS_H_
/* EOF */
