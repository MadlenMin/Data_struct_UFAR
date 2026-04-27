#ifndef CLI_H
#define CLI_H

#include "simulator.h"
#include "analysis.h"
#include <string>
#include <vector>

// ============================================================================
// Milena Yeganyan and Lia Sargsyan: COMMAND-LINE INTERFACE & MAIN PROGRAM FLOW
// ============================================================================

class CLI {
private:
    Simulator simulator;                // The simulation engine
    Analyzer analyzer;                  // The analysis engine
    bool running;                       // Is the REPL active?

public:
    // Constructor: initialize CLI with graph and config
    CLI(const Graph& graph, const SimConfig& config);

    // Main REPL (Read-Eval-Print Loop)
    void run();

    // Parse and execute a single command
    // Returns false if "quit" command is issued
    bool executeCommand(const std::string& command_line);

private:
    // Command handlers
    void cmdRunSimulation(const std::vector<std::string>& args);
    void cmdInfect(const std::vector<std::string>& args);
    void cmdQuarantine(const std::vector<std::string>& args);
    void cmdRelease(const std::vector<std::string>& args);
    void cmdStats(const std::vector<std::string>& args);
    void cmdSuperSpreaders(const std::vector<std::string>& args);
    void cmdVaccinate(const std::vector<std::string>& args);
    void cmdReset(const std::vector<std::string>& args);
    void cmdHelp(const std::vector<std::string>& args);

    // Utility functions
    std::vector<std::string> tokenize(const std::string& input);
    void displayStats();
    void displayHistory();
    void printSimulationSummary();
};

#endif
