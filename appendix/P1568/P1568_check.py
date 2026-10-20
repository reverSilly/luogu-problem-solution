import subprocess, random, time

BINDIR = "/tmp"

def compile_all():
    subprocess.run(["g++", "-O2", "-std=c++17", "P1568.cpp", "-o", f"{BINDIR}/P1568"], check=True)
    subprocess.run(["g++", "-O2", "-std=c++17", "P1568_brute.cpp", "-o", f"{BINDIR}/P1568_brute"], check=True)

def run(bin_name, inp):
    p = subprocess.run([f"{BINDIR}/{bin_name}"], input=inp, text=True, capture_output=True, check=True)
    return p.stdout.strip()

def main():
    compile_all()
    rng = random.Random(20261018)

    # 0. 官方样例 -> 2
    samp = "4 3\n1 2\n4 1\n1 1\n2 10\n2 3\n1 2\n3 9\n"
    r = run("P1568", samp)
    assert r == "2", f"sample fail: got {r}"

    # 1. 随机小规模对拍(两侧总时间严格相同)
    for tc in range(500):
        n = rng.randint(1, 4)
        m = rng.randint(1, 4)
        T = rng.randint(1, 24)

        def part(k):
            cuts = sorted(rng.randint(0, T) for _ in range(k - 1))
            res, prev = [], 0
            for c in cuts:
                res.append(c - prev)
                prev = c
            res.append(T - prev)
            return res

        ds, dk = part(n), part(m)
        sh = [f"{rng.randint(0, 5)} {ds[i]}" for i in range(n)]
        kc = [f"{rng.randint(0, 5)} {dk[i]}" for i in range(m)]
        inp = f"{n} {m}\n" + "\n".join(sh) + "\n" + "\n".join(kc) + "\n"
        a = run("P1568", inp)
        b = run("P1568_brute", inp)
        assert a == b, f"Mismatch:\n{inp}\nstd={a}, bf={b}"

    # 2. 最坏规模计时 N=M=1000, 每段时长 1000 => 总时间 1e6
    n = m = 1000
    sh = ["1 1000"] * n
    kc = ["0 1000"] * (m // 2) + ["2 1000"] * (m - m // 2)
    inp = f"{n} {m}\n" + "\n".join(sh) + "\n" + "\n".join(kc) + "\n"
    t0 = time.perf_counter()
    res = run("P1568", inp)
    dt = time.perf_counter() - t0

    print(f"P1568 PASS: sample=2, 500 random cases ok; worst N=M=1000,T=1e6 ans={res} in {dt:.4f}s")

if __name__ == "__main__":
    main()
