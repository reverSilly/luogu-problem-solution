#!/usr/bin/env python3
import os, random, string, subprocess, sys, time

here = os.path.dirname(os.path.abspath(__file__))
std = '/tmp/P1825'
brute = '/tmp/P1825_brute'
DX = (1, -1, 0, 0)
DY = (0, 0, 1, -1)

sample = """5 6
###=##
#.W.##
#.####
#.@W##
######
"""
tricky = """5 7
#######
#@A.=.#
#######
#..A..#
#######
"""


def compile_all():
    for src, out in (('P1825.cpp', std), ('P1825_brute.cpp', brute)):
        r = subprocess.run(
            ['g++', '-O2', '-std=c++17', '-Wall', '-Wextra', '-Werror',
             '-o', out, os.path.join(here, src)],
            capture_output=True, text=True)
        if r.returncode != 0 or r.stderr.strip():
            sys.stderr.write(r.stderr)
            sys.exit(1)


def run(prog, inp, timeout=30):
    r = subprocess.run([prog], input=inp, capture_output=True, text=True, timeout=timeout)
    if r.returncode != 0:
        raise RuntimeError(f'{prog} exit {r.returncode}\n{r.stderr[:400]}')
    return r.stdout.strip()


def parse(inp):
    lines = inp.strip().splitlines()
    n, m = map(int, lines[0].split())
    g = [list(lines[i + 1].strip()) for i in range(n)]
    return n, m, g


def oracle(inp):
    n, m, g = parse(inp)
    pos = {}
    sx = sy = None
    for i in range(n):
        for j in range(m):
            ch = g[i][j]
            if ch == '@':
                sx, sy = i, j
            if 'A' <= ch <= 'Z':
                pos.setdefault(ch, []).append((i, j))
    dist = [[-1] * m for _ in range(n)]
    dist[sx][sy] = 0
    q = [(sx, sy)]
    head = 0
    while head < len(q):
        x, y = q[head]
        head += 1
        for k in range(4):
            nx, ny = x + DX[k], y + DY[k]
            if not (0 <= nx < n and 0 <= ny < m) or g[nx][ny] == '#':
                continue
            tx, ty = nx, ny
            ch = g[nx][ny]
            if 'A' <= ch <= 'Z':
                a, b = pos[ch]
                if (nx, ny) == a:
                    tx, ty = b
                else:
                    tx, ty = a
            if dist[tx][ty] != -1:
                continue
            dist[tx][ty] = dist[x][y] + 1
            if g[tx][ty] == '=':
                return str(dist[tx][ty])
            q.append((tx, ty))
    return None


def gen(rnd):
    for _try in range(80):
        n = rnd.randint(3, 9)
        m = rnd.randint(3, 9)
        g = [['#'] * m for _ in range(n)]
        cells = []
        for i in range(1, n - 1):
            for j in range(1, m - 1):
                if rnd.random() < 0.72:
                    g[i][j] = '.'
                    cells.append((i, j))
        if len(cells) < 2:
            continue
        rnd.shuffle(cells)
        si, sj = cells[0]
        ei, ej = cells[1]
        g[si][sj] = '@'
        g[ei][ej] = '='
        rest = cells[2:]
        k = rnd.randint(0, min(3, len(rest) // 2))
        letters = rnd.sample(string.ascii_uppercase, k) if k else []
        for t, ch in enumerate(letters):
            a, b = rest[2 * t], rest[2 * t + 1]
            g[a[0]][a[1]] = ch
            g[b[0]][b[1]] = ch
        lines = [f'{n} {m}'] + [''.join(row) for row in g]
        inp = '\n'.join(lines) + '\n'
        if oracle(inp) is not None:
            return inp
    return sample


def main():
    compile_all()
    tests = 0
    fixed = [(sample, '3'), (tricky, '5')]
    plain = """4 5
#####
#@.=#
#####
#####
"""
    fixed.append((plain, '2'))
    for inp, exp in fixed:
        a, b = run(std, inp), run(brute, inp)
        o = oracle(inp)
        if a != exp or b != exp or o != exp:
            print(f'FAIL fixed exp={exp} std={a} brute={b} oracle={o}\n{inp}')
            return 1
        tests += 1
    rnd = random.Random(1825)
    t0 = time.time()
    for i in range(1, 201):
        inp = gen(rnd)
        a, b = run(std, inp), run(brute, inp)
        o = oracle(inp)
        if a != b or a != o:
            print(f'FAIL round {i}\n{inp}std={a}\nbrut={b}\noracle={o}')
            return 1
        tests += 1
    dt = time.time() - t0
    print(f'[随机+边界] {tests} 组一致,随机耗时 {dt:.2f}s')
    # 最大网格：无传送的空心走廊，答案可手算，只跑标程计时
    N, M = 300, 300
    rows = []
    rows.append('#' * M)
    mid = list('#' + '.' * (M - 2) + '#')
    mid[1] = '@'
    mid[M - 2] = '='
    rows.append(''.join(mid))
    for _ in range(N - 3):
        rows.append('#' + '.' * (M - 2) + '#')
    rows.append('#' * M)
    big = f'{N} {M}\n' + '\n'.join(rows) + '\n'
    t1 = time.time()
    ans = run(std, big, timeout=30)
    tm = time.time() - t1
    # 起点在第 2 行第 2 列，出口在第 2 行倒数第 2 列，同行无障碍，距离 M-3
    if ans != str(M - 3):
        print(f'FAIL max ans={ans} expect {M - 3}')
        return 1
    print(f'[最大规模] n=300 m=300 ans={ans} time={tm:.4f}s')
    tests += 1
    print(f'{tests} tests OK')
    return 0


if __name__ == '__main__':
    sys.exit(main())
