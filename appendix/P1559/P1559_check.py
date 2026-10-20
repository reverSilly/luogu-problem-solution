import os
import random
import subprocess
import tempfile
import time

HERE = os.path.dirname(os.path.abspath(__file__))


def compile_all(tmpdir):
    std_bin = os.path.join(tmpdir, "std")
    bf_bin = os.path.join(tmpdir, "bf")
    subprocess.run(["g++", "-O2", "-std=c++17", os.path.join(HERE, "P1559.cpp"), "-o", std_bin], check=True)
    subprocess.run(["g++", "-O2", "-std=c++17", os.path.join(HERE, "P1559_brute.cpp"), "-o", bf_bin], check=True)
    return std_bin, bf_bin


def run(bin_path, inp):
    p = subprocess.run([bin_path], input=inp, text=True, capture_output=True, check=True)
    return p.stdout.strip()


def main():
    tmpdir = tempfile.mkdtemp(prefix="p1559_")
    std_bin, bf_bin = compile_all(tmpdir)
    rng = random.Random(20261018)

    # 1. 随机对拍:n<=8,含负数与零的优势值(乘积可能负,检验 DP 正确性)
    for tc in range(400):
        n = rng.randint(1, 8)
        P = [[rng.randint(-30, 30) for _ in range(n)] for _ in range(n)]
        Q = [[rng.randint(-30, 30) for _ in range(n)] for _ in range(n)]
        lines = [str(n)]
        for row in P:
            lines.append(" ".join(map(str, row)))
        for row in Q:
            lines.append(" ".join(map(str, row)))
        inp = "\n".join(lines) + "\n"
        a = run(std_bin, inp)
        b = run(bf_bin, inp)
        assert a == b, f"Mismatch n={n}:\n{inp}\nstd={a}\nbf={b}"

    # 2. 小手工用例
    manual = [("1\n5\n7\n", "35")]
    for inp, exp in manual:
        got = run(std_bin, inp)
        assert got == exp, f"manual: {got} != {exp}"

    # 3. 最坏规模计时: n=20,随机较大值(保证在 int 范围乘积量级)
    n = 20
    P = [[rng.randint(0, 46340) for _ in range(n)] for _ in range(n)]
    Q = [[rng.randint(0, 46340) for _ in range(n)] for _ in range(n)]
    lines = [str(n)]
    for row in P:
        lines.append(" ".join(map(str, row)))
    for row in Q:
        lines.append(" ".join(map(str, row)))
    big_inp = "\n".join(lines) + "\n"
    t0 = time.perf_counter()
    res = run(std_bin, big_inp)
    dt = time.perf_counter() - t0

    print(f"P1559 check passed! 400 random cases (n<=8); n=20 worst-case in {dt:.4f}s (ans={res})")


if __name__ == "__main__":
    main()
