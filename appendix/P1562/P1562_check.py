import subprocess, random, time

def compile_all():
    subprocess.run(["g++", "-O2", "-std=c++17", "P1562.cpp", "-o", "/tmp/P1562"], check=True)
    subprocess.run(["g++", "-O2", "-std=c++17", "P1562_brute.cpp", "-o", "/tmp/P1562_brute"], check=True)

def run(bin_name, inp):
    p = subprocess.run([bin_name], input=inp, text=True, capture_output=True, check=True)
    return p.stdout.strip()

def main():
    compile_all()
    rng = random.Random(20261018)
    # 1. 小规模随机对拍: 位掩码 DFS vs 显式棋盘回溯
    for tc in range(300):
        n = rng.randint(1, 8)
        p = rng.choice([1.0, 0.8, 0.6, 0.4])
        grid = []
        for _ in range(n):
            row = "".join("*" if rng.random() < p else "." for _ in range(n))
            grid.append(row)
        inp = f"{n}\n" + "\n".join(grid) + "\n"
        out_std = run("/tmp/P1562", inp)
        out_bf = run("/tmp/P1562_brute", inp)
        assert out_std == out_bf, f"Mismatch on:\n{inp}\nstd={out_std}, bf={out_bf}"

    # 2. 关键已知值: 全棋盘 8 皇后 -> 92, 全棋盘 9 皇后 -> 352(仅暴力可验)
    for n, exp in [(1, "1"), (2, "0"), (4, "2"), (6, "4"), (8, "92"), (9, "352")]:
        inp = f"{n}\n" + "\n".join(["*" * n for _ in range(n)]) + "\n"
        got = run("/tmp/P1562", inp)
        assert got == exp, f"full board n={n}: got {got} exp {exp}"

    # 3. 全不可放 -> 0
    inp = "3\n...\n...\n...\n"
    assert run("/tmp/P1562", inp) == "0", "all-forbidden should be 0"

    # 4. 最坏规模计时: n=14 全 '*'
    max_inp = "14\n" + "\n".join(["*" * 14 for _ in range(14)]) + "\n"
    t0 = time.perf_counter()
    res = run("/tmp/P1562", max_inp)
    dt = time.perf_counter() - t0
    print(f"PASS P1562! 300 random cases + known values + n=14 worst-case (ans={res}) in {dt:.4f}s")

if __name__ == "__main__":
    main()
