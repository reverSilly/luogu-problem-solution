#!/usr/bin/env python3
# P2142 对拍：检验十进制高精度减法
import random
import subprocess
import sys

STD = "/tmp/P2142"
BRUTE = "/tmp/P2142_brute"


def run(prog, data):
    r = subprocess.run([prog], input=data, capture_output=True, text=True, timeout=30)
    return r.stdout.strip()


def main():
    random.seed(21402)
    cases = 0
    for _ in range(300):
        la = random.randint(1, 40)
        lb = random.randint(1, 40)
        a = random.randint(10 ** (la - 1), 10 ** la - 1) if la > 1 else random.randint(1, 9)
        b = random.randint(10 ** (lb - 1), 10 ** lb - 1) if lb > 1 else random.randint(1, 9)
        data = f"{a}\n{b}\n"
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
