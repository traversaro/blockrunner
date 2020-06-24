#include "Task.h"

#include <thread>

#include <spdlog/spdlog.h>

#include "Diagram.h"

class BlockRunner::Task::impl
{
public:
    std::vector<BlockRunner::Diagram> diagrams;
    BlockRunner::TaskState state{BlockRunner::TaskState::Stopped};
    std::thread taskThread;
    std::chrono::microseconds period{1000000};
    std::atomic<bool> isClosing{false};
};

BlockRunner::Task::Task()
    : pimpl(std::make_unique<impl>())
{
}

BlockRunner::Task::~Task() = default;

BlockRunner::Task::Task(Task&& task) = default;


void BlockRunner::Task::start()
{
    if (pimpl->state == BlockRunner::TaskState::Running) {
        return;
    }

    pimpl->isClosing = false;
    pimpl->taskThread = std::thread(&BlockRunner::Task::runThread, this);
    pimpl->state = BlockRunner::TaskState::Running;

    // Configure diagrams
    pimpl->diagrams.resize(1);
    for (auto& diagram : pimpl->diagrams) {
        diagram.reset();
    }
}

void BlockRunner::Task::stop()
{
    if (pimpl->state == BlockRunner::TaskState::Stopped) {
        return;
    }

    pimpl->isClosing = true;
    pimpl->taskThread.join();
    pimpl->state = BlockRunner::TaskState::Stopped;

    return;
}

std::chrono::microseconds BlockRunner::Task::getPeriod()
{
    return pimpl->period;
}

bool BlockRunner::Task::setPeriod(std::chrono::microseconds period)
{
    if (pimpl->state == BlockRunner::TaskState::Running) {
        return false;
    }

    pimpl->period = period;
    return true;
}

void BlockRunner::Task::runSinglePeriod()
{
    for (auto& diagram : pimpl->diagrams) {
        diagram.advance(pimpl->period);
    }
    spdlog::info("blockrunner-server: task has been run for one period");
}

void BlockRunner::Task::runThread()
{
    std::chrono::steady_clock::time_point startingTimeSteadyClock = std::chrono::steady_clock::now();

    // With a period of 1 microsecond this will overflow after ~600.000 years
    // so for now we can ignore the overflow
    uint64_t iteration = 0;
    while (!pimpl->isClosing) {
        runSinglePeriod();
        iteration++;
        std::this_thread::sleep_until(startingTimeSteadyClock+iteration*pimpl->period);
    }
}