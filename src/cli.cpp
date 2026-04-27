#include "cli.h"
#include <iostream>
#include <sstream>
#include <iomanip>

// ============================================================================
// Milena Yeganyan: CLI IMPLEMENTATION
// ============================================================================

CLI::CLI(const Graph& graph, const SimConfig& config)
    : simulator(graph, config), analyzer(simulator, graph), running(true) {
    // Infect a random starting node to kickstart the epidemic
    simulator.infect(0);
}

void CLI::run() {
    std::cout << "\n╔════════════════════════════════════════════════════════════╗" << std::endl;
    std::cout << "║         EPIDEMIC SIMULATION - SIR Model on Contact Graph   ║" << std::endl;
    std::cout << "╚════════════════════════════════════════════════════════════╝\n" << std::endl;
    
    std::cout << "Type 'help' for available commands.\n" << std::endl;
    
    while (running) {
        std::cout << "> ";
        std::string user_input;
        std::getline(std::cin, user_input);
        
        if (user_input.empty()) continue;
        
        if (!executeCommand(user_input)) {
            running = false;
        }
    }
    
    std::cout << "\nSimulation ended. Goodbye!" << std::endl;
}

std::vector<std::string> CLI::tokenize(const std::string& input) {
    std::vector<std::string> tokens;
    std::istringstream iss(input);
    std::string token;
    while (iss >> token) {
        tokens.push_back(token);
    }
    return tokens;
}

bool CLI::executeCommand(const std::string& command_line) {
    std::vector<std::string> tokens = tokenize(command_line);
    if (tokens.empty()) return true;
    
    std::string cmd = tokens[0];
    
    if (cmd == "quit" || cmd == "exit") {
        return false;
    } else if (cmd == "help") {
        cmdHelp(tokens);
    } else if (cmd == "run-simulation") {
        cmdRunSimulation(tokens);
    } else if (cmd == "infect") {
        cmdInfect(tokens);
    } else if (cmd == "quarantine") {
        cmdQuarantine(tokens);
    } else if (cmd == "release") {
        cmdRelease(tokens);
    } else if (cmd == "stats") {
        cmdStats(tokens);
    } else if (cmd == "super-spreaders") {
        cmdSuperSpreaders(tokens);
    } else if (cmd == "vaccinate") {
        cmdVaccinate(tokens);
    } else if (cmd == "reset") {
        cmdReset(tokens);
    } else {
        std::cout << "Unknown command: " << cmd << std::endl;
        std::cout << "Type 'help' for list of commands." << std::endl;
    }
    
    return true;
}

void CLI::cmdRunSimulation(const std::vector<std::string>& args) {
    if (args.size() < 2) {
        std::cout << "Usage: run-simulation <num_steps>" << std::endl;
        return;
    }
    
    int num_steps = std::stoi(args[1]);
    std::cout << "\nRunning simulation for " << num_steps << " steps...\n" << std::endl;
    
    simulator.runSteps(num_steps);
    
    displayStats();
}

void CLI::cmdInfect(const std::vector<std::string>& args) {
    if (args.size() < 2) {
        std::cout << "Usage: infect <node_id>" << std::endl;
        return;
    }
    
    int node_id = std::stoi(args[1]);
    simulator.infect(node_id);
}

void CLI::cmdQuarantine(const std::vector<std::string>& args) {
    if (args.size() < 2) {
        std::cout << "Usage: quarantine <node_id>" << std::endl;
        return;
    }
    
    int node_id = std::stoi(args[1]);
    simulator.quarantine(node_id);
}

void CLI::cmdRelease(const std::vector<std::string>& args) {
    if (args.size() < 2) {
        std::cout << "Usage: release <node_id>" << std::endl;
        return;
    }
    
    int node_id = std::stoi(args[1]);
    simulator.releaseQuarantine(node_id);
}

void CLI::cmdStats(const std::vector<std::string>& args) {
    (void)args;  // Unused
    displayStats();
}

void CLI::cmdSuperSpreaders(const std::vector<std::string>& args) {
    (void)args;
    
    std::cout << "\nIdentifying top-5 super-spreaders..." << std::endl;
    std::cout << "(This may take a while...)\n" << std::endl;
    
    auto top_spreaders = analyzer.findTopSpreaders(5);
    
    std::cout << "\nTop-5 Super-Spreaders:" << std::endl;
    std::cout << "─────────────────────────────────────" << std::endl;
    for (size_t i = 0; i < top_spreaders.size(); i++) {
        std::cout << (i+1) << ". Node " << top_spreaders[i].first
                  << " (impact: " << top_spreaders[i].second << " infections prevented)"
                  << std::endl;
    }
}

void CLI::cmdVaccinate(const std::vector<std::string>& args) {
    if (args.size() < 2) {
        std::cout << "Usage: vaccinate <budget>" << std::endl;
        return;
    }
    
    int budget = std::stoi(args[1]);
    analyzer.compareVaccinationStrategies(budget);
}

void CLI::cmdReset(const std::vector<std::string>& args) {
    (void)args;
    simulator.reset();
}

void CLI::cmdHelp(const std::vector<std::string>& args) {
    (void)args;
    
    std::cout << "\n╔═══════════════════════════════════════════════════════════════╗" << std::endl;
    std::cout << "║                    AVAILABLE COMMANDS                         ║" << std::endl;
    std::cout << "╚═══════════════════════════════════════════════════════════════╝\n" << std::endl;
    
    std::cout << "SIMULATION CONTROL:" << std::endl;
    std::cout << "  run-simulation <steps>    Run simulation for N time steps" << std::endl;
    std::cout << "  infect <node_id>          Manually infect a person" << std::endl;
    std::cout << "  quarantine <node_id>      Isolate a person" << std::endl;
    std::cout << "  release <node_id>         Remove quarantine from person" << std::endl;
    std::cout << "  reset                     Reset simulation to initial state\n" << std::endl;
    
    std::cout << "ANALYSIS:" << std::endl;
    std::cout << "  stats                     Display current S, I, R counts" << std::endl;
    std::cout << "  super-spreaders           Identify top-5 influential nodes" << std::endl;
    std::cout << "  vaccinate <budget>        Compare vaccination strategies\n" << std::endl;
    
    std::cout << "OTHER:" << std::endl;
    std::cout << "  help                      Show this help message" << std::endl;
    std::cout << "  quit / exit               Exit the program\n" << std::endl;
}

void CLI::displayStats() {
    std::cout << "\n┌─────────────────────────────────────────┐" << std::endl;
    std::cout << "│        EPIDEMIC STATUS (Current)        │" << std::endl;
    std::cout << "└─────────────────────────────────────────┘\n" << std::endl;
    
    StepStats current = simulator.getCurrentStats();
    
    std::cout << "Time Step:       " << current.step << std::endl;
    std::cout << "Susceptible (S): " << current.susceptible_count << std::endl;
    std::cout << "Infected (I):    " << current.infected_count << std::endl;
    std::cout << "Recovered (R):   " << current.recovered_count << std::endl;
    std::cout << "Total Population: "
              << (current.susceptible_count + current.infected_count + current.recovered_count)
              << std::endl;
    
    displayHistory();
}

void CLI::displayHistory() {
    std::cout << "\n┌─────────────────────────────────────────────────────────────────┐" << std::endl;
    std::cout << "│                    RECENT HISTORY (Last 5)                     │" << std::endl;
    std::cout << "└─────────────────────────────────────────────────────────────────┘\n" << std::endl;
    
    const auto& history = simulator.getHistory();
    int start_idx = std::max(0, (int)history.size() - 5);
    
    std::cout << std::setw(5) << "Step" << " | "
              << std::setw(3) << "S" << " | "
              << std::setw(3) << "I" << " | "
              << std::setw(3) << "R" << " | "
              << std::setw(4) << "New" << " | "
              << std::setw(5) << "R0" << std::endl;
    
    std::cout << "─────────────────────────────────────────────" << std::endl;
    
    for (size_t i = start_idx; i < history.size(); i++) {
        const StepStats& stat = history[i];
        std::cout << std::setw(5) << stat.step << " | "
                  << std::setw(3) << stat.susceptible_count << " | "
                  << std::setw(3) << stat.infected_count << " | "
                  << std::setw(3) << stat.recovered_count << " | "
                  << std::setw(4) << stat.new_infections << " | "
                  << std::setw(5) << std::fixed << std::setprecision(2) << stat.R0_estimate
                  << std::endl;
    }
    
    std::cout << std::endl;
}
