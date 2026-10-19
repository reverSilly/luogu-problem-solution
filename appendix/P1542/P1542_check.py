import subprocess, random, time

def compile_all():
    subprocess.run(["g++", "-O2", "-std=c++17", "P1542.cpp", "-o", "P1542"], check=True)
    subprocess.run(["g++", "-O2", "-std=c++17", "P1542_brute.cpp", "-o", "P1542_brute"], check=True)

def run(bin_name, inp):
    p = subprocess.run([f"./{bin_name}"], input=inp, text=True, capture_output=True, check=True)
    return p.stdout.strip()

def gen(rng, n, xhi, yspan, shi):
    xs = sorted(rng.sample(range(1, xhi + 1), n))   # 严格递增的 x_i
    lines = [str(n)]
    for i in range(n):
        x = xs[i]
        y = x + rng.randint(0, yspan)
        s = rng.randint(1, shi)
        lines.append(f"{x} {y} {s}")
    return "\n".join(lines) + "\n"

def main():
    compile_all()

    # 1. 官方样例
    sample = "3\n1 2 2\n6 6 2\n7 8 4\n"
    assert run("P1542", sample) == "2.00"
    assert run("P1542_brute", sample) == "2.00"

    # 2. 手工边界: n=1, x=y=s=1 => 1/1 = 1.00
    assert run("P1542", "1\n1 1 1\n") == run("P1542_brute", "1\n1 1 1\n") == "1.00"

    # 3. 随机对拍 400 组 (x 严格递增)
    rng = random.Random(20261018)
    for _ in range(400):
        n = rng.randint(1, 20)
        inp = gen(rng, n, 2000, 500, 1000)
        a = run("P1542", inp)
        b = run("P1542_brute", inp)
        assert a == b, f"Mismatch on:\n{inp}\nstd={a}, bf={b}"

    # 4. 最坏规模 n=2e5, x_i=i, y_i=1e8, s_i=1e7
    n = 200000
    lines = [str(n)]
    for i in range(1, n + 1):
        lines.append(f"{i} 100000000 10000000")
    max_inp = "\n".join(lines) + "\n"
    t0 = time.perf_counter()
    res = run("P1542", max_inp)
    dt = time.perf_counter() - t0
    print(f"P1542 check passed! Sample + 400 random + edge + max(n=2e5,ans={res}) in {dt:.4f}s")

if __name__ == "__main__":
    main()
