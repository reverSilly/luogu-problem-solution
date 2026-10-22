#!/usr/bin/env python3
# P2072 对拍：贪心+DP 标程 vs 全划分枚举暴力
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
    m = rnd.randint(2, 6)
    k = rnd.randint(1, m - 1)
    n = rnd.randint(1, 12)
    a = [rnd.randint(1, m) for _ in range(n)]
    return "%d %d %d\n%s\n" % (n, m, k, ' '.join(map(str, a)))

def main():
    build('P2072.cpp', 'P2072')
    build('P2072_brute.cpp', 'P2072_brute')
    rnd = random.Random(20720001)
    cases = [gen(rnd) for _ in range(2000)]
    # 极端构造：全是同一种宗教 / 全部宗教各不相同 / k=1
    cases.append("5 3 1\n1 1 1 1 1\n")
    cases.append("5 5 4\n1 2 3 4 5\n")
    cases.append("1 2 1\n2\n")
    cases.append("10 4 3\n1 2 3 4 3 4 3 2 1 2\n")
    bad = 0
    for c in cases:
        a, b = run('P2072', c), run('P2072_brute', c)
        if a != b:
            bad += 1
            print('MISMATCH\n' + c)
            print('sol=', a, 'brute=', b)
            if bad >= 5:
                break
    print('P2072 done, cases=%d mismatches=%d' % (len(cases), bad))

if __name__ == '__main__':
    main()
