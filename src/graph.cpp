#include "graph.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <set>

// ============================================================================
// Madlen Minasyan : GRAPH IMPLEMENTATION
// ============================================================================

Graph::Graph(int num_nodes) : num_nodes(num_nodes) {
    nodes.resize(num_nodes);
    adjacency_list.resize(num_nodes);
    
    // Initialize nodes with default IDs
    for (int i = 0; i < num_nodes; i++) {
        nodes[i].id = i;
        nodes[i].name = "Person_" + std::to_string(i);
    }
}

void Graph::addNode(int id, const std::string& name) {
    if (id >= 0 && id < (int)nodes.size()) {
        nodes[id].name = name;
    }
}

void Graph::addEdge(int from, int to, double weight) {
    // Ensure weight is in valid range [0, 1]
    weight = std::max(0.0, std::min(1.0, weight));
    
    if (from >= 0 && from < num_nodes && to >= 0 && to < num_nodes && from != to) {
        // Add undirected edge: from→to and to→from
        adjacency_list[from].push_back(Edge(to, weight));
        adjacency_list[to].push_back(Edge(from, weight));
    }
}

void Graph::removeNode(int id) {
    if (id < 0 || id >= num_nodes) return;
    
    // Mark node as deleted by clearing its adjacency list
    adjacency_list[id].clear();
    
    // Remove edges pointing to this node
    for (int i = 0; i < num_nodes; i++) {
        auto& neighbors = adjacency_list[i];
        neighbors.erase(
            std::remove_if(neighbors.begin(), neighbors.end(),
                          [id](const Edge& e) { return e.to == id; }),
            neighbors.end()
        );
    }
}

const std::vector<Edge>& Graph::getNeighbors(int node_id) const {
    static const std::vector<Edge> empty;
    if (node_id < 0 || node_id >= num_nodes) {
        return empty;
    }
    return adjacency_list[node_id];
}

Person& Graph::getPerson(int node_id) {
    static Person dummy;
    if (node_id < 0 || node_id >= num_nodes) {
        return dummy;
    }
    return nodes[node_id];
}

const Person& Graph::getPerson(int node_id) const {
    static const Person dummy;
    if (node_id < 0 || node_id >= num_nodes) {
        return dummy;
    }
    return nodes[node_id];
}

std::vector<int> Graph::bfs(int source) const {
    std::vector<int> visit_order;
    if (source < 0 || source >= num_nodes) return visit_order;
    
    // visited[i] tracks if node i has been visited
    std::vector<bool> visited(num_nodes, false);
    std::queue<int> q;
    
    visited[source] = true;
    q.push(source);
    
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        visit_order.push_back(u);
        
        // Visit all unvisited neighbors
        for (const Edge& edge : adjacency_list[u]) {
            int v = edge.to;
            if (!visited[v]) {
                visited[v] = true;
                q.push(v);
            }
        }
    }
    
    return visit_order;
}

int Graph::countConnectedComponents() const {
    std::vector<bool> visited(num_nodes, false);
    int components = 0;
    
    for (int i = 0; i < num_nodes; i++) {
        if (!visited[i]) {
            components++;
            // BFS from unvisited node
            std::queue<int> q;
            q.push(i);
            visited[i] = true;
            
            while (!q.empty()) {
                int u = q.front();
                q.pop();
                
                for (const Edge& edge : adjacency_list[u]) {
                    if (!visited[edge.to]) {
                        visited[edge.to] = true;
                        q.push(edge.to);
                    }
                }
            }
        }
    }
    
    return components;
}

void Graph::resetAllNodes() {
    for (auto& person : nodes) {
        person.state = SIRState::SUSCEPTIBLE;
        person.days_infected = 0;
        person.quarantined = false;
        person.vaccinated = false;
    }
}

Graph Graph::generateRandom(int num_people, int num_edges, int random_seed) {
    srand(random_seed);
    Graph g(num_people);
    
    // Add random edges until we reach num_edges
    int edges_added = 0;
    std::set<std::pair<int, int>> added_pairs;  // Track edges to avoid duplicates
    
    while (edges_added < num_edges) {
        int from = rand() % num_people;
        int to = rand() % num_people;
        
        // Ensure from < to for undirected graph (avoid duplicates)
        if (from > to) std::swap(from, to);
        if (from == to) continue;  // No self-loops
        
        // Check if edge already exists
        if (added_pairs.find({from, to}) != added_pairs.end()) {
            continue;
        }
        
        // Add edge with random weight
        double weight = 0.3 + (rand() % 70) / 100.0;  // Random weight [0.3, 1.0]
        g.addEdge(from, to, weight);
        added_pairs.insert({from, to});
        edges_added++;
    }
    
    return g;
}

Graph Graph::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return Graph(0);
    }
    
    int num_nodes;
    file >> num_nodes;
    Graph g(num_nodes);
    
    int from, to;
    double weight;
    while (file >> from >> to >> weight) {
        g.addEdge(from, to, weight);
    }
    
    file.close();
    return g;
}
