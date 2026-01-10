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
