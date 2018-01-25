/**
 * @file bind.h
 * @brief
 *
 */

#ifndef BIND_H_
#define BIND_H_

#ifndef __cplusplus
#   error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "bind_internal.h"
#include "callback_internal.h"
#include <new>


template <typename Functor>
Callback<
typename internal::BindState<
typename internal::FunctorTraits<Functor>::RunnableType,
typename internal::FunctorTraits<Functor>::RunType,
void()>
::UnboundRunType>
Bind(Functor functor)
{
    typedef typename internal::FunctorTraits<Functor>::RunnableType RunnableType;
    typedef typename internal::FunctorTraits<Functor>::RunType RunType;

    typedef internal::BindState<RunnableType, RunType, void()> BindState;


    return Callback<typename BindState::UnboundRunType>(
                new (std::nothrow) BindState(internal::MakeRunnable(functor)));
}

template <typename Functor, typename P1>
Callback<
typename internal::BindState<
typename internal::FunctorTraits<Functor>::RunnableType,
typename internal::FunctorTraits<Functor>::RunType,
void(typename internal::CallbackParamTraits<P1>::StorageType)>
::UnboundRunType>
Bind(Functor functor, const P1& p1)
{
    typedef typename internal::FunctorTraits<Functor>::RunnableType RunnableType;
    typedef typename internal::FunctorTraits<Functor>::RunType RunType;

    typedef internal::BindState<RunnableType, RunType,
            void(typename internal::CallbackParamTraits<P1>::StorageType)> BindState;

    return Callback<typename BindState::UnboundRunType>(
                new (std::nothrow) BindState(internal::MakeRunnable(functor), p1));
}

template <typename Functor, typename P1, typename P2>
Callback<
typename internal::BindState<
typename internal::FunctorTraits<Functor>::RunnableType,
typename internal::FunctorTraits<Functor>::RunType,
void(typename internal::CallbackParamTraits<P1>::StorageType,
     typename internal::CallbackParamTraits<P2>::StorageType)>
::UnboundRunType>
Bind(Functor functor, const P1& p1, const P2& p2)
{
    typedef typename internal::FunctorTraits<Functor>::RunnableType RunnableType;
    typedef typename internal::FunctorTraits<Functor>::RunType RunType;

    typedef internal::BindState<RunnableType, RunType,
            void(typename internal::CallbackParamTraits<P1>::StorageType,
                 typename internal::CallbackParamTraits<P2>::StorageType)> BindState;


    return Callback<typename BindState::UnboundRunType>(
                new (std::nothrow) BindState(internal::MakeRunnable(functor), p1, p2));
}

template <typename Functor, typename P1, typename P2, typename P3>
Callback<
typename internal::BindState<
typename internal::FunctorTraits<Functor>::RunnableType,
typename internal::FunctorTraits<Functor>::RunType,
void(typename internal::CallbackParamTraits<P1>::StorageType,
     typename internal::CallbackParamTraits<P2>::StorageType,
     typename internal::CallbackParamTraits<P3>::StorageType)>
::UnboundRunType>
Bind(Functor functor, const P1& p1, const P2& p2, const P3& p3)
{
    typedef typename internal::FunctorTraits<Functor>::RunnableType RunnableType;
    typedef typename internal::FunctorTraits<Functor>::RunType RunType;

    typedef internal::BindState<RunnableType, RunType,
            void(typename internal::CallbackParamTraits<P1>::StorageType,
                 typename internal::CallbackParamTraits<P2>::StorageType,
                 typename internal::CallbackParamTraits<P3>::StorageType)> BindState;

    return Callback<typename BindState::UnboundRunType>(
                new (std::nothrow) BindState(internal::MakeRunnable(functor), p1, p2, p3));
}

template <typename Functor, typename P1, typename P2, typename P3, typename P4>
Callback<
typename internal::BindState<
typename internal::FunctorTraits<Functor>::RunnableType,
typename internal::FunctorTraits<Functor>::RunType,
void(typename internal::CallbackParamTraits<P1>::StorageType,
     typename internal::CallbackParamTraits<P2>::StorageType,
     typename internal::CallbackParamTraits<P3>::StorageType,
     typename internal::CallbackParamTraits<P4>::StorageType)>
::UnboundRunType>
Bind(Functor functor, const P1& p1, const P2& p2, const P3& p3, const P4& p4)
{
    typedef typename internal::FunctorTraits<Functor>::RunnableType RunnableType;
    typedef typename internal::FunctorTraits<Functor>::RunType RunType;

    typedef internal::BindState<RunnableType, RunType,
            void(typename internal::CallbackParamTraits<P1>::StorageType,
                 typename internal::CallbackParamTraits<P2>::StorageType,
                 typename internal::CallbackParamTraits<P3>::StorageType,
                 typename internal::CallbackParamTraits<P4>::StorageType)> BindState;


    return Callback<typename BindState::UnboundRunType>(
                new (std::nothrow) BindState(internal::MakeRunnable(functor), p1, p2, p3, p4));
}

template <typename Functor, typename P1, typename P2, typename P3, typename P4,
          typename P5>
Callback<
typename internal::BindState<
typename internal::FunctorTraits<Functor>::RunnableType,
typename internal::FunctorTraits<Functor>::RunType,
void(typename internal::CallbackParamTraits<P1>::StorageType,
     typename internal::CallbackParamTraits<P2>::StorageType,
     typename internal::CallbackParamTraits<P3>::StorageType,
     typename internal::CallbackParamTraits<P4>::StorageType,
     typename internal::CallbackParamTraits<P5>::StorageType)>
::UnboundRunType>
Bind(Functor functor, const P1& p1, const P2& p2, const P3& p3, const P4& p4,
     const P5& p5)
{
    typedef typename internal::FunctorTraits<Functor>::RunnableType RunnableType;
    typedef typename internal::FunctorTraits<Functor>::RunType RunType;

    typedef internal::BindState<RunnableType, RunType,
            void(typename internal::CallbackParamTraits<P1>::StorageType,
                 typename internal::CallbackParamTraits<P2>::StorageType,
                 typename internal::CallbackParamTraits<P3>::StorageType,
                 typename internal::CallbackParamTraits<P4>::StorageType,
                 typename internal::CallbackParamTraits<P5>::StorageType)> BindState;


    return Callback<typename BindState::UnboundRunType>(
                new (std::nothrow) BindState(internal::MakeRunnable(functor), p1, p2, p3, p4, p5));
}

template <typename Functor, typename P1, typename P2, typename P3, typename P4,
          typename P5, typename P6>
Callback<
typename internal::BindState<
typename internal::FunctorTraits<Functor>::RunnableType,
typename internal::FunctorTraits<Functor>::RunType,
void(typename internal::CallbackParamTraits<P1>::StorageType,
     typename internal::CallbackParamTraits<P2>::StorageType,
     typename internal::CallbackParamTraits<P3>::StorageType,
     typename internal::CallbackParamTraits<P4>::StorageType,
     typename internal::CallbackParamTraits<P5>::StorageType,
     typename internal::CallbackParamTraits<P6>::StorageType)>
::UnboundRunType>
Bind(Functor functor, const P1& p1, const P2& p2, const P3& p3, const P4& p4,
     const P5& p5, const P6& p6)
{
    typedef typename internal::FunctorTraits<Functor>::RunnableType RunnableType;
    typedef typename internal::FunctorTraits<Functor>::RunType RunType;

    typedef internal::BindState<RunnableType, RunType,
            void(typename internal::CallbackParamTraits<P1>::StorageType,
                 typename internal::CallbackParamTraits<P2>::StorageType,
                 typename internal::CallbackParamTraits<P3>::StorageType,
                 typename internal::CallbackParamTraits<P4>::StorageType,
                 typename internal::CallbackParamTraits<P5>::StorageType,
                 typename internal::CallbackParamTraits<P6>::StorageType)> BindState;

    return Callback<typename BindState::UnboundRunType>(
                new (std::nothrow) BindState(internal::MakeRunnable(functor), p1, p2, p3, p4, p5, p6));
}

template <typename Functor, typename P1, typename P2, typename P3, typename P4,
          typename P5, typename P6, typename P7>
Callback<
typename internal::BindState<
typename internal::FunctorTraits<Functor>::RunnableType,
typename internal::FunctorTraits<Functor>::RunType,
void(typename internal::CallbackParamTraits<P1>::StorageType,
     typename internal::CallbackParamTraits<P2>::StorageType,
     typename internal::CallbackParamTraits<P3>::StorageType,
     typename internal::CallbackParamTraits<P4>::StorageType,
     typename internal::CallbackParamTraits<P5>::StorageType,
     typename internal::CallbackParamTraits<P6>::StorageType,
     typename internal::CallbackParamTraits<P7>::StorageType)>
::UnboundRunType>
Bind(Functor functor, const P1& p1, const P2& p2, const P3& p3, const P4& p4,
     const P5& p5, const P6& p6, const P7& p7)
{
    typedef typename internal::FunctorTraits<Functor>::RunnableType RunnableType;
    typedef typename internal::FunctorTraits<Functor>::RunType RunType;

    typedef internal::BindState<RunnableType, RunType,
            void(typename internal::CallbackParamTraits<P1>::StorageType,
                 typename internal::CallbackParamTraits<P2>::StorageType,
                 typename internal::CallbackParamTraits<P3>::StorageType,
                 typename internal::CallbackParamTraits<P4>::StorageType,
                 typename internal::CallbackParamTraits<P5>::StorageType,
                 typename internal::CallbackParamTraits<P6>::StorageType,
                 typename internal::CallbackParamTraits<P7>::StorageType)> BindState;


    return Callback<typename BindState::UnboundRunType>(
                new (std::nothrow) BindState(internal::MakeRunnable(functor), p1, p2, p3, p4, p5, p6,
                              p7));
}

#endif  // BIND_H_
/* EOF */
