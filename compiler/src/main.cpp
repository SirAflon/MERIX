#include "lib/header/inputParser.h"
#include "header/frontend/lexer.h"
#include "header/frontend/syntax.h"
#include <fstream>
#include <string>
#include <chrono>
#include <vector>
#include <numeric>
#include <algorithm>
#include "debug/magic_enum.hpp"

int main(int argc, char* args[]){

    parsedCommand cmd = parseInput(argc, args);
    
    // --- Benchmark configuration ---
    const int ITERATIONS = 10;   // how many times to run lexer::entry
    std::vector<double> times(ITERATIONS);
    std::vector<lexer::TokenFile> lexedFiles;  // will keep result of first run

    // Warm-up (optional, but recommended)
    {
        std::vector<lexer::TokenFile> warmup;
        lexer::entry(cmd.inputFiles, cmd.flag, warmup);
    }

    // Run multiple iterations
    for (int i = 0; i < ITERATIONS; ++i) {
        std::vector<lexer::TokenFile> temp;
        auto start = std::chrono::high_resolution_clock::now();
        lexer::entry(cmd.inputFiles, cmd.flag, temp);
        auto end = std::chrono::high_resolution_clock::now();
        times[i] = std::chrono::duration<double, std::milli>(end - start).count();

        // Keep the tokens from the first run for output and syntax analysis
        if (i == 0) {
            lexedFiles = std::move(temp);
        }
    }

    // Compute statistics
    double total = std::accumulate(times.begin(), times.end(), 0.0);
    double avg = total / ITERATIONS;
    double minTime = *std::min_element(times.begin(), times.end());
    double maxTime = *std::max_element(times.begin(), times.end());

    // Output results (adjust output::print to your logging)
    output::print("Lexer benchmark (");
    output::print(std::to_string(ITERATIONS).c_str());
    output::print(" iterations): avg=");
    output::print(std::to_string(avg).c_str());
    output::print(" ms, min=");
    output::print(std::to_string(minTime).c_str());
    output::print(" ms, max=");
    output::println(std::to_string(maxTime).c_str());

    // Write token file (using the first run's result)
    std::ofstream outFile(cmd.outputFile);
    if (outFile) {
        for (const auto& tf : lexedFiles) {
            for (const auto& token : tf.tokens) {
                outFile << magic_enum::enum_name(token.kind) << " " << token.lexeme << "\n";
            }
        }
    }

    // Proceed to syntax analysis using the first run's tokens
    syntax::entry(lexedFiles, cmd.flag);
    
    return 0;
}