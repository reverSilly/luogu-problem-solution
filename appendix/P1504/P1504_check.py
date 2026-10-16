#!/usr/bin/env python3
"""P1504 积木城堡:暴举 vs 0-1 背包 随机数据对拍 + 样例回归 + 最坏规模计时。

用法:python3 P1504_check.py [循环轮数]
"""
import random, subprocess, sys, time, os

here = os.path.dirname(os.path.abspath(__file__))
std = 'P1504'
brute = 'P1504_brute'
sample = (['2', '2 1 -1', '3 2 1 -1'], '3')


def run(prog, inp):
    r = subprocess.run([os.path.join(here, prog)], input=inp,
                       capture_output=True, text=True, timeout=60)
    if r.returncode:
        raise RuntimeError(f'{prog} 返回 {r.returncode}: {r.stderr[:400]}')
    return r.stdout.strip()


def gen(rnd):
    """随机一座测试用例:n 小、每座城堡块数小,便于暴举在 2^m 内跑完。"""
    n = rnd.randint(1, 6)
    lines = [str(n)]
    for _ in range(n):
        m = rnd.randint(0, 14)                      # 2^14 * 6 座仍在 ms 级
        vals = sorted((rnd.randint(0, 20) for _ in range(m)), reverse=True)
        lines.append(' '.join(map(str, vals)) + ' -1')
    return '\n'.join(lines) + '\n'


def main():
    rounds = int(sys.argv[1]) if len(sys.argv) > 1 else 3000
    subprocess.run(['g++', '-O2', '-std=c++17', '-o', os.path.join(here, std),
                    os.path.join(here, std + '.cpp')], check=True)
    subprocess.run(['g++', '-O2', '-std=c++17', '-o', os.path.join(here, brute),
                    os.path.join(here, brute + '.cpp')], check=True)

    got = run('./' + std, '\n'.join(sample[0]) + '\n')
    print(f'[样例回归] 期望 {sample[1]},实得 {got} -> {"PASS" if got == sample[1] else "FAIL"}')
    assert got == sample[1], '样例不过,后续对拍无意义'

    rnd = random.Random(20241016)
    t0 = time.time()
    for i in range(1, rounds + 1):
        inp = gen(rnd)
        a, b = run('./' + std, inp), run('./' + brute, inp)
        if a != b:
            print(f'[对拍失败] 第 {i} 轮\n{inp}std={a} brute={b}')
            sys.exit(1)
    print(f'[随机对拍] {rounds} 组一致,耗时 {time.time() - t0:.2f}s')

    worst = ['100'] + [' '.join(['100'] * 100) + ' -1'] * 100   # n=100, m=100, 棱长=100
    worst = '\n'.join(worst) + '\n'
    t1 = time.time()
    ans = run('./' + std, worst)
    print(f'[最坏规模] n=100,m=100 -> {ans},耗时 {time.time() - t1:.3f}s')

    edge = [
        ('1\n-1\n', '单座空城堡'),
        ('3\n5 -1\n5 -1\n5 -1\n', '三座全同'),
        ('2\n8 -1\n5 1 1 -1\n', '交集只剩 0'),
        ('2\n3 3 3 -1\n4 4 -1\n', '各自能凑但无公共正数'),
    ]
    for inp, tag in edge:
        print(f'[边界] {tag}: std={run("./" + std, inp)} brute={run("./" + brute, inp)}')


if __name__ == '__main__':
    main()
