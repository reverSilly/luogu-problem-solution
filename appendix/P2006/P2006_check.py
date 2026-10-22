#!/usr/bin/env python3
# P2006 对拍：随机技能表，比较公式解法与逐次施放模拟暴力
import random
import subprocess
import sys

STD = "/tmp/P2006"
BRUTE = "/tmp/P2006_brute"


def run(prog, data):
    r = subprocess.run([prog], input=data, capture_output=True, text=True, timeout=30)
    return r.stdout.strip()


def main():
    random.seed(20260922)
    cases = 0
    for it in range(400):
        m = random.randint(1, 8)
        k = random.choice([0, random.randint(1, 30000)])
        n = random.choice([1, random.randint(1, 30000)])
        skills = []
        for _ in range(m):
            a = random.choice([0, random.randint(0, 200), random.randint(0, 2147483647)])
            b = random.choice([0, random.randint(0, 100), random.randint(0, 2147483647)])
            skills.append((a, b))
        data = f"{k} {m} {n}\n" + "\n".join(f"{a} {b}" for a, b in skills) + "\n"
        x = run(STD, data)
        y = run(BRUTE, data)
        cases += 1
        if x != y:
            print("MISMATCH")
            print(data)
            print("std:", x, "brute:", y)
            return 1
    print(f"all {cases} cases match")
    return 0


if __name__ == "__main__":
    sys.exit(main())
