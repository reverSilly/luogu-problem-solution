#!/usr/bin/env python3
# P1598 校验: 标程 vs 本文件内"二维画布式"的独立实现(先铺满星号矩阵再删尾空格)
# 编译: g++ -O2 -std=c++17 -Wall -o /tmp/P1598 P1598.cpp
import random
import subprocess
import sys

STD = "/tmp/P1598"
LET = [chr(ord('A') + i) for i in range(26)]


def reference(lines):
    cnt = {c: 0 for c in LET}
    for line in lines:
        for c in line:
            if c in cnt:
                cnt[c] += 1
    mx = max(cnt.values()) if cnt else 0
    out = []
    for k in range(mx, 0, -1):                 # 画布: 26 列, 每列 2 字符
        canvas = [[' ', ' '] for _ in range(26)]
        for i, c in enumerate(LET):
            if cnt[c] >= k:
                canvas[i][0] = '*'
        row = ''.join(''.join(p) for p in canvas).rstrip()
        out.append(row)
    out.append(' '.join(LET))
    return '\n'.join(out) + '\n'


def main():
    rng = random.Random(20260521)
    cases = [["AB", "AB", "C", ""],
             ["", "", "", ""],
             ["Z" * 100, "Z" * 100, "Z" * 100, "Z" * 100],
             ["A", "B", "C", "D"]]
    for _ in range(120):
        lines = []
        for _ in range(4):
            L = rng.randint(0, 100)
            lines.append(''.join(rng.choice(LET) for _ in range(L)))
        cases.append(lines)
    for idx, lines in enumerate(cases):
        data = "\n".join(lines) + "\n"
        got = subprocess.run([STD], input=data, capture_output=True, text=True).stdout
        exp = reference(lines)
        if got != exp:
            print("MISMATCH on test", idx, repr(lines)[:80])
            print("got:", repr(got[:200]))
            print("exp:", repr(exp[:200]))
            return 1
    print(f"{len(cases)} tests OK")
    return 0


if __name__ == "__main__":
    sys.exit(main())
