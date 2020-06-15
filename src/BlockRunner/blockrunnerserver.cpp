#include <atomic>
#include <chrono>
#include <thread>
#include <vector>

#include <cstdlib>
#include <csignal>

#include <spdlog/spdlog.h>

#include <BlockRunner/Task.h>

std::atomic<bool> isClosing{false};

void my_handler(int signal)
{
    isClosing = true;
}

#ifdef WIN32

#include <windows.h>

BOOL WINAPI CtrlHandler(DWORD fdwCtrlType)
{
    switch (fdwCtrlType) {
        // Handle the CTRL-C signal.
    case CTRL_C_EVENT:
    case CTRL_CLOSE_EVENT:
    case CTRL_SHUTDOWN_EVENT:
        my_handler(0);
        return TRUE;

    // Handle all other events
    default:
        return FALSE;
    }
}
#endif

void handleSigInt()
{
#ifdef WIN32
    SetConsoleCtrlHandler(CtrlHandler, TRUE);
#else
    struct sigaction action;
    memset(&action, 0, sizeof(action));
    action.sa_handler = &my_handler;
    sigaction(SIGINT, &action, NULL);
    sigaction(SIGTERM, &action, NULL);
    sigaction(SIGABRT, &action, NULL);
#endif
}

int main()
{
    spdlog::info("blockrunner-server version 0.0.1 launched.");

    // Listen to signals for closing in a clean way the server
    handleSigInt();

    // Load task to run and for each tasks load the diagrams to run
    // TODO(traversaro): do not hardcode the tasks
    std::vector<BlockRunner::Task> tasks;

    // Add dummy task
    tasks.emplace_back(BlockRunner::Task());

    spdlog::info("blockrunner-server: starting tasks.");
    for (BlockRunner::Task& task : tasks) {
        task.start();
    }

    // Once all the tasks have been spawned, the server main thread stays in idle
    while (!isClosing) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    spdlog::info("blockrunner-server: stopping tasks.");
    for (BlockRunner::Task& task : tasks) {
        task.stop();
    }

    return EXIT_SUCCESS;
}
