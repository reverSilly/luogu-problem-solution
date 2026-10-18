import subprocess, random, time

def compile_all():
    subprocess.run(["g++", "-O2", "-std=c++17", "P1552.cpp", "-o", "P1552"], check=True)
    subprocess.run(["g++", "-O2", "-std=c++17", "P1552_brute.cpp", "-o", "P1552_brute"], check=True)

def run(bin_name, inp):
    p = subprocess.run([f"./{bin_name}"], input=inp, text=True, capture_output=True, check=True)
    return p.stdout.strip()

def main():
    compile_all()
    # 1. APIO2012 官方样例
    sample = "5 4\n0 3 3\n1 3 5\n2 2 2\n1 2 4\n2 3 1\n"
    assert run("P1552", sample) == "6"
    assert run("P1552_brute", sample) == "6"

    # 2. 随机树对拍 500 组
    rng = random.Random(20261018)
    for _ in range(500):
        n = rng.randint(1, 80)
        m = rng.randint(1, 500)
        lines = [f"{n} {m}"]
        for i in range(1, n + 1):
            b = 0 if i == 1 else rng.randint(1, i - 1)
            c = rng.randint(1, m)
            l = rng.randint(1, 1000)
            lines.append(f"{b} {c} {l}")
        inp = "\n".join(lines) + "\n"
        assert run("P1552", inp) == run("P1552_brute", inp)

    # 3. 极限规模测试 N = 100,000, M = 10^9
    n_max = 100000
    m_max = 1000000000
    lines = [f"{n_max} {m_max}"]
    for i in range(1, n_max + 1):
        b = 0 if i == 1 else rng.randint(max(1, i - 5), i - 1)
        c = rng.randint(1, 50000)
        l = rng.randint(1, 1000000000)
        lines.append(f"{b} {c} {l}")
    max_inp = "\n".join(lines) + "\n"
    t0 = time.perf_counter()
    res = run("P1552", max_inp)
    dt = time.perf_counter() - t0
    print(f"P1552 check passed! Sample + 500 random trees + N=100,000 (ans={res}) in {dt:.4f}s")

if __name__ == "__main__":
    main()
