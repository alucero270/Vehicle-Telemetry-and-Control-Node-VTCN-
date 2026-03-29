# EPI C++ Problem-Solving Playbook

This note turns the `Elements of Programming Interviews in C++` chapter layout
into a reusable way to practice and perform in interviews.

## Standard Interview Loop

For almost every coding problem, use the same sequence.

### 1. Clarify

Ask just enough to remove ambiguity:

- input size
- allowed mutation
- duplicate handling
- sorted vs unsorted input
- expected return format
- error or edge-case behavior

Avoid writing code before you know the shape of the problem.

### 2. State The Baseline

Say the obvious correct solution first, even if it is slow.

Why this matters:

- it proves you understand the problem
- it gives you a comparison point for optimization
- it shows the interviewer you can reason step by step

### 3. Find The Lever

Most improvements come from one of a small set of levers:

- extra memory for speed
- sorting to impose structure
- a stronger invariant
- binary search on ordered behavior
- precomputation
- recursion over a constrained state space
- dynamic programming over overlapping subproblems

If you do not know what to do next, ask: what information am I recomputing?

### 4. Choose The Right Representation

A large fraction of interview performance is just choosing the correct core
container:

- `vector`
- `string`
- `unordered_map`
- `unordered_set`
- `deque`
- `queue`
- `stack`
- `priority_queue`

If your representation is wrong, the algorithm will feel harder than it is.

### 5. Code In Small Safe Steps

During implementation:

- define helper functions only when they reduce risk
- keep loop invariants explicit
- use descriptive variable names
- avoid changing multiple indices in the same line
- handle boundaries early

### 6. Test Actively

Before declaring done, run these cases verbally:

- empty input
- one element
- two elements
- duplicates
- all same values
- already sorted or reverse sorted
- min and max values
- cycle or null cases for pointer structures

## Pattern Recognition By Chapter

These are the main signals to look for while practicing from EPI.

### Primitive Types

If the problem mentions bits, masks, parity, power-of-two structure, or integer
digits, think:

- bitwise operators
- shifting
- masking
- modular arithmetic

Common trap:

- signed overflow or undefined behavior from careless shifts

### Arrays And Strings

If the data is linear and indexable, try these first:

- two pointers
- sliding window
- in-place partitioning
- prefix information
- sorting plus scan

Common trap:

- starting with a hash map when the problem really wants an ordering-based scan

### Linked Lists

If the input is pointer-chasing data, check whether the core move is:

- reversal
- split and merge
- fast/slow pointer
- dummy head
- cycle detection

Common trap:

- losing the rest of the list during rewiring

### Stacks And Queues

If the problem involves nesting, nearest greater/smaller relationships, or
incremental visibility, consider:

- explicit stack
- monotonic stack
- BFS queue
- deque for window state

Common trap:

- using recursion when an explicit stack makes the state clearer

### Trees And BSTs

For tree problems, decide early:

- DFS or BFS
- recursive or iterative
- whether parent context matters
- whether BST ordering gives pruning

Common trap:

- starting traversal without defining what each call or queue entry means

### Heaps

If the problem says:

- top `k`
- streaming median
- merge sorted streams
- repeatedly extract min or max

then a heap is a strong default.

Common trap:

- sorting the full input when only partial order is needed

### Searching

Binary search is not just "find a target".

Use it when:

- the predicate is monotonic
- you need first true / last false
- the answer space is ordered
- the data is rotated or partially structured

Common trap:

- failing to define the invariant for `lo`, `hi`, and `mid`

### Hash Tables

Hashing is often the right answer when you need:

- frequency counts
- duplicate detection
- memo of prior positions
- constant-time membership

Common trap:

- using hashing where sorted order is required later anyway

### Sorting

Sorting often unlocks:

- interval merges
- sweep-line reasoning
- duplicate collapse
- pairing by order

Common trap:

- treating `O(n log n)` as too expensive when it is the cleanest solution

### Recursion

Use recursion when the natural state is:

- choose / explore / undo
- left and right subtree decomposition
- generate all valid configurations

Common trap:

- writing recursion without defining the subproblem contract

### Dynamic Programming

Use DP when you see:

- overlapping subproblems
- optimal substructure
- "count the ways"
- edit distance or subsequences
- grid traversal with local transitions

Ask these questions:

- what is the state?
- what is the recurrence?
- what are the base cases?
- do I need the full table?

Common trap:

- jumping to DP before checking whether greedy or graph traversal is enough

### Greedy And Invariants

Greedy solutions usually become visible when you can explain:

- what local choice is always safe
- what invariant remains true after each step

Common trap:

- claiming a greedy choice is optimal without a proof sketch

### Graphs

When the input is not literally a graph but has transitions between states, it
may still be a graph problem.

Core tools:

- BFS for shortest path in unweighted graphs
- DFS for reachability, cycle checks, and components
- topological reasoning for dependency order

Common trap:

- forgetting to mark visited state at the correct time

## C++ Implementation Checklist

This is the short list to review before and after coding.

- Prefer `const` references for read-only large objects.
- Pass small scalar values by value.
- Watch signed vs unsigned mismatches with indices.
- Guard against integer overflow in products and midpoint calculations.
- Be explicit about ownership if pointers appear.
- Prefer STL containers over manual memory management.
- Know iterator invalidation rules for the container you picked.
- Reserve capacity when the final size is predictable.
- Keep helper functions narrow and testable.
- If recursion depth may explode, mention the iterative fallback.

## Common Failure Modes

These are the mistakes to eliminate.

- coding before stating the approach
- optimizing before producing a correct baseline
- ignoring edge cases until the end
- using the wrong data structure because it feels familiar
- giving complexity for the final code only, not for the baseline
- patching bugs ad hoc instead of re-checking the invariant
- finishing the code but not dry-running it

## Practice Cadence

Use a repeatable rhythm instead of random grinding.

### 45-minute session

- 5 minutes: restate and clarify
- 10 minutes: baseline and optimized approach
- 20 minutes: implement
- 10 minutes: dry-run and postmortem

### 90-minute session

- solve one medium problem end to end
- review one previously missed problem from memory
- write down one reusable lesson

### Weekly cadence

- 3 sessions on Tier 1 chapters
- 1 session on recursion / DP / graphs
- 1 session on C++ language and design discussion

## Postmortem Template

After every problem, record:

- chapter and problem name
- pattern used
- first wrong instinct
- final invariant or insight
- time complexity
- space complexity
- C++ bug or syntax issue encountered
- whether you could re-solve it from scratch the next day

If you cannot explain the key invariant in one sentence, you probably memorized
the solution instead of learning it.

## How To Use This With The Other Notes

Use the notes in this order:

1. `epi-cpp-study-guide.md`
2. this playbook
3. `cpp-interview-prep-plan.md`
4. `cpp-patterns-crosswalk.md`

That sequence gives you:

- what to study
- how to solve problems
- how to discuss your own codebase
- how to talk about modern C++ usage concretely
