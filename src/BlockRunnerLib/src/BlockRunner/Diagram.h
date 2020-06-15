#ifndef BLOCKRUNNER_DIAGRAM_H
#define BLOCKRUNNER_DIAGRAM_H

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

private:
    struct impl;
    std::unique_ptr<impl> pimpl;
};

} // namespace BlockRunner

#endif