import subprocess, random, time, os

SRC = os.path.dirname(os.path.abspath(__file__))
TMP = "/tmp/P1550_bin"

def compile_all():
    os.makedirs(TMP, exist_ok=True)
    subprocess.run(["g++", "-O2", "-std=c++17", os.path.join(SRC, "P1550.cpp"),
                    "-o", os.path.join(TMP, "P1550")], check=True)
    subprocess.run(["g++", "-O2", "-std=c++17", os.path.join(SRC, "P1550_brute.cpp"),
                    "-o", os.path.join(TMP, "P1550_brute")], check=True)

def run(bin_name, inp):
    p = subprocess.run([os.path.join(TMP, bin_name)], input=inp, text=True,
                       capture_output=True, check=True)
    return p.stdout.strip()

def gen(rng, n, wmax, pmax, symmetric=True):
    lines = [str(n)]
    W = [rng.randint(1, wmax) for _ in range(n)]
    lines += [str(x) for x in W]
    P = [[0] * n for _ in range(n)]
    for i in range(n):
        for j in range(n):
            if i < j:
                P[i][j] = rng.randint(0, pmax)
            if i > j:
                P[i][j] = P[j][i] if symmetric else rng.randint(0, pmax)
    for i in range(n):
        lines.append(" ".join(str(P[i][j]) for j in range(n)))
    return "\n".join(lines) + "\n"

def main():
    compile_all()
    rng = random.Random(20261018)
    # 1. 随机对拍:小规模 n<=5(暴力枚举候选边子集)
    for tc in range(300):
        n = rng.randint(1, 5)
        inp = gen(rng, n, 10, 12)
        out_std = run("P1550", inp)
        out_bf = run("P1550_brute", inp)
        assert out_std == out_bf, f"Mismatch on:\n{inp}\nstd={out_std}, bf={out_bf}"

    # 2. 手工边界
    # n=1:只有挖井一种选择
    inp1 = "1\n5\n0\n"
    assert run("P1550", inp1) == "5" == run("P1550_brute", inp1)
    # 井便宜:各自挖井
    inp2 = "3\n1\n1\n1\n0 100 100\n100 0 100\n100 100 0\n"
    assert run("P1550", inp2) == "3" == run("P1550_brute", inp2)

    # 3. 最坏规模计时:n=300,权值取满
    n = 300
    lines = [str(n)]
    lines += [str(100000) for _ in range(n)]
    for i in range(n):
        lines.append(" ".join("0" if i == j else "100000" for j in range(n)))
    worst = "\n".join(lines) + "\n"
    t0 = time.perf_counter()
    res = run("P1550", worst)
    dt = time.perf_counter() - t0
    print(f"P1550 check PASS! 300 random + 2 hand cases; worst n=300 ans={res} in {dt:.4f}s")

if __name__ == "__main__":
    main()
