#!/usr/bin/env python3
# P2078 对拍：并查集标程 vs BFS 连通块暴力
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
    n = rnd.randint(1, 8)
    m = rnd.randint(1, 8)
    p = rnd.randint(0, n)
    q = rnd.randint(0, m)
    lines = ["%d %d %d %d" % (n, m, p, q)]
    for _ in range(p):
        u = rnd.randint(1, n)
        v = rnd.randint(1, n)
        lines.append("%d %d" % (u, v))
    for _ in range(q):
        u = -rnd.randint(1, m)
        v = -rnd.randint(1, m)
        lines.append("%d %d" % (u, v))
    return '\n'.join(lines) + '\n'

def main():
    build('P2078.cpp', 'P2078')
    build('P2078_brute.cpp', 'P2078_brute')
    rnd = random.Random(20780001)
    cases = [gen(rnd) for _ in range(2000)]
    # 极端构造：没有任何朋友关系 / 两边各自连通 / 自环
    cases.append("3 3 0 0\n")
    cases.append("3 3 2 2\n1 2\n2 3\n-1 -2\n-2 -3\n")
    cases.append("1 1 1 1\n1 1\n-1 -1\n")
    cases.append("4 3 4 2\n1 1\n1 2\n2 3\n1 3\n-1 -2\n-3 -3\n")
    bad = 0
    for c in cases:
        a, b = run('P2078', c), run('P2078_brute', c)
        if a != b:
            bad += 1
            print('MISMATCH\n' + c)
            print('sol=', a, 'brute=', b)
            if bad >= 5:
                break
    print('P2078 done, cases=%d mismatches=%d' % (len(cases), bad))

if __name__ == '__main__':
    main()
