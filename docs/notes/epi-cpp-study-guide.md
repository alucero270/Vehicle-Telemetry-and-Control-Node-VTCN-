# EPI C++ Study Guide

This note is a condensed interview-prep companion based on:

- `C:\Users\Alex Lucero\Nextcloud\Media\books\Elements of Programming Interviews in C++.pdf`

Use it alongside the repo-specific notes already in `docs/notes/`.

## What The Book Covers

The book is organized into five major sections:

- Introduction
- Part I: interview process and problem solving
- Part II: core coding problems
- Part III: domain-specific and language/design questions
- Part IV: honors-class problems

The highest-yield coding chapters are concentrated in Part II:

- Chapter 5: Primitive Types
- Chapter 6: Arrays
- Chapter 7: Strings
- Chapter 8: Linked Lists
- Chapter 9: Stacks and Queues
- Chapter 10: Binary Trees
- Chapter 11: Heaps
- Chapter 12: Searching
- Chapter 13: Hash Tables
- Chapter 14: Sorting
- Chapter 15: Binary Search Trees
- Chapter 16: Recursion
- Chapter 17: Dynamic Programming
- Chapter 18: Greedy Algorithms and Invariants
- Chapter 19: Graphs

The later chapters are still useful, but they are less likely to dominate a
general coding screen:

- Chapter 20: Parallel Computing
- Chapter 21: Design Problems
- Chapter 22: Language Questions
- Chapter 23: Object-Oriented Design
- Chapter 24: Common Tools
- Chapter 25: Honors Class

## Priority Order

If your goal is interview return on time invested, use this order.

### Tier 1: Must-know

These chapters show up constantly and form the base for most interview loops:

- 5 Primitive Types
- 6 Arrays
- 7 Strings
- 9 Stacks and Queues
- 10 Binary Trees
- 12 Searching
- 13 Hash Tables
- 14 Sorting
- 15 Binary Search Trees

Target outcome:

- You can recognize the core pattern within 2 to 5 minutes.
- You can state a brute-force approach and improve it.
- You can implement the optimized version without major syntax stalls.

### Tier 2: Very high value

These chapters matter a lot once the interviewer moves past basics:

- 8 Linked Lists
- 11 Heaps
- 16 Recursion
- 18 Greedy Algorithms and Invariants
- 19 Graphs

Target outcome:

- You can move comfortably between recursive and iterative thinking.
- You can choose the right traversal, auxiliary data structure, or invariant.

### Tier 3: Common differentiators

These topics separate average performance from strong performance:

- 17 Dynamic Programming
- 20 Parallel Computing
- 22 Language Questions
- 23 Object-Oriented Design

Target outcome:

- You can explain tradeoffs, not just produce code.
- You can discuss performance, correctness, and language-level pitfalls.

### Tier 4: Stretch work

Use this tier after the others are stable:

- 21 Design Problems
- 24 Common Tools
- 25 Honors Class

Target outcome:

- You are building depth, not just passing screens.

## Recommended Study Path

### Phase 1: Interview basics

Read these first:

- Chapter 1: Getting Ready
- Chapter 2: Strategies For A Great Interview
- Chapter 4: Problem Solving

Why:

- These chapters define the expected interview loop: clarify, propose,
  analyze, implement, test, refine.
- They help you avoid the common failure mode of coding too early.

### Phase 2: Core data structures and search

Work these chapters next:

- 5 Primitive Types
- 6 Arrays
- 7 Strings
- 9 Stacks and Queues
- 12 Searching
- 13 Hash Tables
- 14 Sorting

Why:

- Many tree, graph, and DP problems reduce to these building blocks.
- This phase gives you the fastest improvement in coding screens.

### Phase 3: Pointer-heavy and traversal-heavy problems

Then do:

- 8 Linked Lists
- 10 Binary Trees
- 15 Binary Search Trees
- 19 Graphs

Why:

- These chapters train pointer reasoning, traversal state, and visitation
  discipline.

### Phase 4: Algorithmic maturity

Then add:

- 16 Recursion
- 17 Dynamic Programming
- 18 Greedy Algorithms and Invariants
- 11 Heaps

Why:

- This is where interviewers test whether you can derive structure rather than
  only recognize surface-level patterns.

### Phase 5: C++ and system-level breadth

Close with:

- 20 Parallel Computing
- 22 Language Questions
- 23 Object-Oriented Design
- selected problems from 21 and 25

Why:

- These chapters are especially helpful for C++ interviews, systems roles, and
  more senior loops.

## Four-Week Prep Plan

This is a practical plan if you are studying around work.

### Week 1

- Read Chapters 1, 2, and 4.
- Solve from Chapters 5, 6, and 7.
- Focus on bit tricks, arrays, string scanning, two pointers, and indexing.

### Week 2

- Solve from Chapters 9, 12, 13, and 14.
- Focus on stacks, queues, binary search variants, hash-based lookup, and
  interval/sorting workflows.

### Week 3

- Solve from Chapters 8, 10, 15, and 11.
- Focus on linked list manipulation, DFS/BFS on trees, BST ordering logic, and
  heap-based top-k / streaming problems.

### Week 4

- Solve from Chapters 16, 17, 18, and 19.
- Use Chapter 22 for C++ refresh.
- Use Chapter 23 for higher-level design vocabulary.

## High-Yield Patterns To Master

When you work problems, group them by reusable pattern rather than by chapter
title alone.

- Bit manipulation
  - parity
  - swap bits
  - reverse bits

- Two pointers / scanning
  - partitioning
  - palindromes
  - subarray and substring windows

- Prefix information
  - prefix sums
  - rolling state
  - hash-map memo of prior positions

- Binary search on answer or structure
  - search in sorted data
  - first/last occurrence
  - rotated arrays
  - feasibility search

- Stack discipline
  - parsing
  - monotonic stack
  - path simplification

- Heap discipline
  - k-way merge
  - top-k
  - streaming median

- Tree traversals
  - preorder / inorder / postorder
  - level-order BFS
  - parent-aware traversal

- Recursion and backtracking
  - permutations
  - combinations
  - subsets
  - constrained search

- Dynamic programming
  - 1D recurrence
  - 2D grid state
  - subsequence / edit distance
  - knapsack-style transitions

- Greedy and invariants
  - interval scheduling
  - majority element
  - skyline / water / area reasoning

- Graph traversal
  - reachability
  - cycle detection
  - shortest path
  - connected components

## C++ Topics From Chapter 22 To Rehearse

Do not leave the language chapter for the very end if you are interviewing for
a C++ role.

From the table of contents, make sure you can speak clearly about:

- references and pointers
- pass-by-reference vs pass-by-value
- smart pointers
- iterators
- constructors
- default methods
- `malloc` / `free` vs `new` / `delete`
- strings
- `push_back()` and `emplace_back()`
- map updates
- fast function calls
- template functions
- run-time type identification
- dynamic linkage

For interview prep, the expected standard is not textbook completeness. The
goal is to explain tradeoffs and avoid basic misuse.

## What Strong Performance Looks Like

For each solved problem, you should be able to do all of the following:

- restate the problem in your own words
- identify the critical constraint
- give a correct brute-force baseline
- derive the improved approach
- state time and space complexity
- name the edge cases before coding
- implement cleanly in C++
- dry-run one normal case and one edge case

If you can only code the answer after seeing the pattern name, you are not yet
interview-ready.

## Suggested Problem Mix

Aim for this approximate mix when practicing:

- 60 percent Tier 1 chapters
- 25 percent Tier 2 chapters
- 10 percent DP and graph work
- 5 percent stretch problems from Chapters 21 to 25

That mix is better for most interview loops than spending too much time on
hard puzzles early.

## How This Fits Your Existing Repo Notes

Use the current notes like this:

- `cpp-interview-prep-plan.md`
  - project narrative and modern C++ discussion

- `cpp-patterns-crosswalk.md`
  - mapping practical C++ patterns onto this repo

- this note
  - algorithmic interview study plan based on EPI

That gives you both halves of a typical C++ interview:

- code and algorithms
- project discussion and engineering judgment
