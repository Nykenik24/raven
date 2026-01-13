Raven
---
Raven is a fast, pragmatic general-purpose language mixing imperative and object-oriented programming.

# Build
Raven is made in C99, and uses Make as it's build system.

Right now there are no packages nor built executables, the only way to use Raven (although I don't know why you would want to with no compiler) is to build it from source:
```bash
# supposing you are in the repo's root
cd compiler
make
```

To try the non-existent compiler, run `/compiler/build/compiler`.

To generate `compile_commands.json`, ensure you have `bear` installed and run this instead of the normal `make` command:
```bash
bear -- make
```

Note that Raven will first support Linux, then Windows and finally MacOS (full support coming probably later than version 1).

# Roadmap

## Phase 1
> See [phase 1's description](https://git.ddoichita.dev/nykenik24/raven/src/branch/main/docs/plan.md#phase-1-the-prototype) here.

- [x] **Lexer**: converts the raw source into *tokens* (=words) inside a token list. [Source](./compiler/src/parser/lexer.c).
  - [x] Error handling. Can be enhanced, but it will evolve if other phases elaborate on error handling (`rvnerror.h`), so marked as done for now.
- [ ] **Parser**: converts the tokens into *nodes* (=phrases) inside a parse tree.
  - [ ] Error handling.
- [ ] **Simplifier**: converts the parse tree (which is complex, bloated and full of unnecessary nesting) into a much simpler AST (*Abstract Syntax Tree*).
- [ ] **Semantical analyzer**: analyzes the AST before compilation to check for semantic error before wasting resources compiling clearly illegal (semantically) code.
  - [ ] Error handling.
- [ ] **Optimizer**: tries to optimize the code as much as possible: optimizes loops, removes code that doesn't affect the output (dead code), reuses previously calculated values, among other optimizations. Not to be confused with optimiziation of Raven as whole.
  - [ ] Error handling.
- [ ] **Compiler**: generates *x86/ARM NASM* from the AST and packs it into a binary, which can be ran by the user.
  - [ ] Error handling.
- [ ] **Optimiziation**: optimize all previous phases. Not to be confused with the *optimizer*.
- [ ] **Porting**: add Windows/MacOS compatibilty.


<!-- ## Phase 2 -->
<!-- > See [phase 2's description](https://git.ddoichita.dev/nykenik24/raven/src/branch/main/docs/plan.md#phase-2-the-v1-compiler) here. -->

<!-- ## Phase 3 -->
<!-- > See [phase 3's description](https://git.ddoichita.dev/nykenik24/raven/src/branch/main/docs/plan.md#phase-3-the-stage-2-compiler) here. -->
