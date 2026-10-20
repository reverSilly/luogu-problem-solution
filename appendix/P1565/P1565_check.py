import subprocess, random, time

BINDIR = "/tmp"

def compile_all():
    subprocess.run(["g++", "-O2", "-std=c++17", "P1565.cpp", "-o", f"{BINDIR}/P1565"], check=True)
    subprocess.run(["g++", "-O2", "-std=c++17", "P1565_brute.cpp", "-o", f"{BINDIR}/P1565_brute"], check=True)

def run(bin_name, inp):
    p = subprocess.run([f"{BINDIR}/{bin_name}"], input=inp, text=True, capture_output=True, check=True)
    return p.stdout.strip()

def main():
    compile_all()
    rng = random.Random(20261018)

    # 0. 官方样例: 3x2 -> 4
    samp = "3 2\n4 0\n-10 8\n-2 -2\n"
    r = run("P1565", samp)
    assert r == "4", f"sample fail: got {r}"

    # 1. 随机小规模对拍
    for tc in range(350):
        n = rng.randint(1, 8)
        m = rng.randint(1, 8)
        vals = [rng.randint(-8, 8) for _ in range(n * m)]
        if tc % 5 == 0:                       # 部分用例全非正,答案应为 0
            vals = [-rng.randint(1, 8) for _ in range(n * m)]
        rows = [" ".join(map(str, vals[i * m:(i + 1) * m])) for i in range(n)]
        inp = f"{n} {m}\n" + "\n".join(rows) + "\n"
        a = run("P1565", inp)
        b = run("P1565_brute", inp)
        assert a == b, f"Mismatch:\n{inp}\nstd={a}, bf={b}"

    # 2. 最坏规模计时 N=M=200(随机大值) 与 全正
    n = m = 200
    vals = [rng.randint(-2 * 10 ** 9, 16315) for _ in range(n * m)]
    rows = [" ".join(map(str, vals[i * m:(i + 1) * m])) for i in range(n)]
    inp = f"{n} {m}\n" + "\n".join(rows) + "\n"
    t0 = time.perf_counter()
    res = run("P1565", inp)
    dt = time.perf_counter() - t0

    inp2 = f"{n} {m}\n" + "\n".join([" ".join(["5"] * m) for _ in range(n)]) + "\n"
    res2 = run("P1565", inp2)

    print(f"P1565 PASS: sample=4, 350 random cases ok; worst N=M=200(random) ans={res} in {dt:.4f}s; all-positive ans={res2} (expect 40000)")

if __name__ == "__main__":
    main()
