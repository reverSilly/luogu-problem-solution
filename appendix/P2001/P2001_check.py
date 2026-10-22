#!/usr/bin/env python3
# P2001 对拍：随机小数据，比较贪心标程与 DP 暴力
import random
import subprocess
import sys

STD = "/tmp/P2001"
BRUTE = "/tmp/P2001_brute"


def run(prog, data):
    r = subprocess.run([prog], input=data, capture_output=True, text=True, timeout=20)
    return r.stdout.strip()


def main():
    random.seed(20260922)
    cases = 0
    for it in range(400):
        n = random.randint(1, 12)
        # 随机面值集合，可能不含 1
        vals = sorted(random.sample(range(1, 60), n))
        m = random.choice([random.randint(1, 200), random.randint(1, 2000)])
        data = f"{n} {m}\n" + " ".join(map(str, vals)) + "\n"
        a = run(STD, data)
        b = run(BRUTE, data)
        cases += 1
        if a != b:
            print("MISMATCH")
            print("input:", data)
            print("std:", a, "brute:", b)
            return 1
    print(f"all {cases} cases match")
    return 0


if __name__ == "__main__":
    sys.exit(main())
