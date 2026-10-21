#!/usr/bin/env python3
# 随机对拍：P1623 树的最大匹配计数
import random, subprocess, sys

MAIN = "/tmp/P1623"
BRUTE = "/tmp/P1623_brute"

def gen(maxn=9):
    n = random.randint(1, maxn)
    par = [-1] * n
    for i in range(1, n):
        par[i] = random.randint(0, i - 1)
    if random.random() < 0.5:
        # 随机重标号，避免总是 0 为根
        perm = list(range(n))
        random.shuffle(perm)
        par2 = [-1] * n
        for i in range(1, n):
            par2[perm[i]] = perm[par[i]]
        par = par2
    ch = [[] for _ in range(n)]
    for i in range(n):
        if par[i] >= 0:
            ch[par[i]].append(i)
    order = list(range(n))
    random.shuffle(order)
    lines = [str(n)]
    for u in order:
        random.shuffle(ch[u])
        lines.append(" ".join([str(u + 1), str(len(ch[u]))] + [str(v + 1) for v in ch[u]]))
    return "\n".join(lines) + "\n"

def run(prog, data):
    return subprocess.run([prog], input=data, capture_output=True, text=True).stdout.split()

bad = 0
N = 200
for it in range(N):
    data = gen()
    a = run(MAIN, data)
    b = run(BRUTE, data)
    if a != b:
        bad += 1
        print("MISMATCH", it, a, b)
        print(data)
        if bad > 3:
            sys.exit(1)
print("done", N, "cases, mismatches =", bad)
