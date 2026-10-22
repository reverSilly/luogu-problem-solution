#!/usr/bin/env python3
# P2021 对拍：小规模用数组模拟暴力，检验 O(n) 环形链表解法
import random
import subprocess
import sys

STD = "/tmp/P2021"
BRUTE = "/tmp/P2021_brute"


def run(prog, data):
    r = subprocess.run([prog], input=data, capture_output=True, text=True, timeout=30)
    return r.stdout.strip()


def main():
    random.seed(20260922)
    cases = 0
    for n in list(range(1, 60)) + [random.randint(1, 600) for _ in range(60)]:
        data = f"{n}\n"
        a = run(STD, data)
        b = run(BRUTE, data)
        cases += 1
        if a != b:
            print("MISMATCH n=", n)
            print("std:", a)
            print("brute:", b)
            return 1
    print(f"all {cases} cases match")
    return 0


if __name__ == "__main__":
    sys.exit(main())
