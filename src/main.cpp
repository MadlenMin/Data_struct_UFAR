#include "cli.h"
#include "graph.h"
#include "simulator.h"
#include <iostream>

// ============================================================================
// Lia Sargsyan: SIMULATOR IMPLEMENTATION
// ============================================================================

int main(int argc, char* argv[]) {
    try {
        // Get parameters from command line (or use defaults)
        int num_people = 120;           // Default: 120 people
        int num_edges = 450;            // Default: 450 contacts
        int initial_infected = 1;       // Default: 1 initially infected
        int random_seed = 42;           // Default: reproducible
        
        // Parse arguments
        for (int i = 1; i < argc; i++) {
            std::string arg = argv[i];
            if (arg == "--people" && i + 1 < argc) {
                num_people = std::stoi(argv[++i]);
            } else if (arg == "--edges" && i + 1 < argc) {
                num_edges = std::stoi(argv[++i]);
            } else if (arg == "--seed" && i + 1 < argc) {
                random_seed = std::stoi(argv[++i]);
            } else if (arg == "--help" || arg == "-h") {
                std::cout << "Epidemic Simulator - SIR Model on Contact Networks\n" << std::endl;
                std::cout << "Usage: epidemic_sim [options]\n" << std::endl;
                std::cout << "Options:" << std::endl;
                std::cout << "  --people <n>    Number of people in network (default: 120)" << std::endl;
                std::cout << "  --edges <n>     Number of contact edges (default: 450)" << std::endl;
                std::cout << "  --seed <n>      Random seed (default: 42)" << std::endl;
                std::cout << "  --help          Show this help message" << std::endl;
                return 0;
            }
        }
        std::cout << "╔═════════════════════════════════════════╗" << std::endl;
        std::cout << "║   Generating Contact Network...        ║" << std::endl;
        std::cout << "╚═════════════════════════════════════════╝\n" << std::endl;
        
        Graph network = Graph::generateRandom(num_people, num_edges, random_seed);
        
        std::cout << "✓ Network generated successfully!" << std::endl;
        std::cout << "  - Nodes (people):     " << network.getNumNodes() << std::endl;
        std::cout << "  - Edges (contacts):   " << num_edges << std::endl;
        std::cout << "  - Connected components: " << network.countConnectedComponents() << "\n" << std::endl;

        SimConfig config;
        config.base_infection_probability = 0.3;  // 30% baseline transmission
        config.recovery_days = 14;                 // Typical COVID-like: 14 days
        config.random_seed = random_seed;
        
        std::cout << "╔═════════════════════════════════════════╗" << std::endl;
        std::cout << "║   Simulation Configuration             ║" << std::endl;
        std::cout << "╚═════════════════════════════════════════╝\n" << std::endl;
        
        std::cout << "Base transmission probability: " << config.base_infection_probability << std::endl;
        std::cout << "Recovery period:               " << config.recovery_days << " days\n" << std::endl;

        CLI interactive_simulation(network, config);
        interactive_simulation.run();
        
        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}
