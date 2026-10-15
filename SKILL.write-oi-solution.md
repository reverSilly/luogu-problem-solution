---
name: "write-oi-solution"
description: "Solves competitive programming (OI/ICPC) problems with a student-style discovery process: human thinking, fully-proven chain-of-thought, pattern summary, style-conformant C++ code, and brute-force validation. Invoke when user asks to solve/write a solution for a competitive programming problem (e.g., Luogu P/CF/ABC), especially when they want chain-of-thought or 'think like a human' writeups."
---

# Write OI Solution

This skill produces a complete solution writeup for a competitive programming problem, structured as a *discovery narrative* (how a student with basic math/programming skills would find the solution), not a polished textbook proof.

## Output Structure (always 5 parts)

```
1. Re-read the problem                      - rewrite the problem in a simple version.
2. Chain-of-thought                         - the discovery narrative, complete + concise, WITH FULL PROOF for every lemma
3. Pattern summary                          - short reusable labels, in ACTUAL thinking order
4. The real solution                        - what the code does, WITH a notation glossary
5. Example code                             - C++, following the user's coding style guide
```

Sections may overlap; that is fine.

## Phase 0 solve
Solve the problem yourself first, before organizing any output.

## Phase 1 output

### Phase A — Problem introduction

- Read the problem file completely before thinking.
- Restate constraints ($n \le ?$) and subtasks — they hint at intended complexity.

### Phase B — Solve Like a Human (the discovery order)

Check your chain-of-thought when you were solving the problem. Re-organize your chain-of-thought in a way that is natural to human. Suppose you are a student learning oi, with only basic math/programming knowledge. Preserve the actual thinking order. The following are some steps i use(just for your reference, not limited).

1. **explore its property** - precisely characterize what the special mechanic does. This always comes first.
2. **consider easier version / consider subtask** - strip the special mechanic away, solve the stripped version, then add the increment back. see if the easier version provide some insights.
3. **classic problem solution** - identify the classic technique the insight grows into (e.g. use seg-tree to maintain range sum, ...).
4. **reduce to easier problem** - the structure found is equavalent to something simpler.
5. **Discuss by category** - the problem can use different tecniques to solve under different constraints***

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

- When identical steps are used  multiple times, list them as SEPARATE pattern entries — never merge them under one label.
- Each label gets one line: label — what it meant here.

### Phase D — Real Solution (Part 4)

Before describing the algorithm steps, define EVERY symbol used  For each array/state: what it stores, how it is initialized, and a one-line mnemonic (e.g., "H=horizontal segment, V=vertical; P/K=from which endpoint").

Then list the algorithm steps as numbered items referencing those symbols, ending with complexity and validation status.

This should be a brief summary of your code.

### Phase E — C++ Code (Part 5)

Follow the user's coding style guide if one is provided. Default Style conventions when no guide is given:

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
- `cin.tie(nullptr); cout.tie(nullptr) ios::sync_with_stdio(false);` in that order; `'\n'` never `endl`.
- 0-based index internally, convert on input (`--u`), restore on output (`i+1`).
- Short telegraphic names: `dis`, `prv`, `mv`; functions lowercase: `bfs`, `dfs`, `pathp`.
- Sparse inline comments only for key invariants and pitfalls.
- `goto label; ... label:;` allowed for breaking nested loops.
- No space around operators. Ident using Tabs.

**Porting pitfall (must remember):** global `int` arrays default to **0**, not INF. When migrating logic from `vector`  to global arrays, every "unreachable/undefined" array MUST be pre-filled with `fill_n(&arr[0][0], mxn*mxn, INF)` — otherwise 0 is silently treated as a reachable 0-cost state.


Report validation results in the writeup (e.g., "3893 random cases pass; n=1000 runs in 0.10s").

## Language

Respond in the user's language. Code comments follow the same language rule.

## Workflow Summary

```
read problem → translate mechanic to plain language → strip mechanic (easier version)
→ apply classic technique → reduce structure → speed up (2nd classic technique)
→ prove every lemma → write C-o-T in actual order → extract patterns (separate labels)
→ define all notation → write style-conformant C++ → brute-force + step-walk validation
→ report
```
