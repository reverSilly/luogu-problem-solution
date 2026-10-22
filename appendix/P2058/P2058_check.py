#!/usr/bin/env python3
# P2058 对拍：滑动窗口标程 vs set 暴力
import os, random, subprocess

D = os.path.dirname(os.path.abspath(__file__))

def build(src, exe):
    sp, ep = os.path.join(D, src), os.path.join(D, exe)
    if (not os.path.exists(ep)) or os.path.getmtime(ep) < os.path.getmtime(sp):
        subprocess.run(['g++', '-O2', '-std=c++17', sp, '-o', ep], check=True)

def run(exe, inp):
    p = subprocess.run([os.path.join(D, exe)], input=inp, text=True, capture_output=True)
    return p.stdout.strip()

def main():
    build('P2058.cpp', 'P2058')
    build('P2058_brute.cpp', 'P2058_brute')
    cases = []
    for _ in range(300):
        n = random.randint(1, 8)
        # 时间随机且非递减
        t = 0
        lines = []
        for _ in range(n):
            t += random.choice([0, 1, random.randint(1, 100000)])
            k = random.randint(0, 4)
            xs = [random.randint(1, 6) for _ in range(k)]
            lines.append('%d %d%s' % (t, k, (' ' + ' '.join(map(str, xs))) if k else ''))
        cases.append('%d\n%s' % (n, '\n'.join(lines)))
    # 极端构造：恰好卡在 86400 边界
    cases.append('3\n1 2 1 1\n86401 1 2\n86402 1 3')
    cases.append('2\n86400 1 1\n86401 1 2')
    cases.append('1\n1 0')
    bad = 0
    for c in cases:
        a, b = run('P2058', c), run('P2058_brute', c)
        if a != b:
            bad += 1
            print('MISMATCH\n' + c, 'sol=', a, 'brute=', b)
    print('P2058 done, cases=%d mismatches=%d' % (len(cases), bad))

if __name__ == '__main__':
    main()
