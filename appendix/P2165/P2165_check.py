#!/usr/bin/env python3
# P2165 对拍：检验圆内接矩形计数
import random
import subprocess
import sys

STD = "/tmp/P2165"
BRUTE = "/tmp/P2165_brute"


def run(prog, data):
    r = subprocess.run([prog], input=data, capture_output=True, text=True, timeout=30)
    return r.stdout.strip()


def main():
    random.seed(21651)
    cases = 0
    for _ in range(300):
        n = random.randint(4, 10)
        # 偏向偶数总长，制造更多直径
        lens = [random.choice([1, 1, 2, 2, 3, 4]) for _ in range(n)]
        data = f"{n}\n" + "".join(f"{v}\n" for v in lens)
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
