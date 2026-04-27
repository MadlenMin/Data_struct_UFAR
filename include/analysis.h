#ifndef ANALYSIS_H
#define ANALYSIS_H

#include "graph.h"
#include "simulator.h"
#include <vector>
#include <map>

// ============================================================================
// Milena Yeganyan: ANALYSIS & INTERVENTION
// ============================================================================

struct VaccinationResult {
    std::vector<int> vaccinated_nodes;  // Which nodes were vaccinated
    int total_infections_prevented;     // Reduction in final I count
    double infection_reduction_percent; // Percentage reduction
};

class Analyzer {
private:
    const Simulator& simulator;         // Reference to simulator for running experiments
    const Graph& original_graph;        // Original graph for analysis

public:
    // Constructor
    Analyzer(const Simulator& sim, const Graph& graph);

    // Find top-N super-spreaders by simulating removal of each node
    // Returns vector of (node_id, infection_impact) pairs, sorted by impact
    // Algorithm: for each node, remove it, run simulation, count infections
    std::vector<std::pair<int, int>> findTopSpreaders(int num_to_find = 5);

    // Vaccination strategy 1: randomly vaccinate K nodes
    VaccinationResult vaccinateRandomStrategy(int budget, int random_seed = 42);

    // Vaccination strategy 2: vaccinate K nodes with highest contact degree
    VaccinationResult vaccinateByDegree(int budget);

    // Compare both vaccination strategies
    void compareVaccinationStrategies(int budget);

    // Analyze total epidemic impact
    struct EpidemicAnalysis {
        int total_infected_ever;        // Total people ever infected
        int peak_infected_count;        // Max concurrent infections
        int peak_infected_at_step;      // Step when peak occurred
        double attack_rate;             // (total_infected / population) * 100
    };

    EpidemicAnalysis analyzeEpidemicReach() const;

    // Utility: compute degree (number of neighbors) for each node
    std::vector<std::pair<int, int>> getNodeDegrees() const;
};

#endif 