import subprocess, random, time, os, tempfile, shutil

DIR = os.path.dirname(os.path.abspath(__file__))

def compile_all(tmp):
    subprocess.run(["g++", "-O2", "-std=c++17", os.path.join(DIR, "P1535.cpp"),
                    "-o", os.path.join(tmp, "std")], check=True)
    subprocess.run(["g++", "-O2", "-std=c++17", os.path.join(DIR, "P1535_brute.cpp"),
                    "-o", os.path.join(tmp, "brute")], check=True)

def run(bin_path, inp):
    p = subprocess.run([bin_path], input=inp, text=True, capture_output=True, check=True)
    return p.stdout.strip()

def main():
    tmp = tempfile.mkdtemp()
    try:
        compile_all(tmp)
        rng = random.Random(20261018)

        # 1. 随机对拍：小网格 + 小 T,保证暴力可跑
        for tc in range(300):
            n = rng.randint(2, 5)
            m = rng.randint(2, 5)
            t = rng.randint(1, 7)
            grid = []
            for _ in range(n):
                grid.append("".join(rng.choices([".", "*"], weights=[7, 3], k=m)))
            r1 = rng.randint(1, n); c1 = rng.randint(1, m)
            r2 = rng.randint(1, n); c2 = rng.randint(1, m)
            grid[r1-1] = grid[r1-1][:c1-1] + "." + grid[r1-1][c1:]
            grid[r2-1] = grid[r2-1][:c2-1] + "." + grid[r2-1][c2:]
            inp = f"{n} {m} {t}\n" + "\n".join(grid) + f"\n{r1} {c1} {r2} {c2}\n"
            a = run(os.path.join(tmp, "std"), inp)
            b = run(os.path.join(tmp, "brute"), inp)
            assert a == b, f"Mismatch on case {tc}:\n{inp}\nstd={a}, bf={b}"

        # 1b. 官方样例
        official = "4 5 6\n...*.\n...*.\n.....\n.....\n1 3 1 5\n"
        assert run(os.path.join(tmp, "std"), official) == "1", "official sample mismatch"

        # 2. 手工边界：相邻一步；不可达(奇偶不符)
        assert run(os.path.join(tmp, "std"), "2 2 1\n..\n..\n1 1 1 2\n") == "1"
        assert run(os.path.join(tmp, "std"), "2 2 1\n..\n..\n1 1 2 2\n") == "0"
        # 终点被树挡住 -> 0
        assert run(os.path.join(tmp, "std"), "2 2 2\n..\n.*\n1 1 2 2\n") == "0"

        # 3. 最坏规模计时：100x100 全平地, T=15
        max_inp = "100 100 15\n" + "\n".join(["." * 100 for _ in range(100)]) + "\n1 1 100 100\n"
        t0 = time.perf_counter()
        res = run(os.path.join(tmp, "std"), max_inp)
        dt = time.perf_counter() - t0
        print(f"P1535 check passed! 300 random cases + edges + 100x100 all-flat T=15 "
              f"(ans={res}) in {dt:.4f}s")
    finally:
        shutil.rmtree(tmp, ignore_errors=True)

if __name__ == "__main__":
    main()
