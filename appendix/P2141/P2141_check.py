#!/usr/bin/env python3
# P2141 对拍：检验“能被集合中另外两个不同数之和表示”的元素计数
import random
import subprocess
import sys

STD = "/tmp/P2141"
BRUTE = "/tmp/P2141_brute"


def run(prog, data):
    r = subprocess.run([prog], input=data, capture_output=True, text=True, timeout=30)
    return r.stdout.strip()


def main():
    random.seed(21401)
    cases = 0
    for _ in range(400):
        n = random.randint(3, 12)
        # 小值域，制造大量可表示的元素
        hi = random.choice([6, 8, 10, 15, 30, 100])
        pool = list(range(1, hi + 1))
        a = random.sample(pool, n)
        data = f"{n}\n" + " ".join(map(str, a)) + "\n"
        x = run(STD, data)
        y = run(BRUTE, data)
        cases += 1
        if x != y:
            print("MISMATCH data:")
            print(data)
            print("std:", x, "brute:", y)
            return 1
    print(f"all {cases} cases match")
    return 0


if __name__ == "__main__":
    sys.exit(main())
