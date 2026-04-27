# Epidemic Simulation: SIR Model on Contact Networks

##  Project Overview

This is a **collaborative beginner-friendly C++ project** that implements an **SIR (Susceptible → Infected → Recovered) epidemic model** on a weighted contact network using **graph data structures and BFS traversal**.

### Core Concepts

- **Graph**: Nodes = people, Edges = contacts with frequency weights
- **SIR Dynamics**: People transition through susceptible → infected → recovered states
- **Infection Spread**: Uses BFS-like traversal from infected neighbors
- **Analysis**: Identify super-spreaders and compare vaccination strategies

## Project Structure

```
epidemic_sim/
├── CMakeLists.txt              ← Build configuration
├── README.md                   ← This file
├── include/
│   ├── graph.h                 
│   ├── simulator.h             
│   ├── analysis.h              
│   └── cli.h                  
└── src/
    ├── graph.cpp               
    ├── simulator.cpp           
    ├── analysis.cpp            
    ├── cli.cpp                 
    └── main.cpp                
```

---

## Building the Project

### Prerequisites
- C++17 compiler (g++, clang, or MSVC)
- CMake 3.10+

### Build Instructions

```bash
# Navigate to project directory
cd epidemic_sim

# Create build directory
mkdir build
cd build

# Generate build files
cmake ..

# Compile
cmake --build .

# Run
./epidemic_sim [--people 120] [--edges 450] [--seed 42]
```

### Example Usage

```bash
# Default 120 people, 450 edges, seed 42
./epidemic_sim

# Custom network
./epidemic_sim --people 200 --edges 800 --seed 123
```

---

##  Interactive Commands

Once the program starts, you'll see a prompt `>`. Try these commands:

```
# Run simulation for 10 steps
> run-simulation 10

# Infect specific nodes to study cascade effects
> infect 5
> infect 10

# See current state
> stats

# Quarantine someone to see impact
> quarantine 3
> release 3

# Find most influential spreaders
> super-spreaders

# Compare vaccination strategies
> vaccinate 20

# Get help
> help

# Exit
> quit
```

---

## Key Concepts for Beginners

### Graph & Adjacency List
- Each person (node) has a list of contacts (neighbors)
- Contact weight = frequency (0.0 to 1.0)
- Storage: O(V + E) instead of O(V²) for dense adjacency matrix

### BFS (Breadth-First Search)
- Explores graph level by level from a source
- In epidemic context: infected people "explore" their neighbors
- Time: O(V + E), Space: O(V)

### SIR Model
- **S (Susceptible)**: Can be infected
- **I (Infected)**: Actively spreading to neighbors
- **R (Recovered)**: Immune, cannot be infected again
- Transitions: S → I (contact × probability) → R (after recovery_days)

### Weighted Edges
- Contact frequency affects transmission probability
- Higher weight = more frequent contact = higher transmission risk
- `infection_prob = base_prob × weight`

### Vaccination as Node Removal
- Vaccinated people cannot be infected
- Simulating removing high-degree nodes shows effectiveness
- Degree-targeting beats random (in most networks)

---

## Example Complexity Analysis

For a network with **V = 100 people**, **E = 400 contacts**, running **T = 50 steps**:

| Operation | Complexity | Notes |
|-----------|-----------|-------|
| BFS once | O(V + E) | = O(500) |
| One step | O(V + E) | Infected → neighbors → BFS traversal |
| 50 steps | O(T × (V + E)) | = O(50 × 500) = O(25,000) |
| Super-spreaders (V removals) | O(V × T × (V + E)) | = O(100 × 50 × 500) = O(2.5M) – may take seconds |

---
