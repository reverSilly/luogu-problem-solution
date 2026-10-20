import subprocess, random, time, os

SRC = os.path.dirname(os.path.abspath(__file__))
TMP = "/tmp/P1547_bin"

def compile_all():
    os.makedirs(TMP, exist_ok=True)
    subprocess.run(["g++", "-O2", "-std=c++17", os.path.join(SRC, "P1547.cpp"),
                    "-o", os.path.join(TMP, "P1547")], check=True)
    subprocess.run(["g++", "-O2", "-std=c++17", os.path.join(SRC, "P1547_brute.cpp"),
                    "-o", os.path.join(TMP, "P1547_brute")], check=True)

def run(bin_name, inp):
    p = subprocess.run([os.path.join(TMP, bin_name)], input=inp, text=True,
                       capture_output=True, check=True)
    return p.stdout.strip()

def gen(rng, n, m):
    # 先生成一棵随机生成树保证连通,再补足到 m 条边(允许多重边)
    edges = []
    for v in range(1, n):
        u = rng.randint(0, v - 1)
        w = rng.randint(1, 20)
        edges.append((u, v, w))
    while len(edges) < m:
        u = rng.randint(0, n - 1)
        v = rng.randint(0, n - 1)
        if u == v:
            continue
        w = rng.randint(1, 20)
        edges.append((u, v, w))
    rng.shuffle(edges)
    lines = [f"{n} {len(edges)}"]
    for u, v, w in edges:
        lines.append(f"{u+1} {v+1} {w}")
    return "\n".join(lines) + "\n"

def main():
    compile_all()
    rng = random.Random(20261018)
    # 1. 随机对拍:小规模,含多重边
    for tc in range(300):
        n = rng.randint(2, 5)
        m = rng.randint(n - 1, 10)
        inp = gen(rng, n, m)
        out_std = run("P1547", inp)
        out_bf = run("P1547_brute", inp)
        assert out_std == out_bf, f"Mismatch on:\n{inp}\nstd={out_std}, bf={out_bf}"

    # 2. 手工边界用例:2 个点 1 条边
    hand_inp = "2 1\n1 2 7\n"
    assert run("P1547", hand_inp) == "7" == run("P1547_brute", hand_inp)

    # 3. 最坏规模计时:N=2000,M=10000(链 + 随机补边);总长约束 sum<=1e9,故每条约 1e5
    n, m = 2000, 10000
    edges = [(i, i + 1, 100000) for i in range(n - 1)]
    while len(edges) < m:
        u = rng.randint(0, n - 1); v = rng.randint(0, n - 1)
        if u != v:
            edges.append((u, v, 100000))
    rng.shuffle(edges)
    worst = f"{n} {len(edges)}\n" + "\n".join(f"{u+1} {v+1} {w}" for u, v, w in edges) + "\n"
    t0 = time.perf_counter()
    res = run("P1547", worst)
    dt = time.perf_counter() - t0
    print(f"P1547 check PASS! 300 random + 1 hand case; worst N=2000 M=10000 ans={res} in {dt:.4f}s")

if __name__ == "__main__":
    main()
