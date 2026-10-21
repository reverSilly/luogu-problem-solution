#!/usr/bin/env python3
# 随机对拍：P1624 单词缩写方案计数
import random, subprocess, sys

MAIN = "/tmp/P1624"
BRUTE = "/tmp/P1624_brute"

def gen():
    ign = random.sample(["a", "b", "ab", "cc", "abc"], random.randint(1, 3))
    out = [str(len(ign))] + ign
    for _ in range(random.randint(1, 4)):
        words = []
        for _ in range(random.randint(1, 4)):
            words.append("".join(random.choice("abc") for _ in range(random.randint(1, 3))))
        ab = "".join(random.choice("ABC") for _ in range(random.randint(1, 3)))
        out.append(ab + " " + " ".join(words))
    out.append("LASTCASE")
    return "\n".join(out) + "\n"

def run(prog, data):
    return subprocess.run([prog], input=data, capture_output=True, text=True).stdout

bad = 0
N = 300
for it in range(N):
    data = gen()
    a = run(MAIN, data)
    b = run(BRUTE, data)
    if a != b:
        bad += 1
        print("MISMATCH", it)
        print(data)
        print("main :", a)
        print("brute:", b)
        if bad > 2:
            sys.exit(1)
print("done", N, "cases, mismatches =", bad)
