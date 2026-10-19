import subprocess, random, time

def compile_all():
    subprocess.run(["g++", "-O2", "-std=c++17", "P1546.cpp", "-o", "P1546"], check=True)
    subprocess.run(["g++", "-O2", "-std=c++17", "P1546_brute.cpp", "-o", "P1546_brute"], check=True)

def run(bin_name, inp):
    p = subprocess.run([f"./{bin_name}"], input=inp, text=True, capture_output=True, check=True)
    return p.stdout.strip()

def gen(rng, n, hi):
    # 生成对称邻接矩阵, 对角为 0, 权值在 [0,hi]
    m = [[0] * n for _ in range(n)]
    for i in range(n):
        for j in range(i + 1, n):
            w = rng.randint(1, hi)
            m[i][j] = m[j][i] = w
    inp = f"{n}\n" + "\n".join(" ".join(map(str, row)) for row in m) + "\n"
    return inp

def main():
    compile_all()

    # 1. 官方 USACO 样例 (n=4, 答案 28)
    sample = "4\n0 4 9 21\n4 0 8 17\n9 8 0 16\n21 17 16 0\n"
    assert run("P1546", sample) == "28"
    assert run("P1546_brute", sample) == "28"

    # 2. 随机对拍 400 组
    rng = random.Random(20261018)
    for _ in range(400):
        n = rng.randint(3, 25)
        inp = gen(rng, n, rng.choice([10, 100, 1000, 100000]))
        a = run("P1546", inp)
        b = run("P1546_brute", inp)
        assert a == b, f"Mismatch on n={n}: std={a}, bf={b}"

    # 3. 最坏规模 n=100 稠密完全图
    inp = gen(rng, 100, 100000)
    t0 = time.perf_counter()
    res = run("P1546", inp)
    dt = time.perf_counter() - t0
    print(f"P1546 check passed! Sample + 400 random cases + max(n=100,ans={res}) in {dt:.4f}s")

if __name__ == "__main__":
    main()
