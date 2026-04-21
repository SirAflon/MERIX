# MERIX Language Reference
**Memory Register eXecution**
**MRX | Version 0.1**

---

## Table of Contents
1. [The Bracket Rule System](#1-the-bracket-rule-system)
2. [Core Concepts](#2-core-concepts)
3. [Memory — mem<>](#3-memory--mem)
4. [Struct](#4-struct)
5. [Modifiers](#5-modifiers)
6. [Visibility](#6-visibility)
7. [Declarations](#7-declarations)
8. [Type Macros](#8-type-macros)
9. [Character & String Literals](#9-character--string-literals)
10. [Control Flow](#10-control-flow)
11. [Operators](#11-operators)
12. [Arrays & Indexing](#12-arrays--indexing)
13. [Modules](#13-modules)
14. [Object-Oriented Programming](#14-object-oriented-programming)
15. [Generics](#15-generics)
16. [Compile-Time System](#16-compile-time-system)
17. [Error Handling](#17-error-handling)
18. [Scope & Other Rules](#18-scope--other-rules)

---

## 1. The Bracket Rule System

MERIX has a strict and consistent bracket rule. Every bracket type has exactly one purpose across the entire language. There are no exceptions anywhere.

| Bracket | Rule |
|---------|------|
| `< >` | Metadata — compile-time descriptions of structure, types, and parameters |
| `( )` | Runtime expressions — values, conditions, and inputs evaluated at runtime |
| `{ }` | Code blocks — executable statements |
| `[ ]` | Index — range, values at index, or access by index |

---

## 2. Core Concepts

MERIX has only two real concepts at the language level. Everything else is built on top of these two.

- `mem<>` — a named piece of memory of a specific bit size
- `struct` — a named collection of memory declarations

---

## 3. Memory — mem<>

`mem<integerBits, fractionBits>` declares a named piece of memory.

- Fraction parameter defaults to `0` if omitted
- `mem<>` with no parameters is 0 bits — nothing — equivalent to void, null, and zero
- All bits default to zero at declaration — no uninitialized state exists in MERIX

### Sizes

| Syntax | Meaning |
|--------|---------|
| `mem<16, 10>` | 26 bit memory — 16 integer bits, 10 fraction bits |
| `mem<16>` | 16 bit memory — no fraction |
| `mem<>` | 0 bit memory — nothing, void, null |
| `mem<>*` | Universal pointer — points to any memory regardless of size |
| `mem<0>*` | Same as `mem<>*` — null pointer |

### Universal Pointer

`mem<>*` is a universal pointer like C's `void*`. The programmer is fully responsible for knowing what it points to and dereferencing it correctly. No implicit safety — same as C.

### Struct Memory Slots

`mem<StructName>` creates a memory slot that holds that struct or any child struct that inherits from it.

```
mem<Animal> a;     // holds an Animal or any child of Animal
mem<Dog> d;        // holds a Dog or any child of Dog
mem<Point> p;      // holds a Point — no children
```

### Register-Pinned Memory

| Syntax | Meaning |
|--------|---------|
| `mem<reg<r16>>` | Memory pinned to hardware register r16 |
| `mem<reg<r16>, 10>` | Pinned to r16 — 10 integer bits, rest fraction |
| `mem<reg<r16>, 10, 5>` | Pinned to r16 — 10 integer, 5 fraction, rest ignored |

> `mem<reg<r16>>*` is not valid — pinned memory cannot be pointed to.

### Memory Layout

Bit sizes are always rounded up to the nearest byte in RAM. The compiler enforces the declared bit limit — the programmer cannot exceed it.

```
mem<1> a;   // 1 bit logically, 1 byte in RAM — compiler enforces 1 bit limit
```

---

## 4. Struct

`struct` is the only real structure in the language. `class` is a type macro that expands to `struct`.

```
struct Point {
public:
    mem<32> x;
    mem<32> y;

    Point(mem<32> px, mem<32> py) {
        x = px;
        y = py;
    }

    ~Point() { }
};
```

---

## 5. Modifiers

| Modifier | Meaning |
|----------|---------|
| `signed` | Bits interpreted as signed value |
| `unsigned` | Bits interpreted as unsigned value |
| `const` | Immutable — cannot change after declaration |

### Implicit Casting Rules

- Bigger bit size always wins — both operands promoted to larger size
- If one operand is `signed` and the other `unsigned` — `signed` always wins regardless of size

```
unsigned mem<8> a = 5;
signed mem<64> b = 10;
// result: signed mem<64> — bigger size AND signed wins

unsigned mem<64> c = 20;
signed mem<8> d = 3;
// result: signed mem<64> — signed wins even though mem<8> is smaller
```

For explicit casting:
```
mem<64> a = 125;
mem<16> b = a;              // implicit
mem<16> c = mem<16>(a);     // explicit manual cast
```

---

## 6. Visibility

`public` and `private` work as both block labels and per-declaration keywords. The explicit keyword always overrides the block label.

```
public:
    mem<32> x;           // public — inherits block
    private mem<32> y;   // private — keyword overrides
    mem<32> z;           // public — inherits block
```

---

## 7. Declarations

| Declaration | Syntax |
|-------------|--------|
| Variable | `mem<32> x = 5;` |
| Constant | `const mem<32> x = 5;` |
| Pointer | `mem<32>* ptr;` |
| Reference | `mem<32>& ref;` |
| Function | `mem<32> add(mem<32> a, mem<32> b) { }` |
| Void function | `mem<> doThing(mem<32> a) { }` |
| Struct instance | `mem<MyStruct> s;` |

### Semicolon Rules

- Every statement ends with `;`
- `struct` and `class` definitions require `};`
- Function bodies — closing `}` never needs `;`
- Control flow blocks — closing `}` never needs `;`

```
struct Point {
    mem<32> x;
};                    // struct requires };

mem<> doThing() {
    mem<32> x = 5;    // statement — needs ;
}                     // function body — no ;

if(x > 0) {
    x = 1;
}                     // control flow — no ;
```

---

## 8. Type Macros

All familiar type keywords are type macros — expanded before compilation. The compiler never sees them.

| Macro | Expands to |
|-------|-----------|
| `char` | `unsigned mem<8>` |
| `short` | `signed mem<16>` |
| `int` | `signed mem<32>` |
| `long` | `signed mem<64>` |
| `bool` | `unsigned mem<1>` |
| `float` | `signed mem<8, 23>` |
| `double` | `signed mem<11, 52>` |
| `void` | `mem<>` |
| `null` / `nullptr` | `mem<>*` |
| `true` | `1` |
| `false` | `0` |
| `class` | `struct` |
| `unsigned int` | `unsigned mem<32>` |
| `signed char` | `signed mem<8>` |
| `unsigned float` | `unsigned mem<8, 23>` |
| `unsigned double` | `unsigned mem<11, 52>` |

### Macro Scope

Macros are scoped to where they are defined:
- Inside a function — available only inside that function
- At file level — available throughout that file
- In a top-level `<meta>` block — available project-wide after that point

---

## 9. Character & String Literals

- `'a'` is the bit value of that character as `mem<8>` — same as `97`
- `"hello"` is an array of `mem<8>` values — same as `[72, 101, 108, 108, 111]`
- No special string type — just `mem<8>` arrays
- All array operations work on strings automatically

```
unsigned mem<8>[..5] str = "hello";
// identical to:
unsigned mem<8>[..5] str = [72, 101, 108, 108, 111];
```

---

## 10. Control Flow

Identical to C/C++. No changes.

```
if(condition) { }
else if(condition) { }
else { }

for(mem<32> i = 0; i < 10; i++) { }

while(condition) { }

do { } while(condition);

switch(value) {
    case 1: break;
    default: break;
}

return value;
break;
continue;
```

---

## 11. Operators

### Standard Operators

All C/C++ operators. Precedence mostly identical to C/C++ with fixes to:
- `&&` vs `&` confusion
- `+` vs `*` mixing ambiguity
- Ternary `?:` precedence

### Bitwise Operators

Identical to C/C++ — `&`, `|`, `^`, `~`, `<<`, `>>`

### Operator Definition

```
// Binary — with precedence, left associative by default
mem<64> operation<'~', 10>(mem<64> a, mem<64> b) {
    return a * b % 10;
}

// Binary — right associative
mem<64> operation<'$', 10, right>(mem<64> a, mem<64> b) {
    return a + b;
}

// Binary — default precedence and associativity
mem<64> operation<'$'>(mem<64> a, mem<64> b) {
    return a + b;
}

// Unary
mem<64> operation<'-', 8>(mem<64> a) {
    return a * -1;
}
```

| Metadata slot | Meaning | Default |
|---------------|---------|---------|
| `o` — char literal | Operator symbol — bit value used | Required |
| `pl` — any type | Precedence level | C/C++ default for built-ins, lowest for custom |
| Associativity | `left` or `right` | `left` |

- Custom operator symbols freely invented
- Each operator is its own separate function
- Works on `mem<>`, `reg<>`, and `vreg<>`

---

## 12. Arrays & Indexing

`[ ]` is always the index bracket. `..` inside `[ ]` always means range.

### Declaration & Initialization

```
mem<32>[..3] arr = [1, 2, 56];   // indices 0, 1, 2
```

### Single Value Access

```
arr[2];                           // value at index 2
```

### Range — Copy

`arr[1..5]` always produces a new independent array copy.

```
mem<32>[..4] cArr = arr[1..5];   // new array, indices 1,2,3,4 copied
```

### Range — Reference

`arr[1..5]&` is a reference into the original array — no copy, same bits.

```
arr[1..5]& refName;              // read/write reference
const arr[1..5]& refName;        // read-only reference
```

Changing `refName` changes `arr` directly. `const` makes it read-only.

### Range Rules

| Syntax | Indices |
|--------|---------|
| `[..3]` | 0, 1, 2 |
| `[1..3]` | 1, 2 |
| `[5..15]` | 5, 6 ... 14 |

- Lower bound inclusive, upper bound exclusive
- No out of bounds protection — same as C/C++

---

## 13. Modules

```
include "path/to/module";
include "path/to/module" as Name;
```

- No partial imports — whole module or aliased whole module only
- `public:` / `private:` control what is visible outside the module

---

## 14. Object-Oriented Programming

### Compile-Time Polymorphism

MERIX has no virtual methods and no runtime dispatch. Polymorphism is compile-time only.

- Non-overridden methods are copied from parent into child by the compiler
- Parent produces zero compiled code if never directly instantiated
- `mem<StructName>` holds that struct or any child struct
- All method calls resolved at compile time — no vtable, no runtime lookup

```
mem<Animal>*[..3] animals = [Dog(arg), Cat(arg), Dog(arg)];
// compiler resolves every method call at compile time
// Animal produces no compiled code if never directly instantiated
```

### Rules

| Feature | Behavior |
|---------|----------|
| Inheritance | Multiple inheritance — same as C++ |
| Virtual methods | None — compile time only |
| Polymorphism | Through inheritance and generics — compile time |
| Constructor | Same name as struct |
| Destructor | `~` prefix |
| Method location | Inside or outside struct body |
| External method | `StructName::methodName()` |
| Self reference | `this` pointer |
| Override | `override` keyword |
| Non-overridden methods | Copied from parent into child at compile time |

### Example

```
struct Animal {
public:
    mem<8>[..32] name;
    Animal(mem<8>[..32] n) { name = n; }
    ~Animal() { }
    mem<> speak() { }
};

struct Dog : Animal {
public:
    mem<> speak() override { }
};

mem<Animal>* a = Dog("Rex");
```

---

## 15. Generics

### Parameter Keywords

| Keyword | Meaning |
|---------|---------|
| `typ T` | Type parameter |
| `val N` | Compile-time value parameter |

### Syntax

```
<typ T> T add(T a, T b) { return a + b; }

<typ T> {
    T add(T a, T b) { return a + b; }
    T subtract(T a, T b) { return a - b; }
}

<val N> mem<8>[..N] createBuffer() { }

<typ T, val N> {
    struct Buffer {
        T[..N] data;
    };
}
```

### Constraints

```
constraint Addable {
    typ T operation<'+'>( T a, T b);
}

<typ T: Addable> T add(T a, T b) { return a + b; }

<typ T requires operation<'+'>( T a, T b)> T add(T a, T b) { return a + b; }

<typ T: Addable requires operation<'-'>( T a, T b)> T addSub(T a, T b) { }
```

---

## 16. Compile-Time System

Everything compile-time lives inside `<meta>`. Same bracket rules apply inside `<meta>` as everywhere else.

```
<meta> define MAXSIZE = 256;
<meta> macro char = { unsigned mem<8> };
<meta> mem<32> calcSize(mem<32> n) { return n * 2; }

<meta> {
    define MAXSIZE = 256;
    if(PLATFORM == "x86") {
        define ARCH = "x86";
    }
    macro char = { unsigned mem<8> };
    macro double<typ T> = { T[..2] };
}
```

| Feature | Rule |
|---------|------|
| `define NAME = value` | Compile-time constant |
| `macro NAME = { }` | Code replacement |
| `macro NAME<typ T> = { }` | Parametric macro |
| Compile-time functions | Same syntax as runtime, inside `<meta>` |
| Control flow | Same keywords as runtime |
| Macro scope | Scoped to where defined |

---

## 17. Error Handling

### Exceptions — Same as C++

For unexpected fatal situations only. Disabled in bare-metal / kernel mode.

```
try {
    riskyOperation();
} catch(ErrorType e) { }
finally { }

throw SomeError();
```

### err Register

`err` is a permanent built-in thread-local register. Cannot be deleted. Every thread has its own.

| Property | Value |
|----------|-------|
| Size | Platform sized |
| Scope | Thread-local |
| Default success | `0` |
| Default error | Anything other than `0` |

```
doSomething(a, b);
if(err != 0) { }

// Setting manually
mem<32> doSomething() {
    err = 5;
    return 0;
}
```

### Rules

- Functions return one value only
- Error information communicated through `err`
- Recoverable errors → `err`
- Fatal errors → exceptions
- Bare-metal mode → `err` only, exceptions disabled

---

## 18. Scope & Other Rules

| Feature | Behavior |
|---------|----------|
| Scope rules | Same as C/C++ — inner scope can shadow outer |
| Function overloading | Same as C++ — same name, different parameters |
| Comments | `//` and `/* */` — identical to C/C++ |
| Semicolons | Every statement — `};` after structs, no `;` after function/control flow `}` |
| Default values | All bits default to zero |
| Null handling | `mem<>*` — programmer responsible, same as C `void*` |

---

*MERIX Language Reference — Version 0.1 — Draft*
