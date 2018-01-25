/**
 * @file task_runner.h
 * @brief
 *
 */

#ifndef TASK_RUNNER_H
#define TASK_RUNNER_H

#ifndef __cplusplus
#   error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include <queue>

#include <pthread.h>
#include "callback.h"

/**
 * @brief
 *
 * @class task_runner
 */

class task_runner
{

public:

    task_runner();

    ~task_runner();

    void start();

    bool postTask(const Closure& task);

    bool postStopTask();

    void runTask();

    bool isRunning();

private:

    bool checkEmpty();

    void stopRunner();

    bool m_stop;

    std::queue<Closure> m_tasks;

    pthread_mutex_t m_mutex;

};

#endif // TASK_RUNNER_H
/* EOF */
