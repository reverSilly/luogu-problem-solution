#!/usr/bin/env python3
# P2026 对拍：用 Python fractions 精确计算期望输出，与 C++ 标程比对
import random
import subprocess
import sys
from fractions import Fraction

STD = "/tmp/P2026"


def fmt_k(k):
    if k.denominator == 1:
        return f"{k.numerator}x"
    return f"{k.numerator}/{k.denominator}*x"


def fmt_b(b):
    if b == 0:
        return ""
    sign = "+" if b > 0 else "-"
    b = abs(b)
    if b.denominator == 1:
        return f"{sign}{b.numerator}"
    return f"{sign}{b.numerator}/{b.denominator}"


def expected(x1, y1, x2, y2):
    k = Fraction(y2 - y1, x2 - x1)
    b = Fraction(y1) - k * x1
    return "y=" + fmt_k(k) + fmt_b(b)


def main():
    random.seed(20260922)
    cases = 0
    for it in range(400):
        while True:
            x1 = random.randint(-50, 50)
            x2 = random.randint(-50, 50)
            if x1 != x2:
                break
        y1 = random.randint(-50, 50)
        y2 = random.randint(-50, 50)
        data = f"{x1} {y1}\n{x2} {y2}\n"
        r = subprocess.run([STD], input=data, capture_output=True, text=True, timeout=20)
        got = r.stdout.strip()
        exp = expected(x1, y1, x2, y2)
        cases += 1
        if got != exp:
            print("MISMATCH")
            print("input:", data.replace("\n", " "))
            print("std:", got)
            print("exp:", exp)
            return 1
    print(f"all {cases} cases match")
    return 0


if __name__ == "__main__":
    sys.exit(main())
