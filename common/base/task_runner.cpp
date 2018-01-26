#include "task_runner.h"
#include "bind.h"

task_runner::task_runner():
    m_stop(false)
{
}

void task_runner::start()
{
    m_stop = false;
}

bool task_runner::postTask(const Closure& task)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    if (!m_stop) {
        m_tasks.push(task);
        return true;
    }
    return false;
}

bool task_runner::postStopTask()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    if (!m_stop) {
        m_tasks.push(Bind(&task_runner::stopRunner, this));
        return true;
    }
    return false;
}

void task_runner::runTask()
{
    while (!checkEmpty()) {
        m_mutex.lock();
        Closure task = m_tasks.front();
        m_tasks.pop();
        m_mutex.unlock();
        task.Run();
        if (m_stop) {
            m_mutex.lock();
            while (!m_tasks.empty()) {
                m_tasks.pop();
            }
            m_mutex.unlock();
        }
    }
}

bool task_runner::isRunning()
{
    return !m_stop;
}

bool task_runner::checkEmpty()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    bool ret = m_tasks.empty();
    return ret;
}

void task_runner::stopRunner()
{
    m_stop = true;
}

/* EOF */
