#!/usr/bin/env python3
# P1603 对拍: 标程(排序拼接) vs 暴力(全排列取最小)
# 编译:
#   g++ -O2 -std=c++17 -Wall -o /tmp/P1603 P1603.cpp
#   g++ -O2 -std=c++17 -Wall -o /tmp/P1603_brute P1603_brute.cpp
import random
import subprocess
import sys

STD = "/tmp/P1603"
BRU = "/tmp/P1603_brute"

WORDS = ["zero", "one", "two", "three", "four", "five", "six", "seven", "eight",
         "nine", "ten", "eleven", "twelve", "thirteen", "fourteen", "fifteen",
         "sixteen", "seventeen", "eighteen", "nineteen", "twenty",
         "a", "both", "another", "first", "second", "third"]


def run(path, data):
    r = subprocess.run([path], input=data.encode(), capture_output=True)
    return r.stdout.decode().strip()


def gen(rng):
    ws = [rng.choice(WORDS + ["is", "obama", "hello", "snowden"]) for _ in range(6)]
    if rng.random() < 0.5:                        # 句首大写也合法
        ws[0] = ws[0].capitalize()
    return " ".join(ws) + "."


def main():
    rng = random.Random(20260521)
    cases = ["Obama is a two five zero.",
             "zero zero zero zero zero zero.",
             "two two two two two two.",
             "twenty twenty twenty twenty twenty twenty.",
             "a a a a a a."]
    for _ in range(160):
        cases.append(gen(rng))
    for t, s in enumerate(cases):
        data = s + "\n"
        a, b = run(STD, data), run(BRU, data)
        if a != b:
            print("MISMATCH on test", t, s)
            print("std:", a, "brute:", b)
            return 1
    print(f"{len(cases)} tests OK")
    return 0


if __name__ == "__main__":
    sys.exit(main())
