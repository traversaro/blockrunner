#ifndef BLOCKRUNNER_TASK_H
#define BLOCKRUNNER_TASK_H

#include <chrono>
#include <memory>
#include <vector>

namespace BlockRunner {

/**
 * State of task.
 */
enum class TaskState
{
    Running,
    Stopped
};

/**
 * A task is a high level representation of a thread.
 *
 * On platform that support threads, each Task is executed in different
 * thread. The term "Task" is inspired from the equivalent concept in
 * the IEC 61131-3 standard, see https://plcopen.org/iec-61131-1 .
 */
class Task
{
public:
    Task();
    ~Task();
    Task(Task&& task);

    /**
     * Start the task.
     */
    void start();

    /**
     * Stop the task.
     */
    void stop();

    /**
     * Get the period.
     */
    std::chrono::microseconds getPeriod();

    /**
     * Set the period.
     * Only possible if the state of the task is stopped.
     */
    bool setPeriod(const std::chrono::microseconds period);

private:
    /**
     * Run a single period of the task.
     */
    void runSinglePeriod();

    /**
     * Function called by the thread.
     */
    void runThread();

    struct impl;
    std::unique_ptr<impl> pimpl;
};
} // namespace BlockRunner


#endif