#include "Diagram.h"

#include <vector>

#include <drake/systems/framework/diagram_builder.h>
#include <drake/systems/framework/diagram.h>
#include <drake/systems/analysis/simulator.h>

// Example Drake discrete system includes
#include <drake/systems/framework/leaf_system.h>
#include <spdlog/spdlog.h>

// Simple Drake system, from https://drake.mit.edu/doxygen_cxx/group__discrete__systems.html
class ExampleDiscreteSystem : public drake::systems::LeafSystem<double> {
public:
    DRAKE_NO_COPY_NO_MOVE_NO_ASSIGN(ExampleDiscreteSystem)
        ExampleDiscreteSystem() {
        DeclareDiscreteState(1);  // Just one state variable, x[0], default=0.
        // Update to x_{n+1}, using a Drake "discrete update" event (occurs
        // at the beginning of step n+1).
        DeclarePeriodicDiscreteUpdateEvent(kPeriod, kOffset,
            &ExampleDiscreteSystem::Update);
        // Present y_n (=S_n) at the output port.
        DeclareVectorOutputPort("Sn", drake::systems::BasicVector<double>(1),
            &ExampleDiscreteSystem::Output);
    }
    static constexpr double kPeriod = 1.;  // Update at 1 Hz (h=1).
    static constexpr double kOffset = 0.;  // Trigger events at n=0.
private:
    void Update(const drake::systems::Context<double>& context,
        drake::systems::DiscreteValues<double>* xd) const {
        const double x_n = context.get_discrete_state()[0];
        (*xd)[0] = x_n + 1.;
        spdlog::info("blockrunner-server: ExampleDiscreteSystem increasing the state from {} to {}", x_n, (*xd)[0]);
    }
    void Output(const drake::systems::Context<double>& context,
        drake::systems::BasicVector<double>* result) const {
        const double x_n = context.get_discrete_state()[0];
        const double S_n = 10 * x_n;
        (*result)[0] = S_n;
    }
};


class BlockRunner::Diagram::impl
{
public:
    ExampleDiscreteSystem dummy_system;
    std::unique_ptr<drake::systems::Diagram<double>> diagram{};
    std::unique_ptr<drake::systems::Simulator<double>> simulator{};
};

BlockRunner::Diagram::Diagram()
    : pimpl(std::make_unique<impl>())
{
    // TODO(traversaro): support arbitrary diagrams, not the hardcoded ExampleDiscreteSystem
    pimpl->simulator = std::make_unique<drake::systems::Simulator<double>>(pimpl->dummy_system);

    this->reset();
}

BlockRunner::Diagram::~Diagram() = default;
BlockRunner::Diagram::Diagram(Diagram && diagram) = default;

void BlockRunner::Diagram::reset()
{
    // Set the initial conditions to zero for discrete and continuous values
    for (int groupDiscrete = 0; groupDiscrete < pimpl->simulator->get_mutable_context().get_mutable_discrete_state().num_groups(); groupDiscrete++) {
        pimpl->simulator->get_mutable_context().get_mutable_discrete_state().get_mutable_vector(groupDiscrete).SetZero();
    }
    pimpl->simulator->get_mutable_context().get_mutable_continuous_state().get_mutable_vector().SetZero();
}

bool BlockRunner::Diagram::advance(const std::chrono::microseconds advancedTime)
{
    double currentTimeInSeconds = pimpl->simulator->get_context().get_time();
    double microsecondsToSeconds = 1. / 1000000.;
    double finalTimeInSeconds = currentTimeInSeconds + advancedTime.count() * microsecondsToSeconds;
    pimpl->simulator->AdvanceTo(finalTimeInSeconds);
    return true;
}

