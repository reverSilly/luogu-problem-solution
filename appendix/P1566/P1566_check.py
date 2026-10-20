import subprocess, random, time

BINDIR = "/tmp"

def compile_all():
    subprocess.run(["g++", "-O2", "-std=c++17", "P1566.cpp", "-o", f"{BINDIR}/P1566"], check=True)
    subprocess.run(["g++", "-O2", "-std=c++17", "P1566_brute.cpp", "-o", f"{BINDIR}/P1566_brute"], check=True)

def run(bin_name, inp):
    p = subprocess.run([f"{BINDIR}/{bin_name}"], input=inp, text=True, capture_output=True, check=True)
    return p.stdout.strip()

def main():
    compile_all()
    rng = random.Random(20261018)

    # 0. 官方样例: 1 / 0 / 7
    samp = "3\n3 1 2 3\n3 1 2 5\n6 1 2 3 5 4 6\n"
    r = run("P1566", samp)
    assert r == "1\n0\n7", f"sample fail: got {r!r}"

    # 1. 随机小规模对拍(m<=14 便于 2^(m-1) 穷举)
    for tc in range(350):
        t = rng.randint(1, 3)
        lines = []
        for _ in range(t):
            m = rng.randint(1, 14)
            vals = rng.sample(range(1, 31), m)      # 小范围使加等式更多
            lines.append(f"{m} " + " ".join(map(str, vals)))
        inp = f"{t}\n" + "\n".join(lines) + "\n"
        a = run("P1566", inp)
        b = run("P1566_brute", inp)
        assert a == b, f"Mismatch:\n{inp}\nstd={a}, bf={b}"

    # 2. 最坏规模计时 t=10, m=30, x<=1000
    lines = []
    for _ in range(10):
        vals = rng.sample(range(1, 1001), 30)
        lines.append("30 " + " ".join(map(str, vals)))
    inp = "10\n" + "\n".join(lines) + "\n"
    t0 = time.perf_counter()
    res = run("P1566", inp)
    dt = time.perf_counter() - t0

    print(f"P1566 PASS: sample=1/0/7, 350 random cases ok; worst t=10,m=30,x<=1000 outputs={res.split()} in {dt:.4f}s")

if __name__ == "__main__":
    main()
