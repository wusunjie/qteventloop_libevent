/**
 * @file RefCountedObject.h
 * @brief
 *
 */

#ifndef __REFCOUNTEDOBJECT_H__
#define __REFCOUNTEDOBJECT_H__

#ifndef __cplusplus
#   error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include <atomic>

/**
 * @brief
 *
 * @class RefCountedObject
 */

class RefCountedObject
{
public:
    RefCountedObject():
        m_counter(0)
    {
    }

    void duplicate();

    void release();

protected:
    virtual ~RefCountedObject()
    {
    }

private:
    RefCountedObject(const RefCountedObject&);
    RefCountedObject& operator = (const RefCountedObject&);

    std::atomic<int> m_counter;

};

inline void RefCountedObject::duplicate()
{
    m_counter.fetch_add(1);
}


inline void RefCountedObject::release()
{
    if (m_counter.fetch_sub(1) == 1) {
        delete this;
    }
}

#endif
/* EOF */
