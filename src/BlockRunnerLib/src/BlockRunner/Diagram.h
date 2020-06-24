#ifndef BLOCKRUNNER_DIAGRAM_H
#define BLOCKRUNNER_DIAGRAM_H

#include <chrono>
#include <memory>

namespace BlockRunner {
/**
 * A diagram is an interconnection of systems that is executed by a task.
 *
 * A task can execute multiple diagrams.
 */
class Diagram
{
public:
    Diagram();
    ~Diagram();
    Diagram(Diagram&& diagram);

    /**
     * Reset diagram to its initial state.
     */
    void reset();

    /**
     * Advance the diagram for the specified time.
     */
    bool advance(const std::chrono::microseconds advancedTime);

private:
    struct impl;
    std::unique_ptr<impl> pimpl;
};

} // namespace BlockRunner

#endif