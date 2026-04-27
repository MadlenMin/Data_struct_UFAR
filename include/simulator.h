#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "graph.h"
#include <vector>
#include <iomanip>

// ============================================================================
// Lia Sargsyan: SIMULATION ENGINE
// ============================================================================


// Configuration for the epidemic simulation
struct SimConfig {
    double base_infection_probability;  // Base prob of transmission per contact (e.g., 0.3)
    int recovery_days;                  // Days until infected → recovered (e.g., 14 days)
    int random_seed;                    // Seed for random number generation

    SimConfig() : base_infection_probability(0.3), recovery_days(14), random_seed(42) {}
};

// Statistics collected at each simulation step
struct StepStats {
    int step;                           // Time step number
    int susceptible_count;              // Number of S individuals
    int infected_count;                 // Number of I individuals
    int recovered_count;                // Number of R individuals
    int new_infections;                 // Newly infected in this step
    double R0_estimate;                 // Estimated reproduction number

    StepStats() : step(0), susceptible_count(0), infected_count(0), 
                  recovered_count(0), new_infections(0), R0_estimate(0.0) {}
};

// Simulator class: manages epidemic dynamics on the contact graph
class Simulator {
private:
    Graph graph;                        // The contact network
    SimConfig config;                   // Simulation parameters
    std::vector<StepStats> history;     // Historical statistics for each step
    int current_step;                   // Current simulation time step

public:
    // Constructor: initialize simulator with graph and config
    Simulator(const Graph& graph, const SimConfig& config);

    // Execute one time step of the epidemic:
    //   1. Infected nodes try to infect their neighbors (BFS-like traversal)
    //   2. Update recovery times for aged infections
    StepStats spreadStep();

    // Attempt to infect one neighbor based on contact weight
    // Returns true if infection was successful
    bool tryInfect(int from_id, int to_id);

    // Age all infections and transition recovered individuals
    void updateRecovery();

    // Run the simulation for n steps
    std::vector<StepStats> runSteps(int num_steps);

    // Manually infect a person (for testing or initialization)
    void infect(int node_id);

    // Quarantine a person (prevents spreading and being infected)
    void quarantine(int node_id);

    // Release quarantine
    void releaseQuarantine(int node_id);

    // Reset simulation to initial state
    void reset();

    // Get statistics from a specific step
    const StepStats& getStepStats(int step_index) const;

    // Get current step statistics
    StepStats getCurrentStats() const;

    // Get simulation history
    const std::vector<StepStats>& getHistory() const { return history; }

    // Getters
    int getCurrentStep() const { return current_step; }
    const Graph& getGraph() const { return graph; }
    Graph& getMutableGraph() { return graph; }
};

#endif