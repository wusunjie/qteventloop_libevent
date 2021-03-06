#include "callback_internal.h"

namespace internal
{

bool CallbackBase::is_null() const
{
    return bind_state_.get() == NULL;
}

void CallbackBase::Reset()
{
    polymorphic_invoke_ = NULL;
    bind_state_.release();
}

bool CallbackBase::Equals(const CallbackBase& other) const
{
    return bind_state_.get() == other.bind_state_.get() &&
            polymorphic_invoke_ == other.polymorphic_invoke_;
}

CallbackBase::CallbackBase(BindStateBase* bind_state)
    : bind_state_(bind_state),
      polymorphic_invoke_(NULL)
{
}

CallbackBase::~CallbackBase()
{
}

CallbackBase::CallbackBase(const CallbackBase &other)
    : bind_state_(other.bind_state_)
    , polymorphic_invoke_(other.polymorphic_invoke_)
{
}

CallbackBase &CallbackBase::operator =(const CallbackBase &other)
{
    if (this == &other) {
        return *this;
    }
    bind_state_ = other.bind_state_;
    polymorphic_invoke_ = other.polymorphic_invoke_;
    return *this;
}

}  // namespace internal
/* EOF */
