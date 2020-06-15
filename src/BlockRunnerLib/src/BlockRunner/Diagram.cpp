#include "Diagram.h"

#include <vector>

#include <drake/systems/framework/diagram_builder.h>
#include <drake/systems/framework/diagram.h>
#include <drake/systems/analysis/simulator.h>


class BlockRunner::Diagram::impl
{
public:
    std::unique_ptr<drake::systems::Diagram<double>> diagram{};
    std::unique_ptr<drake::systems::Simulator<double>> simulator{};
};

BlockRunner::Diagram::Diagram()
    : pimpl(std::make_unique<impl>())
{
}

BlockRunner::Diagram::~Diagram() = default;
BlockRunner::Diagram::Diagram(Diagram&& diagram) = default;