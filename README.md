# MERIX Language

**Version 0.1 Specification | Compiler In Development**

This repository contains the formal specification, reference compiler, and examples for the MERIX programming language.

## Contents

| Path | Purpose |
|------|---------|
| `docs/` | language documentation |
| `compiler/` | MRX compiler implementation |
| `examples/` | Runnable MERIX programs |
| `stdlib/` | Standard library (future) |

## Language Overview

MERIX reduces systems programming to two primitives:

- `mem<integerBits, fractionBits>` — A named piece of memory of exact bit size
- `struct` — A named collection of memory declarations
  
## Example

```merix
mem<32> add(mem<32> a, mem<32> b) {
    return a + b;
}
