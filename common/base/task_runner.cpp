#include "task_runner.h"
#include "bind.h"

task_runner::task_runner():
    m_stop(false)
{
    pthread_mutex_init(&m_mutex, NULL);
}

task_runner::~task_runner()
{
    pthread_mutex_destroy(&m_mutex);
}

void task_runner::start()
{
    m_stop = false;
}

bool task_runner::postTask(const Closure& task)
{
    pthread_mutex_lock(&m_mutex);
    if (!m_stop) {
        m_tasks.push(task);
        pthread_mutex_unlock(&m_mutex);
        return true;
    }
    pthread_mutex_unlock(&m_mutex);
    return false;
}

bool task_runner::postStopTask()
{
    pthread_mutex_lock(&m_mutex);
    if (!m_stop) {
        m_tasks.push(Bind(&task_runner::stopRunner, this));
        pthread_mutex_unlock(&m_mutex);
        return true;
    }
    pthread_mutex_unlock(&m_mutex);
    return false;
}

void task_runner::runTask()
{
    while (!checkEmpty()) {
        pthread_mutex_lock(&m_mutex);
        Closure task = m_tasks.front();
        m_tasks.pop();
        pthread_mutex_unlock(&m_mutex);
        task.Run();
        if (m_stop) {
            pthread_mutex_lock(&m_mutex);
            while (!m_tasks.empty()) {
                m_tasks.pop();
            }
            pthread_mutex_unlock(&m_mutex);
        }
    }
}

bool task_runner::isRunning()
{
    return !m_stop;
}

bool task_runner::checkEmpty()
{
    pthread_mutex_lock(&m_mutex);
    bool ret = m_tasks.empty();
    pthread_mutex_unlock(&m_mutex);
    return ret;
}

void task_runner::stopRunner()
{
    m_stop = true;
}

/* EOF */
