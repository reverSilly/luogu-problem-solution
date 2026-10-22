#!/usr/bin/env python3
# P2077 对拍：取模相位标程 vs 逐分钟模拟暴力
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
    n = rnd.randint(1, 6)
    m = rnd.randint(1, 10)
    a = [rnd.randint(1, 10) for _ in range(n - 1)]
    r = [rnd.randint(1, 5) for _ in range(n)]
    g = [rnd.randint(1, 5) for _ in range(n)]
    lines = ["%d %d" % (n, m)]
    if n > 1:
        lines.append(' '.join(map(str, a)))
    lines.append(' '.join(map(str, r)))
    lines.append(' '.join(map(str, g)))
    return '\n'.join(lines) + '\n'

def main():
    build('P2077.cpp', 'P2077')
    build('P2077_brute.cpp', 'P2077_brute')
    rnd = random.Random(20770001)
    cases = [gen(rnd) for _ in range(2000)]
    # 极端构造：n=1 / 红灯极长 / 绿灯极长
    cases.append("1 5\n1\n1\n")
    cases.append("1 5\n5\n5\n")
    cases.append("3 3\n1 1\n5 5\n5 5\n1 1\n")
    cases.append("5 3\n2 4 3 2\n3 4 3 4 3\n2 3 2 3 1\n")
    bad = 0
    for c in cases:
        a, b = run('P2077', c), run('P2077_brute', c)
        if a != b:
            bad += 1
            print('MISMATCH\n' + c)
            print('sol=', a, 'brute=', b)
            if bad >= 5:
                break
    print('P2077 done, cases=%d mismatches=%d' % (len(cases), bad))

if __name__ == '__main__':
    main()
