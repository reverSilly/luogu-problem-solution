#!/usr/bin/env python3
# P2069 对拍：数组模拟标程 vs 链表暴力
import os, random, subprocess

D = os.path.dirname(os.path.abspath(__file__))

def build(src, exe):
    sp, ep = os.path.join(D, src), os.path.join(D, exe)
    if (not os.path.exists(ep)) or os.path.getmtime(ep) < os.path.getmtime(sp):
        subprocess.run(['g++', '-O2', '-std=c++17', sp, '-o', ep], check=True)

def run(exe, inp):
    p = subprocess.run([os.path.join(D, exe)], input=inp, text=True, capture_output=True)
    return p.stdout.strip()

def valid(n, m):
    #题目保证"能够跳到第 m 次"，用与标程相同的模拟检查合法性
    sz = n
    p = 0
    for i in range(1, m + 1):
        d = i * i * i % 5 + 1
        q = p + d
        if q >= sz:
            #跳出最前面：回到最下面重做第 i 跳，落到下标 d
            p = 0
            q = d
        if q >= sz:
            return False
        sz -= 1
        p = q
    return True

def gen(rnd):
    while True:
        n = rnd.randint(3, 40)
        m = rnd.randint(1, n)
        if valid(n, m):
            return "%d %d\n" % (n, m)

def main():
    build('P2069.cpp', 'P2069')
    build('P2069_brute.cpp', 'P2069_brute')
    rnd = random.Random(20690001)
    cases = [gen(rnd) for _ in range(3000)]
    # 穷举所有合法的小规模 (n, m)
    for n in range(3, 13):
        for m in range(1, n + 1):
            if valid(n, m):
                cases.append("%d %d\n" % (n, m))
    # 官方样例
    cases.append("10 4\n")
    bad = 0
    for c in cases:
        a, b = run('P2069', c), run('P2069_brute', c)
        if a != b:
            bad += 1
            print('MISMATCH\n' + c)
            print('sol=', a, 'brute=', b)
            if bad >= 5:
                break
    print('P2069 done, cases=%d mismatches=%d' % (len(cases), bad))

if __name__ == '__main__':
    main()
