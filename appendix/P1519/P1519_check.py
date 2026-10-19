import subprocess, random, time

SD = "/tmp/P1519"
BF = "/tmp/P1519_brute"

# 题面给出的 W=5,H=3 样例(注意第 6 行行尾有两个空格)
SAMPLE = [
    "+-+-+-+-+-+",
    "|         |",
    "+-+ +-+ + +",
    "|     | | |",
    "+ +-+-+ + +",
    "| |     |  ",
    "+-+ +-+-+-+",
]


def compile_all():
    subprocess.run(["g++", "-O2", "-std=c++17", "P1519.cpp", "-o", SD], check=True)
    subprocess.run(["g++", "-O2", "-std=c++17", "P1519_brute.cpp", "-o", BF], check=True)


def run(bin_name, inp):
    p = subprocess.run([bin_name], input=inp, text=True, capture_output=True, check=True)
    return p.stdout.strip()


def gen_maze(W, H, rng):
    """随机生成完美迷宫(连通), 再打两个出口."""
    R, C = 2 * H + 1, 2 * W + 1
    g = [[' '] * C for _ in range(R)]
    for r in range(R):
        for c in range(C):
            if r % 2 == 0 and c % 2 == 0:
                g[r][c] = '+'
            elif r % 2 == 0:
                g[r][c] = '-'
            elif c % 2 == 0:
                g[r][c] = '|'
            else:
                g[r][c] = ' '

    visited = [[False] * W for _ in range(H)]
    stack = [(0, 0)]
    visited[0][0] = True
    while stack:
        r, c = stack[-1]
        nbrs = []
        for dr, dc in ((-1, 0), (1, 0), (0, -1), (0, 1)):
            nr, nc = r + dr, c + dc
            if 0 <= nr < H and 0 <= nc < W and not visited[nr][nc]:
                nbrs.append((nr, nc, dr, dc))
        if not nbrs:
            stack.pop()
            continue
        nr, nc, dr, dc = rng.choice(nbrs)
        rr, cc = 2 * r + 1, 2 * c + 1
        g[rr + dr][cc + dc] = ' '        # 打通两格之间的墙
        visited[nr][nc] = True
        stack.append((nr, nc))

    cand = ([(0, c) for c in range(1, C - 1, 2)] +
            [(R - 1, c) for c in range(1, C - 1, 2)] +
            [(r, 0) for r in range(1, R - 1, 2)] +
            [(r, C - 1) for r in range(1, R - 1, 2)])
    a, b = rng.sample(cand, 2)
    g[a[0]][a[1]] = ' '                  # 两个出口
    g[b[0]][b[1]] = ' '
    return f"{W} {H}\n" + "\n".join("".join(row) for row in g) + "\n"


def main():
    compile_all()
    rng = random.Random(20261018)

    # 1. 题面样例
    inp = "5 3\n" + "\n".join(SAMPLE) + "\n"
    o1 = run(SD, inp)
    o2 = run(BF, inp)
    print(f"sample expects 9 -> std={o1} brute={o2}")
    assert o1 == "9" and o2 == "9", f"sample std={o1} brute={o2}"

    # 2. 随机完美迷宫对拍
    cases = 0
    for tc in range(350):
        W = rng.randint(1, 7)
        H = rng.randint(1, 7)
        inp = gen_maze(W, H, rng)
        a = run(SD, inp)
        b = run(BF, inp)
        assert a == b, f"Mismatch:\n{inp}\nstd={a} brute={b}"
        cases += 1

    # 3. 最坏规模计时 W=38, H=100
    inp = gen_maze(38, 100, rng)
    t0 = time.perf_counter()
    res = run(SD, inp)
    dt = time.perf_counter() - t0
    print(f"P1519 check passed! sample + {cases} random mazes + worst case 38x100 (ans={res}) in {dt:.4f}s")


if __name__ == "__main__":
    main()
