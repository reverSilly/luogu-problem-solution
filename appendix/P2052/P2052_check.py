#!/usr/bin/env python3
# P2052 对拍：子树大小标程 vs 逐边删边 BFS 暴力
import os, random, subprocess

D = os.path.dirname(os.path.abspath(__file__))

def build(src, exe):
    sp, ep = os.path.join(D, src), os.path.join(D, exe)
    if (not os.path.exists(ep)) or os.path.getmtime(ep) < os.path.getmtime(sp):
        subprocess.run(['g++', '-O2', '-std=c++17', sp, '-o', ep], check=True)

def run(exe, inp):
    p = subprocess.run([os.path.join(D, exe)], input=inp, text=True, capture_output=True)
    return p.stdout.strip()

def random_tree(n):
    edges = []
    for v in range(2, n + 1):
        u = random.randint(1, v - 1)
        edges.append((u, v, random.randint(0, 20)))
    random.shuffle(edges)
    return edges

def main():
    build('P2052.cpp', 'P2052')
    build('P2052_brute.cpp', 'P2052_brute')
    cases = []
    for _ in range(200):
        n = random.randint(2, 30)
        edges = random_tree(n)
        cases.append('%d\n%s' % (n, '\n'.join('%d %d %d' % e for e in edges)))
    # 极端构造：链、纯星形
    cases.append('2\n1 2 5')
    cases.append('4\n1 2 1\n2 3 1\n3 4 1')
    cases.append('5\n1 2 3\n1 3 3\n1 4 3\n1 5 3')
    bad = 0
    for c in cases:
        a, b = run('P2052', c), run('P2052_brute', c)
        if a != b:
            bad += 1
            print('MISMATCH\n' + c, 'sol=', a, 'brute=', b)
    print('P2052 done, cases=%d mismatches=%d' % (len(cases), bad))

if __name__ == '__main__':
    main()
