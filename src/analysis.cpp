#include "analysis.h"
#include <iostream>
#include <algorithm>
#include <cstdlib>

// ============================================================================
// Milena Yeganyan: ANALYSIS IMPLEMENTATION
// ============================================================================

Analyzer::Analyzer(const Simulator& sim, const Graph& graph)
    : simulator(sim), original_graph(graph) {}

std::vector<std::pair<int, int>> Analyzer::findTopSpreaders(int num_to_find) {
    std::vector<std::pair<int, int>> spreader_impact;  // (node_id, infection_reduction)
    
    int original_total_infected = 0;
    
    // First, simulate baseline (no removal)
    Simulator baseline_sim = simulator;
    std::vector<StepStats> baseline_history = baseline_sim.runSteps(100);
    for (const auto& stat : baseline_history) {
        original_total_infected += stat.new_infections;
    }
    
    std::cout << "Baseline total infections: " << original_total_infected << std::endl;
    
    // Now, for each node, simulate removing it
    int num_nodes = original_graph.getNumNodes();
    for (int node_to_remove = 0; node_to_remove < num_nodes; node_to_remove++) {
        // Create modified graph (copy of original)
        Graph modified_graph = original_graph;
        modified_graph.removeNode(node_to_remove);
        
        // Run simulation on modified graph
        SimConfig config;
        config.base_infection_probability = 0.3;
        config.recovery_days = 14;
        Simulator test_sim(modified_graph, config);
        
        // Manually infect an initial node (not the removed one)
        int initial_infected = 0;
        for (int i = 0; i < num_nodes; i++) {
            if (i != node_to_remove) {
                initial_infected = i;
                break;
            }
        }
        test_sim.infect(initial_infected);
        
        // Run simulation
        std::vector<StepStats> test_history = test_sim.runSteps(100);
        int test_total_infected = 0;
        for (const auto& stat : test_history) {
            test_total_infected += stat.new_infections;
        }
        
        // Compute impact (reduction in infections)
        int impact = original_total_infected - test_total_infected;
        spreader_impact.push_back({node_to_remove, impact});
        
        std::cout << "Node " << node_to_remove << " impact: " << impact << " prevented infections" << std::endl;
    }
    
    // Sort by impact (descending)
    std::sort(spreader_impact.begin(), spreader_impact.end(),
              [](const auto& a, const auto& b) { return a.second > b.second; });
    
    // Return top N
    if (spreader_impact.size() > (size_t)num_to_find) {
        spreader_impact.resize(num_to_find);
    }
    
    return spreader_impact;
}

VaccinationResult Analyzer::vaccinateRandomStrategy(int budget, int random_seed) {
    VaccinationResult result;
    srand(random_seed);
    
    int num_nodes = original_graph.getNumNodes();
    
    // Randomly select K nodes to vaccinate
    std::vector<int> all_nodes;
    for (int i = 0; i < num_nodes; i++) {
        all_nodes.push_back(i);
    }
    
    // Shuffle and select first budget nodes
    for (int i = 0; i < budget && i < (int)all_nodes.size(); i++) {
        int swap_idx = i + (rand() % (all_nodes.size() - i));
        std::swap(all_nodes[i], all_nodes[swap_idx]);
        result.vaccinated_nodes.push_back(all_nodes[i]);
    }
    
    // Create vaccinated graph
    Graph vaccinated_graph = original_graph;
    for (int node_id : result.vaccinated_nodes) {
        vaccinated_graph.getPerson(node_id).vaccinated = true;
    }
    
    // Run simulation
    SimConfig config;
    Simulator vaccinated_sim(vaccinated_graph, config);
    vaccinated_sim.infect(0);
    std::vector<StepStats> vaccinated_history = vaccinated_sim.runSteps(100);
    
    int vaccinated_total_infected = 0;
    for (const auto& stat : vaccinated_history) {
        vaccinated_total_infected += stat.new_infections;
    }
    
    // Compute baseline for comparison
    Simulator baseline_sim = simulator;
    std::vector<StepStats> baseline_history = baseline_sim.runSteps(100);
    int baseline_total_infected = 0;
    for (const auto& stat : baseline_history) {
        baseline_total_infected += stat.new_infections;
    }
    
    result.total_infections_prevented = baseline_total_infected - vaccinated_total_infected;
    if (baseline_total_infected > 0) {
        result.infection_reduction_percent = 
            (double)result.total_infections_prevented / baseline_total_infected * 100;
    }
    
    return result;
}

VaccinationResult Analyzer::vaccinateByDegree(int budget) {
    VaccinationResult result;
    
    // Get node degrees (number of contacts)
    std::vector<std::pair<int, int>> degrees = getNodeDegrees();
    
    // Sort by degree (descending)
    std::sort(degrees.begin(), degrees.end(),
              [](const auto& a, const auto& b) { return a.second > b.second; });
    
    // Vaccinate top-degree nodes
    int num_nodes = original_graph.getNumNodes();
    for (int i = 0; i < budget && i < num_nodes; i++) {
        result.vaccinated_nodes.push_back(degrees[i].first);
    }
    
    // Create vaccinated graph
    Graph vaccinated_graph = original_graph;
    for (int node_id : result.vaccinated_nodes) {
        vaccinated_graph.getPerson(node_id).vaccinated = true;
    }
    
    // Run simulation
    SimConfig config;
    Simulator vaccinated_sim(vaccinated_graph, config);
    vaccinated_sim.infect(0);
    std::vector<StepStats> vaccinated_history = vaccinated_sim.runSteps(100);
    
    int vaccinated_total_infected = 0;
    for (const auto& stat : vaccinated_history) {
        vaccinated_total_infected += stat.new_infections;
    }
    
    // Compute baseline
    Simulator baseline_sim = simulator;
    std::vector<StepStats> baseline_history = baseline_sim.runSteps(100);
    int baseline_total_infected = 0;
    for (const auto& stat : baseline_history) {
        baseline_total_infected += stat.new_infections;
    }
    
    result.total_infections_prevented = baseline_total_infected - vaccinated_total_infected;
    if (baseline_total_infected > 0) {
        result.infection_reduction_percent = 
            (double)result.total_infections_prevented / baseline_total_infected * 100;
    }
    
    return result;
}

void Analyzer::compareVaccinationStrategies(int budget) {
    std::cout << "\n=== VACCINATION STRATEGY COMPARISON ===" << std::endl;
    std::cout << "Budget: " << budget << " vaccinations\n" << std::endl;
    
    VaccinationResult random_result = vaccinateRandomStrategy(budget);
    VaccinationResult degree_result = vaccinateByDegree(budget);
    
    std::cout << "Random Strategy:" << std::endl;
    std::cout << "  Infections prevented: " << random_result.total_infections_prevented << std::endl;
    std::cout << "  Reduction: " << random_result.infection_reduction_percent << "%" << std::endl;
    
    std::cout << "\nDegree-targeting Strategy:" << std::endl;
    std::cout << "  Infections prevented: " << degree_result.total_infections_prevented << std::endl;
    std::cout << "  Reduction: " << degree_result.infection_reduction_percent << "%" << std::endl;
    
    if (degree_result.total_infections_prevented > random_result.total_infections_prevented) {
        std::cout << "\n✓ Degree-targeting is more effective!" << std::endl;
    } else if (random_result.total_infections_prevented > degree_result.total_infections_prevented) {
        std::cout << "\n✓ Random strategy is more effective!" << std::endl;
    } else {
        std::cout << "\n✓ Both strategies are equally effective." << std::endl;
    }
}

Analyzer::EpidemicAnalysis Analyzer::analyzeEpidemicReach() const {
    EpidemicAnalysis analysis;
    analysis.total_infected_ever = 0;
    analysis.peak_infected_count = 0;
    analysis.peak_infected_at_step = 0;
    
    int population = original_graph.getNumNodes();
    
    const auto& history = simulator.getHistory();
    for (const auto& stat : history) {
        analysis.total_infected_ever += stat.new_infections;
        if (stat.infected_count > analysis.peak_infected_count) {
            analysis.peak_infected_count = stat.infected_count;
            analysis.peak_infected_at_step = stat.step;
        }
    }
    
    if (population > 0) {
        analysis.attack_rate = (double)analysis.total_infected_ever / population * 100;
    }
    
    return analysis;
}

std::vector<std::pair<int, int>> Analyzer::getNodeDegrees() const {
    std::vector<std::pair<int, int>> degrees;
    int num_nodes = original_graph.getNumNodes();
    
    for (int i = 0; i < num_nodes; i++) {
        int degree = original_graph.getNeighbors(i).size();
        degrees.push_back({i, degree});
    }
    
    return degrees;
}
