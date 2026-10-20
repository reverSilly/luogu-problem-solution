import subprocess, random, time

SD = "/tmp/P1518"
BF = "/tmp/P1518_brute"

SAMPLE = [
    "*...*.....",
    "......*...",
    "...*...*..",
    "..........",
    "...*.F....",
    "*.....*...",
    "...*......",
    "..C......*",
    "...*.*....",
    ".*.*......",
]


def compile_all():
    subprocess.run(["g++", "-O2", "-std=c++17", "P1518.cpp", "-o", SD], check=True)
    subprocess.run(["g++", "-O2", "-std=c++17", "P1518_brute.cpp", "-o", BF], check=True)


def run(bin_name, inp):
    p = subprocess.run([bin_name], input=inp, text=True, capture_output=True, check=True)
    return p.stdout.strip()


def gen_map(rng, wallp):
    grid = [['.'] * 10 for _ in range(10)]
    cells = [(i, j) for i in range(10) for j in range(10)]
    f, c = rng.sample(cells, 2)
    for (i, j) in cells:
        if (i, j) != f and (i, j) != c and rng.random() < wallp:
            grid[i][j] = '*'
    grid[f[0]][f[1]] = 'F'
    grid[c[0]][c[1]] = 'C'
    return "\n".join("".join(row) for row in grid) + "\n"


def main():
    compile_all()
    rng = random.Random(20261018)

    # 1. 官方样例
    o = run(SD, "\n".join(SAMPLE) + "\n")
    ob = run(BF, "\n".join(SAMPLE) + "\n")
    print(f"sample expects 49 -> std={o} brute={ob}")
    assert o == "49" and ob == "49", f"sample std={o} brute={ob}"

    # 2. 随机对拍
    cases = 0
    for tc in range(400):
        wallp = rng.choice([0.0, 0.1, 0.2, 0.35, 0.5])
        inp = gen_map(rng, wallp)
        o1 = run(SD, inp)
        o2 = run(BF, inp)
        assert o1 == o2, f"Mismatch:\n{inp}\nstd={o1} brute={o2}"
        cases += 1

    # 3. 最坏规模计时: 挑一个永不相遇的图(迫使状态遍历到重复)
    worst = None
    for _ in range(2000):
        inp = gen_map(rng, 0.5)
        if run(SD, inp) == "0":
            worst = inp
            break
    assert worst is not None, "no never-meet map found"
    t0 = time.perf_counter()
    res = run(SD, worst)
    dt = time.perf_counter() - t0
    print(f"P1518 check passed! sample + {cases} random cases + never-meet worst case (ans={res}) in {dt:.4f}s")


if __name__ == "__main__":
    main()
