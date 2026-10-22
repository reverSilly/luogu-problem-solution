---
name: "write-oi-solution"
description: "Solves competitive programming (OI/ICPC) problems with a student-style discovery process: human thinking, fully-proven chain-of-thought, pattern summary, and concise style-conformant C++ code. Invoke when user asks to solve/write a solution for a competitive programming problem (e.g., Luogu P/CF/ABC), especially when they want chain-of-thought or 'think like a human' writeups."
---

# Write OI Solution

This skill produces a complete solution writeup for a competitive programming problem, structured as a *discovery narrative* (how a student with basic math/programming skills would find the solution), not a polished textbook proof.

## Core Philosophy (Read First — Avoid Unnecessary Work)

1. **The Chain-of-Thought is the soul of the solution; code isn't that important.**
   - Spend 80%+ of your effort on **Part 2 (Chain-of-thought)**: motivating each step, showing why naive ideas fail, and writing rigorous, complete proofs for every lemma.
   - **Part 5 (Example code)** is just a concrete translation of the proven idea. Keep it to a single, clean reference implementation. Do **not** paste a second "brute-force baseline" code block into the writeup, and do **not** clutter the writeup with stress-test logs or micro-benchmarking tables.
2. **Do NOT write brute-force or stress-test scripts for simple problems.**
   - If a problem is straightforward (simulation, basic greedy/DP, direct math, or low difficulty) or your mathematical proof already closes the gap, **do not** write `brute.cpp` or `check.py`, and do not create an `appendix/P{id}/` directory just to check a box.
   - Only write auxiliary trace/check scripts when a problem is genuinely complex or subtle enough that a hypothesis or tricky corner case needs empirical verification.
3. **NEVER waste effort hunting for original test data or official samples.**
   - Local problem statements in `statement/` may omit sample inputs/outputs, and original judge test data (`.in`/`.out`) is not stored in the repo.
   - **Never** search the web, scrape OJ pages, or desperately hunt for original test data or missing official samples. Whenever you need an example to trace or test, construct a small, valid testcase yourself directly from the problem rules.

## Output Structure (always 5 parts)

```
1. Re-read the problem                      - rewrite the problem in a simple version.
2. Chain-of-thought (CORE)                  - the discovery narrative, complete + concise, WITH FULL PROOF for every lemma
3. Pattern summary                          - short reusable labels, in ACTUAL thinking order
4. The real solution                        - what the algorithm does, WITH a notation glossary
5. Example code                             - concise C++, following the user's coding style guide
```

Sections may overlap; that is fine.

## Phase 0 solve
Solve the problem yourself first, before organizing any output. Focus on getting the reasoning and mathematical proof rock-solid.

## Phase 1 output

### Phase A — Problem introduction

- Read the problem file completely before thinking.
- Restate constraints ($n \le ?$) and subtasks — they hint at intended complexity.

### Phase B — Solve Like a Human (the discovery order — MOST IMPORTANT)

Check your chain-of-thought when you were solving the problem. Re-organize your chain-of-thought in a way that is natural to human. Suppose you are a student learning oi, with only basic math/programming knowledge. Preserve the actual thinking order. The following are some steps i use(just for your reference, not limited).

1. **explore its property** - precisely characterize what the special mechanic does. This always comes first.
2. **consider easier version / consider subtask** - strip the special mechanic away, solve the stripped version, then add the increment back. see if the easier version provide some insights.
3. **classic problem solution** - identify the classic technique the insight grows into (e.g. use seg-tree to maintain range sum, ...).
4. **reduce to easier problem** - the structure found is equavalent to something simpler.
5. **Discuss by category** - the problem can use different tecniques to solve under different constraints

Key discipline when writing:
- Explain *why the next idea comes* ("greedy got stuck → dissect why → generalize the failure pattern").
- Use proof by contradiction / exchange arguments to find and verify failure conditions; then prove the converse ("if the bad pattern is absent, greedy never fails") before claiming a necessary-and-sufficient condition.
- **Every lemma gets a full proof.** A proof sketch like "obvious" is not acceptable.

### Phase C — Pattern Summary (Part 3)

Condense the chain-of-thought into short labels, e.g.(just for your reference, not limited):

```
- explore its property — <one line>
- consider easier version — <one line>
- classic problem solution — <one line: technique + what it gives>
- reduce to easier problem — <one line>
- classic problem solution — <one line: the second, distinct technique>
- defer reconstruction — <one line>
```

Rules:
- Keep the labels in the actual thinking order.
- One label per distinct technique.
- When identical patterns are used multiple times, list them as SEPARATE pattern entries — never merge them under one label.
- Each label gets one line: label — what it meant here.

### Phase D — Real Solution (Part 4)

Before describing the algorithm steps, define EVERY symbol used. For each array/state: what it stores, how it is initialized, and a one-line mnemonic (e.g., "H=horizontal segment, V=vertical; P/K=from which endpoint").

Then list the algorithm steps as numbered items referencing those symbols, ending with time and space complexity.

This should be a brief summary of your algorithm. Do not pad this section with mechanical validation tables or runtime benchmarks.

### Phase E — C++ Code (Part 5)

Code is secondary to the chain-of-thought: include a single, clean C++ implementation that follows the user's coding style guide . Default style conventions when no guide is given:

```cpp
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{1005};              // size = constraint + 5 relaxation
constexpr int inf{1145141919};        // 114514-family sentinel (or 1e9)

int n,m,a[mxn],s[mxn],f[mxn];
// global static arrays, never vectors-as-state

bool isPass(int r,int c){...}         // lowercase telegraphic functions
void bfs(int u)
{
    ...
}

int main()
{
    cin.tie(nullptr);                 // this order, always
    cout.tie(nullptr);
    ios::sync_with_stdio(false);
    // read → solve → output
}
```

Hard rules:
- `#include<bits/stdc++.h>` only; no individual headers.
- Global static arrays; `constexpr int mxn{...}` with +5 slack.
- Sentinel values from the 114514 family (`1145141919`) or `1e9`.
- `cin.tie(nullptr); cout.tie(nullptr); ios::sync_with_stdio(false);` in that order; `'\n'` never `endl`.
- 0-based index internally, convert on input (`--u`), restore on output (`i+1`).
- Short telegraphic names: `dis`, `prv`, `mv`; functions lowercase: `bfs`, `dfs`, `pathp`.
- Sparse inline comments only for key invariants and pitfalls.
- `goto label; ... label:;` allowed for breaking nested loops.
- No space around operators. Indent using Tabs.

**Porting pitfall (must remember):** global `int` arrays default to **0**, not INF. When migrating logic from `vector` to global arrays, every "unreachable/undefined" array MUST be pre-filled with `fill_n(&arr[0][0], mxn*mxn, INF)` — otherwise 0 is silently treated as a reachable 0-cost state.

## Language

Respond in the user's language. Code comments follow the same language rule.
