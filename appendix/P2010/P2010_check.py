#!/usr/bin/env python3
# P2010 对拍：随机日期区间，比较逐年枚举标程与逐日枚举暴力
import random
import subprocess
import sys

STD = "/tmp/P2010"
BRUTE = "/tmp/P2010_brute"


def run(prog, data):
    r = subprocess.run([prog], input=data, capture_output=True, text=True, timeout=30)
    return r.stdout.strip()


def main():
    random.seed(20260922)
    cases = 0
    for it in range(200):
        y1 = random.randint(1000, 9998)
        y2 = random.randint(y1, min(9999, y1 + random.choice([0, 1, 5, 30, 200])))
        def rand_date(y):
            while True:
                m = random.randint(1, 12)
                md = [31, 29 if (y % 4 == 0 and y % 100 != 0) or y % 400 == 0 else 28,
                      31, 30, 31, 30, 31, 31, 30, 31, 30, 31]
                d = random.randint(1, md[m - 1])
                return y * 10000 + m * 100 + d
        a = rand_date(y1)
        b = rand_date(y2)
        if a > b:
            a, b = b, a
        data = f"{a}\n{b}\n"
        x = run(STD, data)
        y = run(BRUTE, data)
        cases += 1
        if x != y:
            print("MISMATCH", data, x, y)
            return 1
    print(f"all {cases} cases match")
    return 0


if __name__ == "__main__":
    sys.exit(main())
