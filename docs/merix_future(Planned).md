# MERIX Future Plans
**Memory Register eXecution**
**MRX | Version 0.1 — Looking Ahead**

---

## Table of Contents
1. [Overview](#1-overview)
2. [Planned Language Features](#2-planned-language-features)
3. [Planned Memory System Improvements](#3-planned-memory-system-improvements)
4. [Planned Compiler Improvements](#4-planned-compiler-improvements)
5. [Planned Toolchain](#5-planned-toolchain)
6. [Standard Library](#6-standard-library)
7. [OS Development Goals](#7-os-development-goals)
8. [Version Roadmap](#8-version-roadmap)

---

## 1. Overview

This document tracks everything planned for future versions of MERIX that is not part of version 1. Nothing here is decided in detail — these are intentions and directions. Each item will go through the same planning process as version 1 features before being implemented.

---

## 2. Planned Language Features

### Enums

Enums are planned for a future version. Since everything in MERIX is bits, an enum value is ultimately just a named bit pattern. The design will reflect that naturally.

**Open questions:**
- Are enums just named `mem<>` constants grouped under a name?
- Do they have a fixed bit size declared at definition?
- Can they be used in `switch` with exhaustive checking?
- Can they participate in the `operation<>` system?

### Inline Functions

The `inline` keyword is planned. In version 1 the compiler makes its own inlining decisions.

**Open questions:**
- Is `inline` a hint or a guarantee?
- Does it interact with the `<meta>` system?
- Can generics be forced inline?

### Additional Generic Features

- Variadic generics — a generic accepting any number of type parameters
- Generic specialization — different implementation for a specific concrete type
- Constraint inheritance — one constraint extending another

### Additional Operator Features

- Named precedence groups — group levels by name instead of raw numbers
- Ternary custom operators — operators taking three operands

### Sum Types / Tagged Unions

A built-in way to hold one of several possible types in a single slot, with a tag indicating which type is currently stored. Planned for a future version once the enum system is designed.

**Open questions:**
- Is this a new keyword like `variant`?
- Or is it built on top of enums and structs?
- How does it interact with `mem<StructName>` polymorphism?

### Optional Bounds Checking

An opt-in compiler flag for debug builds that adds runtime bounds checking to all array accesses. Version 1 has no bounds checking — same as C/C++.

```
compiler --bounds-check file.mrx    // planned flag
```

---

## 3. Planned Memory System Improvements

### RAM Optimization Flag

Groups small `mem<>` declarations together to reduce RAM usage.

- Eight `mem<1>` variables packed into one byte instead of eight bytes
- Slower to access — extra bit masking required
- Useful for embedded systems, OS kernel, memory-constrained environments
- **Planned flag:** `--optimize-ram`

### Memory Layout Flag

Controls the order in which the compiler lays out memory.

- **Programmer order** — memory laid out exactly as declared in source
- **Compiler optimized** — compiler reorders to minimize RAM usage through alignment
- **Planned flag:** `--layout-mode programmer` or `--layout-mode optimized`

### Additional Memory Safety Modes

All opt-in via compiler flags. Default always remains fast and unchecked.

| Planned flag | Behavior |
|---|---|
| `--bounds-check` | Runtime bounds checking on all array accesses |
| `--null-check` | Runtime checks on all pointer dereferences |
| `--leak-detect` | Warnings or errors for `vreg<>` never deleted |

---

## 4. Planned Compiler Improvements

### Native Binary Compilation

Version 1 outputs C/C++ code. A future version compiles directly to native binary.

**Target architectures planned:**
- x86 / x86-64
- ARM
- RISC-V
- Others as needed for the OS project

### MERIX Self-Hosting

The compiler will be rewritten in MERIX itself once the language is stable.

**The bootstrapping process:**
1. Version 1 compiler written in C/C++ — compiles MERIX to C/C++
2. MERIX compiler written in MERIX — compiled by version 1
3. MERIX compiler compiles itself — self-hosted from this point

### Additional Compilation Targets

- WebAssembly — for running MERIX in web environments
- Other languages — if use cases emerge

### Improved Error Messages

Better, more informative error messages are an ongoing planned improvement. Version 1 prioritizes correctness. Future versions focus on making errors as clear and actionable as possible — one of the stated improvements over C/C++.

---

## 5. Planned Toolchain

### Package Manager

Planned but not yet designed. Will integrate with the `.merp` project file system.

**Open questions:**
- Separate binary or compiler flag?
- How are packages versioned and distributed?
- How does it handle OS-level packages with no standard library dependency?
- Centralized or decentralized registry?

### Debugger

Approach not yet decided.

**Options under consideration:**
- GDB integration for version 1 — fast to implement, familiar
- Custom MERIX debugger later — natively aware of `mem<>`, `vreg<>`, `reg<>`
- Possibly both

**Key requirement:** The debugger must clearly show the full register hierarchy — hardware registers, virtual registers, and automatic memory — as distinct levels. Standard debuggers do not have this concept natively.

### Language Server (LSP)

Planned editor integration providing:
- Syntax highlighting
- Code completion
- Error checking while typing
- Go-to-definition
- Inline type and register information

---

## 6. Standard Library

Intentionally skipped for version 1. The language must be fully specified first.

**Confirmed principles:**
- Written entirely in MERIX
- Uses `<meta>` and type macros for platform-specific parts
- Must work with zero standard library in bare-metal / kernel mode — nothing is a hard dependency for OS code
- High-level components (strings, collections, IO) will exist but be entirely optional

**Open questions for the standard library planning phase:**
- Minimum viable standard library for version 1?
- How does it handle platform differences?
- Separate OS development library vs general application library?
- Are standard library components just regular MERIX modules?

---

## 7. OS Development Goals

One of the primary goals of MERIX is to write a complete operating system entirely within the language.

### Version 1 OS Capability

Already supported in version 1:
- Boot code — first instructions at machine power-on
- Bootloader logic — environment setup before kernel loads
- Kernel code — memory management, process scheduling, system calls
- Device drivers — direct hardware register access via `reg<>`
- Interrupt handlers — using `reg<>` and `vreg<>` for precise timing
- Memory-mapped I/O — reading and writing hardware at specific addresses

### Planned OS-Specific Features

**Interrupt handler syntax** — a dedicated way to mark a function as an interrupt handler, ensuring the compiler generates correct entry/exit code including saving and restoring all registers.

**Memory-mapped register definitions** — a way to declare hardware memory-mapped registers at specific addresses, accessible through the `reg<>` system.

**Bare-metal mode** — a formal compiler mode disabling all features requiring a runtime, including exceptions and automatic `err` setting.

**Calling convention control** — ability to specify the calling convention for a function. Useful for interfacing with hardware or other languages at the OS level.

---

## 8. Version Roadmap

Rough intended order. Not a firm commitment — priorities may change.

### Version 0.1 — Current
- Language specification complete
- Compiler written in C/C++
- Compiles to C/C++ code
- Core features: `mem<>`, `reg<>`, `vreg<>`, structs, generics, `<meta>`, OOP, arrays, operators, modules, error handling
- Project file system `.merp`
- All version 1 issues resolved — casting rules, range semantics, operator associativity, macro scope, `err` thread-safety, semicolon rules, compile-time polymorphism

### Version 0.2 — Compiler Improvements
- Better error messages
- More robust C/C++ output
- Initial debugger integration (GDB)
- Expanded compiler flag system

### Version 0.3 — Language Additions
- Enums
- Inline functions
- RAM optimization flag
- Memory layout flag
- Optional bounds checking flag

### Version 0.4 — Native Compilation
- Direct compilation to native binary for at least one architecture (x86-64)
- Begin removing dependency on C/C++ compiler

### Version 0.5 — Toolchain
- Package manager design and implementation
- Language server (LSP)
- Custom debugger (initial version)

### Version 0.6 — Standard Library
- Standard library design and initial implementation
- Platform abstraction layer
- OS development library

### Version 1.0 — Self-Hosting
- MERIX compiler rewritten in MERIX
- Compiler compiles itself
- Full native binary output
- Complete toolchain

---

*MERIX Future Plans — Version 0.1 — Living Document — Subject to Change*
