#!/usr/bin/env python3
# P2128 对拍：检验平面图最大权团（团大小至多 4）
# 造平面图：圆周顺序编号的一段上取互不相交的弦（保证平面），
# 另加若干独立的 K4 组件（K4 本身是平面图），用于覆盖 4 元团分支
import random
import subprocess
import sys

STD = "/tmp/P2128"
BRUTE = "/tmp/P2128_brute"


def run(prog, data):
    r = subprocess.run([prog], input=data, capture_output=True, text=True, timeout=30)
    return r.stdout.strip()


def cross(e1, e2):
    a, b = e1
    c, d = e2
    if a > b:
        a, b = b, a
    if c > d:
        c, d = d, c
    if a == c or b == d:
        return False
    return a < c < b < d or c < a < d < b


def main():
    random.seed(21281)
    cases = 0
    for _ in range(300):
        n = random.randint(4, 12)
        nk = random.randint(0, 2)
        nk = min(nk, n // 4)
        n1 = n - 4 * nk
        cand = [(i, j) for i in range(n1) for j in range(i + 1, n1)]
        random.shuffle(cand)
        chosen = []
        for e in cand:
            if all(not cross(e, f) for f in chosen):
                chosen.append(e)
        # K4 组件
        for t in range(nk):
            base = n1 + 4 * t
            for i in range(4):
                for j in range(i + 1, 4):
                    chosen.append((base + i, base + j))
        vals = [random.randint(1, 6000) for _ in range(n)]
        data = f"{n} {len(chosen)}\n" + "".join(f"{v}\n" for v in vals)
        data += "".join(f"{a + 1} {b + 1}\n" for a, b in chosen)
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
