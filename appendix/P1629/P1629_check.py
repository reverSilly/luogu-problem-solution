#!/usr/bin/env python3
# 随机对拍：P1629 邮递员送信（1->i 与 i->1 最短路之和）
import random, subprocess, sys

MAIN = "/tmp/P1629"
BRUTE = "/tmp/P1629_brute"

def gen():
    n = random.randint(2, 7)
    edges = []
    # 先造一个环保证强连通
    perm = list(range(n))
    random.shuffle(perm)
    for i in range(n):
        u, v = perm[i], perm[(i + 1) % n]
        edges.append((u, v, random.randint(1, 9)))
    for _ in range(random.randint(0, 8)):
        u = random.randrange(n)
        v = random.randrange(n)
        if u != v:
            edges.append((u, v, random.randint(1, 9)))
    lines = [f"{n} {len(edges)}"] + [f"{u+1} {v+1} {w}" for u, v, w in edges]
    return "\n".join(lines) + "\n"

bad = 0
N = 200
for it in range(N):
    data = gen()
    a = subprocess.run([MAIN], input=data, capture_output=True, text=True).stdout.split()
    b = subprocess.run([BRUTE], input=data, capture_output=True, text=True).stdout.split()
    if a != b:
        bad += 1
        print("MISMATCH", it, data, a, b)
        if bad > 3:
            sys.exit(1)
print("done", N, "cases, mismatches =", bad)
