#!/usr/bin/env python3
# P2028 对拍：小规模用集合划分枚举暴力，比较第二类斯特林数 DP
import random
import subprocess
import sys

STD = "/tmp/P2028"
BRUTE = "/tmp/P2028_brute"


def run(prog, data):
    r = subprocess.run([prog], input=data, capture_output=True, text=True, timeout=30)
    return r.stdout.strip()


def main():
    random.seed(20260922)
    cases = 0
    for it in range(200):
        n = random.randint(1, 10)
        k = random.randint(1, n)
        p = random.choice([1, 2, 3, 7, 100, 999983, 1000000007, random.randint(1, 10 ** 9)])
        data = f"{n} {k} {p}\n"
        x = run(STD, data)
        y = run(BRUTE, data)
        cases += 1
        if x != y:
            print("MISMATCH", data, "std:", x, "brute:", y)
            return 1
    print(f"all {cases} cases match")
    return 0


if __name__ == "__main__":
    sys.exit(main())
