#include "lib/header/inputParser.h"
#include "header/frontend/lexer.h"
#include "header/frontend/syntax.h"
#include "lib/header/syntaxTree.h"   // for NodeKind and AST node definitions
#include <fstream>
#include <string>
#include <chrono>
#include <vector>
#include <numeric>
#include <algorithm>
#include "debug/magic_enum.hpp"

// -----------------------------------------------------------------------------
// Helper: convert NodeKind to string (fallback if magic_enum not available)
// -----------------------------------------------------------------------------
std::string_view nodeKindToName(syntax::NodeKind kind) {
    // Use magic_enum if available
    #ifdef MAGIC_ENUM_HPP
        return magic_enum::enum_name(kind);
    #else
        // Manual mapping – extend as needed
        switch (kind) {
            case syntax::NodeKind::TranslationUnit: return "TranslationUnit";
            case syntax::NodeKind::VariableDecl:    return "VariableDecl";
            case syntax::NodeKind::FunctionDecl:    return "FunctionDecl";
            case syntax::NodeKind::IntegerLiteral:  return "IntegerLiteral";
            case syntax::NodeKind::IdentifierExpr:  return "IdentifierExpr";
            // ... add other cases
            default: return "Unknown";
        }
    #endif
}

// -----------------------------------------------------------------------------
// Dump a single AST node (simple recursive pretty‑printer)
// -----------------------------------------------------------------------------
void dumpAST(std::ostream& out, const syntax::Node* node, int indent = 0) {
    if (!node) return;
    std::string indentStr(indent * 2, ' ');
    out << indentStr << nodeKindToName(node->kind);

    // Print additional details for specific node types
    switch (node->kind) {
        case syntax::NodeKind::VariableDecl: {
            auto* v = static_cast<const syntax::VariableDecl*>(node);
            out << " name='" << v->name << "'";
            if (v->is_const) out << " const";
            break;
        }
        case syntax::NodeKind::IntegerLiteral: {
            auto* lit = static_cast<const syntax::IntegerLiteral*>(node);
            out << " value=" << lit->value;
            break;
        }
        case syntax::NodeKind::IdentifierExpr: {
            auto* id = static_cast<const syntax::IdentifierExpr*>(node);
            out << " name='" << id->name << "'";
            break;
        }
        case syntax::NodeKind::BinaryOpExpr: {
            auto* bin = static_cast<const syntax::BinaryOpExpr*>(node);
            out << " op=" << magic_enum::enum_name(bin->op);
            break;
        }
        // Add more cases for FunctionDecl, StructDecl, etc.
        default:
            break;
    }
    out << "\n";

    // Recurse into children – this is simplified; you should add proper
    // recursion for each node type (e.g., block statements, expressions).
    // For demonstration, we only recurse into VariableDecl's initializer.
    if (node->kind == syntax::NodeKind::VariableDecl) {
        auto* v = static_cast<const syntax::VariableDecl*>(node);
        if (v->initializer) {
            dumpAST(out, v->initializer.get(), indent + 1);
        }
    }
    else if (node->kind == syntax::NodeKind::BinaryOpExpr) {
        auto* bin = static_cast<const syntax::BinaryOpExpr*>(node);
        dumpAST(out, bin->left.get(), indent + 1);
        dumpAST(out, bin->right.get(), indent + 1);
    }
}

// -----------------------------------------------------------------------------
// Dump a whole TranslationUnit
// -----------------------------------------------------------------------------
void dumpTranslationUnit(std::ostream& out, const syntax::TranslationUnit* tu) {
    if (!tu) return;
    out << "TranslationUnit\n";
    for (const auto& decl : tu->declarations) {
        dumpAST(out, decl.get(), 1);
    }
}

// -----------------------------------------------------------------------------
// Main
// -----------------------------------------------------------------------------
int main(int argc, char* args[]) {
    parsedCommand cmd = parseInput(argc, args);

    // --- Lexer benchmark & tokenisation ---
    const int ITERATIONS = 1;
    std::vector<double> times(ITERATIONS);
    std::vector<lexer::TokenFile> lexedFiles;

    // Warm-up
    {
        std::vector<lexer::TokenFile> warmup;
        lexer::entry(cmd.inputFiles, cmd.flag, warmup);
    }

    // Run benchmark
    for (int i = 0; i < ITERATIONS; ++i) {
        std::vector<lexer::TokenFile> temp;
        auto start = std::chrono::high_resolution_clock::now();
        lexer::entry(cmd.inputFiles, cmd.flag, temp);
        auto end = std::chrono::high_resolution_clock::now();
        times[i] = std::chrono::duration<double, std::milli>(end - start).count();
        if (i == 0) {
            lexedFiles = std::move(temp);
        }
    }

    // Print statistics
    double total = std::accumulate(times.begin(), times.end(), 0.0);
    double avg = total / ITERATIONS;
    double minTime = *std::min_element(times.begin(), times.end());
    double maxTime = *std::max_element(times.begin(), times.end());

    output::print("Lexer benchmark (");
    output::print(std::to_string(ITERATIONS).c_str());
    output::print(" iterations): avg=");
    output::print(std::to_string(avg).c_str());
    output::print(" ms, min=");
    output::print(std::to_string(minTime).c_str());
    output::print(" ms, max=");
    output::println(std::to_string(maxTime).c_str());

    // --- Write lexer output to outLexer.txt ---
    std::ofstream lexOut("outLexer.txt");
    if (lexOut) {
        for (const auto& tf : lexedFiles) {
            lexOut << "File: " << tf.filePath << "\n";
            for (const auto& token : tf.tokens) {
                lexOut << magic_enum::enum_name(token.kind) << " " << token.lexeme << "\n";
            }
            lexOut << "\n";
        }
        output::println("Lexer tokens written to outLexer.txt");
    } else {
        output::println("Failed to open outLexer.txt");
    }

    // --- Syntax analysis ---
    std::vector<syntax::Syntax> syn;
    syntax::entry(lexedFiles, cmd.flag, syn);

    // --- Write syntax tree to outSyntax.txt ---
    std::ofstream synOut("outSyntax.txt");
    if (synOut) {
        for (size_t i = 0; i < syn.size(); ++i) {
            synOut << "File: " << syn[i].filePath << "\n";
            dumpTranslationUnit(synOut, syn[i].tree.get());
            synOut << "\n";
        }
        output::println("Syntax tree written to outSyntax.txt");
    } else {
        output::println("Failed to open outSyntax.txt");
    }

    return 0;
}