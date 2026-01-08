This is the plan that I'll follow to develop Raven:

## Phase 0: Completing the specification
Although the initial design (sketching) is nearly complete, the formal specification still remains.

The specification will serve as a comprehensive reference for development, achieving balance between high-level guidance and necessary detail. While design refinements will continue iteratively in later phases, completing the specification ensures a solid foundation for Phase 1 and subsequent development.

---
## Phase 1: the prototype
Phase 1 consists of a minimal compiler written in C that generates either C code or direct x86 NASM (only x86 for this phase), with the purpose of:

* Stress-testing the language with edge cases and illegal statements to ensure that both this phase and subsequent phases can handle maximum stress.
* Identifying design flaws, inconsistencies, and contradictions, and, overall, refining the design as much as possible.

The compiler will start as simple as possible and gradually increase in complexity with each iteration. Progression to the next phase, or continuation within this one, will be determined by the compiler’s ability to pass all tests, which include:

* Tiny tests of one or two lines that either **must** evaluate or **must not** evaluate.
* Multi-file programs that test a wide range of cases simultaneously.

Once the compiler has successfully:

* Refined the design so that flaws are (almost) eliminated.
* Structured the syntax and semantics to ensure subsequent phases can handle significant stress.

---
## Phase 2: the v1 compiler
After refining the design as much as possible in Phase 1, this phase focuses on transforming the minimal prototype compiler to a robust and polished version.

The compiler will prioritize optimizations and functionality over design, which has already been sufficiently iterated, while Phase 1 was not.

The purpose of this phase is to produce a compiler robust enough to be considered version 1 of Raven, built on extensive design iterations and capable of generating native assembly for x86/ARM.

> Raven will initially support Linux, followed by Windows, and finally macOS. Version 1 will only be released once all platforms are supported, though macOS may receive full support slightly later due to hardware availability.

---
## Phase 3: the stage 2 compiler
Once Raven is sufficiently stable and self-hosting is judged beneficial for development speed and convenience, a compiler written in Raven itself will be developed.

This phase, while planned and likely to occur, may take place in later versions and is not required for version 1. The timing of self-hosting cannot really be predicted and will be pursue when appropriate, after Phases 1 and 2.
