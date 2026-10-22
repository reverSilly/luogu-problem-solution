#!/usr/bin/env python3
# P2084 交叉校验：标程输出 vs Python 独立实现的期望字符串
import os, random, subprocess

D = os.path.dirname(os.path.abspath(__file__))

def build(src, exe):
    sp, ep = os.path.join(D, src), os.path.join(D, exe)
    if (not os.path.exists(ep)) or os.path.getmtime(ep) < os.path.getmtime(sp):
        subprocess.run(['g++', '-O2', '-std=c++17', sp, '-o', ep], check=True)

def run(exe, inp):
    p = subprocess.run([os.path.join(D, exe)], input=inp, text=True, capture_output=True)
    return p.stdout.strip()

def expect(m, s):
    parts = []
    L = len(s)
    for i, ch in enumerate(s):
        if ch == '0':
            continue
        parts.append('%s*%d^%d' % (ch, m, L - 1 - i))
    return '+'.join(parts)

def main():
    build('P2084.cpp', 'P2084')
    rnd = random.Random(20260101)
    cases = []
    for _ in range(2000):
        m = rnd.randint(2, 9)
        L = rnd.randint(1, 30)
        s = ''.join(rnd.choice('0123456789') for _ in range(L))
        if s[0] == '0':
            s = rnd.choice('123456789') + s[1:]
        cases.append('%d %s' % (m, s))
    # 极端构造：全零系数位、超长数字、单数字
    cases.append('2 1000000')
    cases.append('9 9')
    cases.append('3 ' + '1' + '0' * 999)
    cases.append('2 ' + '1' * 1000)
    bad = 0
    for c in cases:
        m, s = c.split()
        a, b = run('P2084', c), expect(int(m), s)
        if a != b:
            bad += 1
            print('MISMATCH in=%s\nsol=%s\nexp=%s' % (c, a, b))
            if bad >= 5:
                break
    print('P2084 done, cases=%d mismatches=%d' % (len(cases), bad))

if __name__ == '__main__':
    main()
