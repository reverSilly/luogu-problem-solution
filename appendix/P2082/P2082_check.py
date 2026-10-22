#!/usr/bin/env python3
# P2082 对拍：排序合并标程 vs 逐点标记暴力
import os, random, subprocess

D = os.path.dirname(os.path.abspath(__file__))

def build(src, exe):
    sp, ep = os.path.join(D, src), os.path.join(D, exe)
    if (not os.path.exists(ep)) or os.path.getmtime(ep) < os.path.getmtime(sp):
        subprocess.run(['g++', '-O2', '-std=c++17', sp, '-o', ep], check=True)

def run(exe, inp):
    p = subprocess.run([os.path.join(D, exe)], input=inp, text=True, capture_output=True)
    return p.stdout.strip()

def gen(rnd):
    # 暴力按坐标开数组，坐标必须控制在 1e5 以内
    n = rnd.randint(1, 30)
    lines = [str(n)]
    for _ in range(n):
        s = rnd.randint(1, 99999)
        t = rnd.randint(s + 1, 100000)
        lines.append(f"{s} {t}")
    return "\n".join(lines) + "\n"

def main():
    build('P2082.cpp', 'P2082')
    build('P2082_brute.cpp', 'P2082_brute')
    rnd = random.Random(20260101)
    cases = [gen(rnd) for _ in range(3000)]
    # 极端构造：完全包含 / 首尾相接 / 互不相交
    cases.append("3\n1 100000\n2 99999\n100000 100000\n")
    cases.append("2\n1 2\n3 4\n")
    cases.append("4\n1 3\n4 6\n7 9\n10 12\n")
    cases.append("1\n1 100000\n")
    bad = 0
    for c in cases:
        a, b = run('P2082', c), run('P2082_brute', c)
        if a != b:
            bad += 1
            print('MISMATCH\n' + c)
            print('sol=', a, 'brute=', b)
            if bad >= 5:
                break
    print('P2082 done, cases=%d mismatches=%d' % (len(cases), bad))

if __name__ == '__main__':
    main()
