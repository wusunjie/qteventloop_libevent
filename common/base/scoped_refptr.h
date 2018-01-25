/**
 * @file scoped_refptr.h
 * @brief
 *
 */

#ifndef SCOPED_REFPTR_H
#define SCOPED_REFPTR_H

#ifndef __cplusplus
#   error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

/**
 * @brief
 *
 * @class scoped_refptr
 */

template <class T>
class scoped_refptr
{
public:
    typedef T element_type;

    scoped_refptr() : ptr_(NULL)
    {
    }

    scoped_refptr(T* p) : ptr_(p)
    {
        if (ptr_) {
            ptr_->duplicate();
        }
    }

    scoped_refptr(const scoped_refptr<T>& r) : ptr_(r.ptr_)
    {
        if (ptr_) {
            ptr_->duplicate();
        }
    }

    template <typename U>
    scoped_refptr(const scoped_refptr<U>& r) : ptr_(r.get())
    {
        if (ptr_) {
            ptr_->duplicate();
        }
    }

    ~scoped_refptr()
    {
        if (ptr_) {
            ptr_->release();
        }
    }

    T* get() const
    {
        return ptr_;
    }

    operator T*() const
    {
        return ptr_;
    }

    T* operator->() const
    {
        assert(ptr_ != NULL);
        return ptr_;
    }

    T* release()
    {
        T* retVal = ptr_;
        ptr_ = NULL;
        return retVal;
    }

    scoped_refptr<T>& operator=(T* p)
    {
        if (p) {
            p->duplicate();
        }
        T* old_ptr = ptr_;
        ptr_ = p;
        if (old_ptr) {
            old_ptr->release();
        }
        return *this;
    }

    scoped_refptr<T>& operator=(const scoped_refptr<T>& r)
    {
        return *this = r.ptr_;
    }

    template <typename U>
    scoped_refptr<T>& operator=(const scoped_refptr<U>& r)
    {
        return *this = r.get();
    }

    void swap(T** pp)
    {
        T* p = ptr_;
        ptr_ = *pp;
        *pp = p;
    }

    void swap(scoped_refptr<T>& r)
    {
        swap(&r.ptr_);
    }

protected:
    T* ptr_;
};

#endif // SCOPED_REFPTR_H
/* EOF */
