#include "simulator.h"
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <iostream>
#include <algorithm>

// ============================================================================
// Lia Sargsyan: SIMULATOR IMPLEMENTATION
// ============================================================================

Simulator::Simulator(const Graph& graph, const SimConfig& config)
    : graph(graph), config(config), current_step(0) {
    srand(config.random_seed);
}

StepStats Simulator::spreadStep() {
    StepStats stats;
    stats.step = current_step;
    
    // Count current S, I, R
    int S = 0, I = 0, R = 0;
    for (const auto& person : graph.getNodes()) {
        if (person.state == SIRState::SUSCEPTIBLE) S++;
        else if (person.state == SIRState::INFECTED) I++;
        else if (person.state == SIRState::RECOVERED) R++;
    }
    
    stats.susceptible_count = S;
    stats.infected_count = I;
    stats.recovered_count = R;
    stats.new_infections = 0;
   
    std::vector<int> newly_infected;
    
    for (const auto& person : graph.getNodes()) {
        if (person.state != SIRState::INFECTED || person.quarantined) {
            continue;  // Skip non-infected or quarantined nodes
        }
        
        // This infected person tries to infect neighbors
        const auto& neighbors = graph.getNeighbors(person.id);
        for (const Edge& edge : neighbors) {
            int neighbor_id = edge.to;
            
            // Try to infect this neighbor
            if (tryInfect(person.id, neighbor_id)) {
                newly_infected.push_back(neighbor_id);
            }
        }
    }
    
    // Mark newly infected people
    for (int node_id : newly_infected) {
        graph.getPerson(node_id).state = SIRState::INFECTED;
        graph.getPerson(node_id).days_infected = 1;
        stats.new_infections++;
    }

    updateRecovery();

    stats.R0_estimate = (I > 0) ? (double)stats.new_infections / I : 0.0;
    
    current_step++;
    history.push_back(stats);
    
    return stats;
}

bool Simulator::tryInfect(int from_id, int to_id) {
    Person& target = graph.getPerson(to_id);
    
    // Can only infect susceptible people
    if (target.state != SIRState::SUSCEPTIBLE) {
        return false;
    }
    
    // Quarantined or vaccinated people cannot be infected
    if (target.quarantined || target.vaccinated) {
        return false;
    }
    
    // Get contact weight from edge
    const auto& neighbors = graph.getNeighbors(from_id);
    double contact_weight = 0.5;  // Default if not found
    
    for (const Edge& edge : neighbors) {
        if (edge.to == to_id) {
            contact_weight = edge.weight;
            break;
        }
    }
    
    // Infection probability = base_prob * contact_weight
    double infection_prob = config.base_infection_probability * contact_weight;
    
    // Random outcome
    double random_value = (double)rand() / RAND_MAX;
    
    return (random_value < infection_prob);
}

void Simulator::updateRecovery() {
    for (auto& person : graph.getMutableNodes()) {
        if (person.state != SIRState::INFECTED) {
            continue;
        }
        
        // Increment infection age
        person.days_infected++;
        
        // If infected for recovery_days, transition to recovered
        if (person.days_infected >= config.recovery_days) {
            person.state = SIRState::RECOVERED;
            person.days_infected = 0;
        }
    }
}

std::vector<StepStats> Simulator::runSteps(int num_steps) {
    std::vector<StepStats> results;
    
    for (int i = 0; i < num_steps; i++) {
        StepStats stats = spreadStep();
        results.push_back(stats);
        
        // Print progress
        std::cout << "Step " << stats.step << ": S=" << stats.susceptible_count
                  << " I=" << stats.infected_count << " R=" << stats.recovered_count
                  << " (new: " << stats.new_infections << ", R0: " 
                  << std::fixed << stats.R0_estimate << ")" << std::endl;
        
        // Stop if epidemic is over (no infected individuals)
        if (stats.infected_count == 0) {
            std::cout << "Epidemic ended at step " << stats.step << std::endl;
            break;
        }
    }
    
    return results;
}

void Simulator::infect(int node_id) {
    Person& person = graph.getPerson(node_id);
    if (person.state == SIRState::SUSCEPTIBLE) {
        person.state = SIRState::INFECTED;
        person.days_infected = 1;
        std::cout << "Infected node " << node_id << std::endl;
    } else {
        std::cout << "Cannot infect node " << node_id << " (not susceptible)" << std::endl;
    }
}

void Simulator::quarantine(int node_id) {
    Person& person = graph.getPerson(node_id);
    person.quarantined = true;
    std::cout << "Quarantined node " << node_id << std::endl;
}

void Simulator::releaseQuarantine(int node_id) {
    Person& person = graph.getPerson(node_id);
    person.quarantined = false;
    std::cout << "Released node " << node_id << " from quarantine" << std::endl;
}

void Simulator::reset() {
    graph.resetAllNodes();
    history.clear();
    current_step = 0;
    std::cout << "Simulation reset." << std::endl;
}

const StepStats& Simulator::getStepStats(int step_index) const {
    static StepStats empty;
    if (step_index < 0 || step_index >= (int)history.size()) {
        return empty;
    }
    return history[step_index];
}

StepStats Simulator::getCurrentStats() const {
    StepStats stats;
    stats.step = current_step;
    
    for (const auto& person : graph.getNodes()) {
        if (person.state == SIRState::SUSCEPTIBLE) stats.susceptible_count++;
        else if (person.state == SIRState::INFECTED) stats.infected_count++;
        else if (person.state == SIRState::RECOVERED) stats.recovered_count++;
    }
    
    return stats;
}
