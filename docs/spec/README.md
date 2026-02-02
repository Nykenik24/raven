<h1 align="center">Raven</h1>

## Index
1. [Syntax](./syntax/README.md)
    1. [Abstract syntax](./syntax/abstract_syntax.md)
    2. [Concrete syntax](./syntax/concrete_syntax.md)
    3. [Lexer](./syntax/lexer/README.md) *TODO*
        1. [Tokens](./syntax/lexer/tokens.md) *TODO*
    4. [Parser](./syntax/parser/README.md) *TODO*
        1. [Trees](./syntax/parser/trees.md) *TODO*
1. [Semantics](./semantics/README.md) *TODO*
    1. [Static semantics](./semantics/static/README.md) *TODO*
    2. [Dynamic semantics](./semantics/dynamic/README.md) *TODO*
1. [Compilation](./compiling/README.md) *TODO*
    1. [Code generation](./compiling/codegen.md) *TODO*
    2. [Optimization](./compiling/optimization.md) *TODO*
---

## Table of contents
1. [Overview](README#Overview)
2. [Programs](README#Programs)

## Overview
Raven is a general-purpose, low-level pragmatic programming language designed specifically to be as explicit as possible.

Raven prioritizes *comfort* and *reliability* over 
- Preference of specific groups of programmers.
- Terseness of code.
- Abstract concepts (even if they might favor the language's range of use cases).

Raven also aims to be completely at the user's control, with:
- Configuration of the compiler's behavior (file output, flags, log level, *strictness* [which, for example, mandates if the compiler should treat warnings as errors too], etc.).
- Removable modules (allowing for the removal of standard library in the program or certain parts of it to make programs more lightweight).

## Programs
Programs in Raven consist of **one or more** declaration(s).

Programs MUST include the `main` function **only once**.

Programs CAN declare (aside from the `main` function):
- Other [functions]().
- File [scope]() variables.
- [Structures]().
- [Enumeration]().
