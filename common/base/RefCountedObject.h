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

    int m_counter;

};

inline void RefCountedObject::duplicate()
{
    __sync_add_and_fetch(&m_counter, 1);
}

inline void RefCountedObject::release()
{
    if (__sync_sub_and_fetch(&m_counter, 1) == 0) {
        delete this;
    }
}

#endif
/* EOF */
