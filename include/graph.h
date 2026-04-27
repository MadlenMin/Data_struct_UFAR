#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <string>
#include <unordered_map>
#include <queue>

// ============================================================================
// Madlen Minasyan : GRAPH FOUNDATION
// ============================================================================


// Enum for SIR states: Susceptible, Infected, Recovered
enum class SIRState {
    SUSCEPTIBLE = 0,
    INFECTED = 1,
    RECOVERED = 2
};

// Structure representing a single person in the network
struct Person {
    int id;                          // Unique identifier (0 to num_people-1)
    std::string name;                // Person's name
    SIRState state;                  // Current SIR state
    int days_infected;               // How many days this person has been infected (0 if not infected)
    bool quarantined;                // Is this person quarantined?
    bool vaccinated;                 // Is this person vaccinated?

    Person() : id(-1), name(""), state(SIRState::SUSCEPTIBLE), 
               days_infected(0), quarantined(false), vaccinated(false) {}
    
    Person(int id, const std::string& name)
        : id(id), name(name), state(SIRState::SUSCEPTIBLE), 
          days_infected(0), quarantined(false), vaccinated(false) {}
};

// Structure representing a contact edge between two people
struct Edge {
    int to;                          // Destination node ID
    double weight;                   // Contact frequency (0.0 to 1.0)

    Edge(int to, double weight) : to(to), weight(weight) {}
};

// Graph class: represents the contact network using weighted adjacency list
class Graph {
private:
    int num_nodes;                   // Total number of people in the network
    std::vector<Person> nodes;       // Array of Person structures
    std::vector<std::vector<Edge>> adjacency_list;  // Weighted adjacency list

public:
    // Constructor: initialize empty graph
    Graph(int num_nodes = 0);

    // Add a person to the network
    void addNode(int id, const std::string& name);

    // Add a contact edge between two people (undirected)
    // weight: contact frequency (0.0 to 1.0)
    void addEdge(int from, int to, double weight);

    // Remove a person from the network (used for super-spreader simulation)
    void removeNode(int id);

    // Get all neighbors of a given person
    const std::vector<Edge>& getNeighbors(int node_id) const;

    // Get person by ID
    Person& getPerson(int node_id);
    const Person& getPerson(int node_id) const;

    // Generate a random network with num_people nodes and num_edges edges
    static Graph generateRandom(int num_people, int num_edges, int random_seed);

    // Load network from a file
    // Format: first line = num_nodes, then each line: from to weight
    static Graph loadFromFile(const std::string& filename);

    // BFS traversal starting from source node
    // Returns order in which nodes are visited
    std::vector<int> bfs(int source) const;

    // Check if graph has connected components
    int countConnectedComponents() const;

    // Getters
    int getNumNodes() const { return num_nodes; }
    const std::vector<Person>& getNodes() const { return nodes; }
    std::vector<Person>& getMutableNodes() { return nodes; }
    
    // Reset all nodes to initial state
    void resetAllNodes();
};

#endif 
