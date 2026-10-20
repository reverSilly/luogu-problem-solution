import subprocess, random, time

def compile_all():
    subprocess.run(["g++", "-O2", "-std=c++17", "P1576.cpp", "-o", "P1576"], check=True)
    subprocess.run(["g++", "-O2", "-std=c++17", "P1576_brute.cpp", "-o", "P1576_brute"], check=True)

def run(bin_name, inp):
    p = subprocess.run([f"./{bin_name}"], input=inp, text=True, capture_output=True, check=True)
    return p.stdout.strip()

def ok_float(o1, o2, tol=1e-6):
    a = o1.split()
    b = o2.split()
    assert len(a) == len(b), f"line count {len(a)} vs {len(b)}: {o1!r} vs {o2!r}"
    for x, y in zip(a, b):
        fx, fy = float(x), float(y)
        assert abs(fx - fy) <= tol * max(1.0, abs(fy)), f"{x} vs {y}"

def main():
    compile_all()
    rng = random.Random(20261018)

    # 1. 随机对拍：n<=7，先造一棵随机树保证连通，再加若干随机边（含重边）
    for tc in range(300):
        n = rng.randint(2, 7)
        edges = []
        for i in range(1, n):
            j = rng.randint(0, i - 1)
            edges.append((j, i, rng.randint(0, 99)))
        for _ in range(rng.randint(0, 8)):
            u = rng.randrange(n)
            v = rng.randrange(n)
            if u == v:
                continue
            if u > v:
                u, v = v, u
            edges.append((u, v, rng.randint(0, 99)))
        m = len(edges)
        A = rng.randrange(n)
        B = rng.randrange(n)
        lines = [f"{n} {m}"] + [f"{u+1} {v+1} {z}" for u, v, z in edges] + [f"{A+1} {B+1}"]
        inp = "\n".join(lines) + "\n"
        o1 = run("P1576", inp)
        o2 = run("P1576_brute", inp)
        ok_float(o1, o2)

    # 2. 边界：A==B（无需转账，答案 100）与 手续费 0（答案 100 且路径任意）
    for inp in ["2 1\n1 2 50\n1 1\n", "3 2\n1 2 0\n2 3 0\n1 3\n", "1 0\n1 1\n"]:
        o1 = run("P1576", inp)
        o2 = run("P1576_brute", inp)
        ok_float(o1, o2)

    # 3. 最坏规模计时：n=2000, m=1e5，链保证连通 + 随机补边
    n, m = 2000, 100000
    lines = [f"{n} {m}"]
    for i in range(1, n):
        lines.append(f"{i} {i+1} {rng.randint(0, 99)}")
    for _ in range(m - (n - 1)):
        u = rng.randint(1, n)
        v = rng.randint(1, n)
        if u == v:
            v = v % n + 1
        lines.append(f"{u} {v} {rng.randint(0, 99)}")
    lines.append(f"1 {n}")
    inp = "\n".join(lines) + "\n"
    t0 = time.perf_counter()
    res = run("P1576", inp)
    dt = time.perf_counter() - t0

    print(f"P1576 check passed! 300 random (n<=7, dfs-brute) + 3 boundary cases all match; "
          f"n=2000,m=1e5 -> {res} in {dt:.4f}s")

if __name__ == "__main__":
    main()
