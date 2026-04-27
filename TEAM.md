# Teamwork Documentation – Epidemic Simulation Project

##  Project Overview

This project consists of the development of an **epidemic simulation system based on the SIR model (Susceptible–Infected–Recovered)**, implemented in **C++**. The goal is to simulate how a disease spreads across a population modeled as a graph, where nodes represent individuals and edges represent interactions between them.

Beyond simple simulation, the project also includes **analysis tools and user interaction features**, making it possible to experiment with different epidemic scenarios and intervention strategies.

The work was carried out collaboratively by **Lia Sargsyan Madlen Minasyan Milena Yeganyan**, each responsible for a major component of the system.

---

##  Team Contributions

###  Madlen Minasyan — Graph Design

Was responsible for building the **core infrastructure of the project**, which serves as the backbone for all other components.

Their primary task was to design and implement the **graph data structure** used to represent the population and the relationships between individuals.

**Main responsibilities:**

- Designing the fundamental data structures:
  - A `Person` structure to store each individual's state (Susceptible, Infected, or Recovered) and related attributes
  - An `Edge` structure to represent connections and interaction strength between individuals
  - An `SIRState` enumeration for clear state management

- Implementing the **Graph class**, which includes:
  - Adding and removing nodes (individuals)
  - Managing edges (contacts between individuals)
  - Accessing and modifying node data
  - Retrieving neighbors of a given node

- Developing **graph traversal algorithms**, such as:
  - Breadth-First Search (BFS)
  - Connected components detection

- Creating **network generation and loading tools**:
  - Random graph generation for simulations
  - File-based graph construction for reproducible experiments

- Providing a **reset mechanism** to restore the graph to its initial state between simulations

**Contribution impact:**

This component is essential because it provides the **structural model of the population**. All simulation and analysis features rely directly on this implementation, making it a critical foundation for the entire project.

---

### Lia Sargsyan — Simulation Engine & Epidemic Dynamics

Focused on implementing the **epidemic simulation engine**, which brings the graph to life by modeling how the disease spreads over time.

This part transforms a static network into a **dynamic system** where individuals change state according to probabilistic rules.

**Main responsibilities:**

- Designing simulation configuration structures:
  - `SimConfig` to define parameters such as infection probability and recovery duration
  - `StepStats` to track the number of susceptible, infected, and recovered individuals at each step

- Implementing the **core simulation logic**:
  - A step-based simulation process where each iteration represents a unit of time
  - Infection spread based on contact between individuals and associated probabilities
  - Recovery mechanics that transition individuals from infected to recovered after a certain duration

- Developing key simulation functions:
  - `spreadStep()` to process one iteration of the epidemic
  - `tryInfect()` to determine whether transmission occurs between individuals
  - `updateRecovery()` to manage recovery timing

- Adding **control features** for experimentation:
  - Running multiple simulation steps automatically
  - Manually infecting selected individuals
  - Introducing quarantine measures and releasing individuals
  - Resetting the simulation state

- Tracking and storing **simulation statistics over time** for later analysis

**Contribution impact:**

This component acts as the **core engine of the project**, enabling realistic modeling of epidemic behavior. It allows users to observe how diseases propagate and how different parameters influence the outcome.

---

### Milena Yeganyan — Analysis Tools & User Interaction

Was responsible for both the **analytical layer** of the project and the **user interface**, ensuring that the simulation results are not only generated but also interpretable and accessible.

Their work connects the technical system to the user, turning the project into a **practical and interactive tool**.

**Main responsibilities:**

- Implementing **analysis features** to extract meaningful insights from simulations:
  - Identification of **super-spreaders** (highly influential individuals in the network)
  - Development of **vaccination strategies**, such as:
    - Random vaccination
    - Targeted vaccination based on node connectivity
  - Tools to **compare different strategies** and evaluate their effectiveness

- Creating **epidemic metrics**:
  - Total number of infected individuals
  - Peak infection level
  - Overall spread rate (attack rate)

- Developing the **Command-Line Interface (CLI)**:
  - Parsing user commands and inputs
  - Allowing users to control the simulation interactively
  - Providing commands to:
    - Run simulations
    - Infect individuals
    - Apply or remove quarantine
    - Display statistics and results

- Designing **clear and structured output**:
  - Readable display of simulation progress
  - Well-formatted presentation of analysis results

- Managing the **main program flow**:
  - Initializing the system
  - Connecting all modules together
  - Ensuring smooth execution from start to finish

**Contribution impact:**

This component makes the project **usable and insightful**. Without it, the simulation would remain a purely technical system. The work enables users to interact with the model, test scenarios, and draw conclusions from the results.

---

##  Collaboration & Integration

The success of the project relied on strong collaboration and a well-defined architecture.

- The system follows a **layered modular design**:
  - The **Graph module** provides the structural data
  - The **Simulation module** operates on this structure
  - The **Analysis and CLI modules** interpret and present the results

- Each member worked on a **clearly defined component**, reducing overlap and improving efficiency.

- Integration was facilitated through:
  - Well-designed header files (`.h`) defining interfaces
  - Consistent data structures shared across modules
  - Regular coordination to ensure compatibility between components

---

##  Teamwork Highlights

- Clear division of responsibilities across three major components  
- Strong modular architecture enabling parallel development  
- Effective integration of independent modules into a cohesive system  
- Combination of algorithmic concepts (graphs, BFS, probability) with practical implementation  
- Balanced contribution leading to a complete and functional application  

---

##  Conclusion

This project demonstrates a successful collaborative effort in building a complex system from the ground up. By dividing the work into **core infrastructure, simulation logic, and analysis/interface**, the team was able to efficiently develop a robust epidemic simulation platform.

Each member played a crucial role, and the final result reflects both **technical competence** and **effective teamwork**.