import subprocess, random, time

def compile_all():
    subprocess.run(["g++", "-O2", "-std=c++17", "P1561.cpp", "-o", "P1561"], check=True)
    subprocess.run(["g++", "-O2", "-std=c++17", "P1561_brute.cpp", "-o", "P1561_brute"], check=True)

def run(bin_name, inp):
    p = subprocess.run([f"./{bin_name}"], input=inp, text=True, capture_output=True, check=True)
    return p.stdout.strip()

def main():
    compile_all()
    # 1. USACO 12JAN 官方样例
    sample = "3\n6 4\n8 1\n2 3\n"
    assert run("P1561", sample) == "17"
    assert run("P1561_brute", sample) == "17"

    # 2. 随机对拍 500 组 (n <= 8 vs 全排列暴力)
    rng = random.Random(20261021)
    for _ in range(500):
        n = rng.randint(1, 8)
        lines = [str(n)]
        for _ in range(n):
            lines.append(f"{rng.randint(1, 30)} {rng.randint(1, 30)}")
        inp = "\n".join(lines) + "\n"
        assert run("P1561", inp) == run("P1561_brute", inp)

    # 3. 极限规模 N = 25,000
    n_max = 25000
    lines = [str(n_max)]
    for _ in range(n_max):
        lines.append(f"{rng.randint(1, 50000)} {rng.randint(1, 50000)}")
    max_inp = "\n".join(lines) + "\n"
    t0 = time.perf_counter()
    res = run("P1561", max_inp)
    dt = time.perf_counter() - t0
    print(f"P1561 check passed! Sample (17) + 500 random cases + N=25,000 (ans={res}) in {dt:.4f}s")

if __name__ == "__main__":
    main()
