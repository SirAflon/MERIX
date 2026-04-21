# MERIX Compiler Reference
**Memory Register eXecution**
**MRX | Version 0.1**

---

## Table of Contents
1. [Overview](#1-overview)
2. [The Register Hierarchy](#2-the-register-hierarchy)
3. [Hardware Register Access](#3-hardware-register-access)
4. [Manual Virtual Registers](#4-manual-virtual-registers)
5. [Memory Layout](#5-memory-layout)
6. [The err Register](#6-the-err-register)
7. [Compilation Model](#7-compilation-model)
8. [Project File — .merp](#8-project-file--merp)
9. [Compiler Invocation](#9-compiler-invocation)

---

## 1. Overview

The MERIX compiler translates `.mrx` source files into C/C++ code in version 1. The resulting C/C++ is then compiled using a standard C/C++ compiler. In later versions the MERIX compiler will compile directly to native binary.

The compiler is written in C/C++ in version 1. Once the language is stable it will be rewritten in MERIX itself — bootstrapping the toolchain.

---

## 2. The Register Hierarchy

| Syntax | What it is | Who manages it |
|--------|-----------|----------------|
| `reg<r0>` | Actual hardware register — physically in the CPU | Programmer |
| `vreg<x>` | Virtual register — manual, not tied to scope | Programmer |
| `mem<16> x` | Automatic memory — compiler managed | Compiler |

The naming always tells you exactly where data lives. There is no hidden movement between levels.

---

## 3. Hardware Register Access — reg<>

`reg<registerName>` accesses an actual hardware register directly. Register names are identical to ASM names for the target CPU architecture.

### Move Operations

The `->` arrow maps directly to an ASM `mov` instruction. The arrowhead always points to the destination. Both directions produce identical output.

```
reg<r1> -> reg<r2>;           // move r1 into r2 — ASM: mov r1, r2
reg<r2> <- reg<r1>;           // identical — arrow flipped for readability
reg<r0> = reg<r1>;            // copy r1 into r0
```

### Mixed Expressions

`reg<>` is fully interchangeable with `mem<>` in any expression.

```
mem<16> a = reg<r16>;              // copy hardware register into memory
mem<16> a <- reg<r16>;             // same — arrow syntax
reg<r0> = reg<r1> + a;             // mixed arithmetic
reg<r16> = reg<ra16> + reg<rb16>;  // arithmetic on hardware registers
mem<16> a = reg<ra16> + reg<rb16>; // result into memory
```

### Accessing Virtual Register Storage

`reg<>` can access the hardware registers that physically hold virtual registers, giving full visibility into where the compiler placed data.

---

## 4. Manual Virtual Registers — vreg<>

`vreg<>` creates a named virtual register fully under programmer control. Not tied to any scope — lives until explicitly deleted.

### Creation

```
vreg<mem<16>> x = 65;        // 16-bit virtual register, value 65
vreg<mem<MyStruct>> s;       // virtual register for a struct
```

### Access

```
vreg<x>                      // access value — works like reg<> in any expression
```

### Register Operations

```
vreg<x> -> vreg<y>;          // move x into y
vreg<x> <- reg<r0>;          // move hardware register into x
reg<r0> -> vreg<x>;          // move x into hardware register
vreg<x> = vreg<y> + reg<r1>; // mixed arithmetic
```

### Deletion

```
vreg<x> = mem<>;             // delete x — frees the virtual register
```

The compiler issues a warning if a `vreg<>` is never deleted but still compiles successfully.

---

## 5. Memory Layout

### Current Behavior — Version 1

All `mem<>` sizes are rounded up to the nearest byte in RAM.

| Declared | RAM usage | Compiler enforced limit |
|----------|-----------|------------------------|
| `mem<1>` | 1 byte | 1 bit |
| `mem<4>` | 1 byte | 4 bits |
| `mem<8>` | 1 byte | 8 bits |
| `mem<16>` | 2 bytes | 16 bits |
| `mem<32>` | 4 bytes | 32 bits |
| `mem<64>` | 8 bytes | 64 bits |

The compiler enforces the declared bit limit. The programmer cannot read or write more bits than declared.

### Register-Pinned Memory

When memory is pinned to a hardware register with `mem<reg<r16>>`, the physical storage is the hardware register itself — not RAM.

```
mem<reg<r16>> a;             // all bits of r16
mem<reg<r16>, 10> a;         // 10 integer bits, rest fraction
mem<reg<r16>, 10, 5> a;      // 10 integer, 5 fraction, rest ignored
```

Changing `a` changes `r16` and vice versa — same bits.

---

## 6. The err Register

`err` is a built-in thread-local register automatically managed by the compiler. Every thread has its own independent `err`. It is platform-sized — matching the natural register size of the target CPU.

### Behavior

- `0` = success
- Any other value = error code
- Set automatically by the compiler after every function call
- Can also be set manually by the programmer

### Manual Setting

```
mem<32> doSomething() {
    err = 5;      // set error code
    return 0;
}

mem<32> doOther() {
    err = 0;      // explicitly signal success
    return 42;
}
```

### Compiler Flag

Automatic `err` setting can be disabled:

```
compiler --no-auto-err file.mrx
```

When disabled, `err` is only set when the programmer explicitly writes to it.

### Usage

```
doSomething(a, b);
if(err != 0) {
    // handle error
}
```

---

## 7. Compilation Model

### Version 1 — C/C++ Target

```
.mrx source files
       |
       v
MERIX Compiler (written in C/C++)
       |
       v
C/C++ source code
       |
       v
C/C++ Compiler (gcc, clang, msvc)
       |
       v
Native binary
```

All type macros are expanded before translation. The C/C++ output never contains `mem<>`, `vreg<>`, or `reg<>` directly.

### Compile-Time Polymorphism Output

When a struct inherits from a parent:
- Non-overridden methods are copied into the child struct in the output
- The parent struct is omitted from the output entirely if never directly instantiated
- `mem<StructName>` slots are resolved to the concrete type at compile time

### Later Versions

- Direct compilation to native binary
- Possibly other output targets

---

## 8. Project File — .merp

The `.merp` project file is the root of every MERIX project. Compiling without a project file compiles a single `.mrx` file only.

### Format

```
project {
    name = "MYOS";
    version = "0.1";
    target = ["x86", "ARCv3"];    // single value or array
    author = "YourName";           // single value or array
    build = "Debug";
    flags = ["-o", "v=c++17"];
}

files {
    x = "src/main.mrx";            // file with alias
    y = "src/kernel/memory.mrx";
    z = "src/kernel/console.mrx";
    "src/test.mrx";                 // no alias
    assets = "src/assets";          // entire directory
    y = assets/main.mrx;            // sub-path from aliased directory
    if(build == "Debug") {
        d = "src/debug.mrx";        // conditional file
    }
}

clusters {
    kernel = [y, z];
    if(build == "Release") {
        all = [x, kernel];
    }
}
```

### project { }

| Key | Type | Meaning |
|-----|------|---------|
| `name` | string | Project name |
| `version` | string | Version string |
| `target` | string or array | Target architecture(s) |
| `author` | string or array | Author name(s) |
| `build` | string | Current build mode — used in conditionals |
| `flags` | array | Compiler flags |

All values from `project { }` are accessible from every registered source file.

### files { }

- Files can have an alias or no alias
- Directories can be aliased — imports entire directory
- Sub-paths from aliased directories: `y = assets/main.mrx`
- Conditionals supported with `if(build == "...")`
- Files not listed in `files { }` cannot be included by other project files

### clusters { }

Named groups of files for easier importing.

```
include "kernel";    // imports both y and z
include "x";         // imports just x
```

---

## 9. Compiler Invocation

Single binary. Flags select tools and modes.

```
compiler file.mrx                         // compile single file
compiler --project project.merp           // compile full project
compiler --target x86 file.mrx            // specify target architecture
compiler --check file.mrx                 // check errors without compiling
compiler --meta file.mrx                  // run compile-time phase only
compiler --no-auto-err file.mrx           // disable automatic err setting
compiler --build Release --project p.merp // set build mode
```

---

*MERIX Compiler Reference — Version 0.1 — Draft*
