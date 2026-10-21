import subprocess, random, time

def compile_all():
    subprocess.run(["g++", "-O2", "-std=c++17", "P1560.cpp", "-o", "P1560"], check=True)
    subprocess.run(["g++", "-O2", "-std=c++17", "P1560_brute.cpp", "-o", "P1560_brute"], check=True)

def run(bin_name, inp):
    p = subprocess.run([f"./{bin_name}"], input=inp, text=True, capture_output=True, check=True)
    return p.stdout.strip()

def main():
    compile_all()
    # 1. USACO 5.2 官方样例 (8x8, 4 obstacles -> 33)
    sample = "8 4\nE2\nA6\nG1\nF5\n"
    assert run("P1560", sample) == "33"
    assert run("P1560_brute", sample) == "33"

    # 2. 随机棋盘对拍 300 组
    rng = random.Random(20261021)
    for _ in range(300):
        n = rng.randint(2, 18)
        b = rng.randint(1, min(25, n * n - 2))
        obs = set()
        while len(obs) < b:
            r = rng.randint(1, n)
            c = rng.randint(0, n - 1)
            if r == 1 and c == 0:
                continue
            obs.add(f"{chr(ord('A') + c)}{r}")
        inp = f"{n} {b}\n" + "\n".join(obs) + "\n"
        assert run("P1560", inp) == run("P1560_brute", inp)

    # 3. 棋盘性能测试 (N = 50, B = 150, ASCII 7-bit 范围内)
    n_max, b_max = 50, 150
    obs = set()
    while len(obs) < b_max:
        r = rng.randint(1, n_max)
        c = rng.randint(0, n_max - 1)
        if r == 1 and c == 0:
            continue
        obs.add(f"{chr(ord('A') + c)}{r}")
    max_inp = f"{n_max} {b_max}\n" + "\n".join(obs) + "\n"
    t0 = time.perf_counter()
    res = run("P1560", max_inp)
    dt = time.perf_counter() - t0
    assert res == run("P1560_brute", max_inp)
    print(f"P1560 check passed! Sample (33) + 300 random boards + 50x50 (ans={res}) in {dt:.4f}s")

if __name__ == "__main__":
    main()
