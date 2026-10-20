import os
import random
import subprocess
import tempfile
import time

HERE = os.path.dirname(os.path.abspath(__file__))


def compile_all(tmpdir):
    std_bin = os.path.join(tmpdir, "std")
    bf_bin = os.path.join(tmpdir, "bf")
    subprocess.run(["g++", "-O2", "-std=c++17", os.path.join(HERE, "P1558.cpp"), "-o", std_bin], check=True)
    subprocess.run(["g++", "-O2", "-std=c++17", os.path.join(HERE, "P1558_brute.cpp"), "-o", bf_bin], check=True)
    return std_bin, bf_bin


def run(bin_path, inp):
    p = subprocess.run([bin_path], input=inp, text=True, capture_output=True, check=True)
    return p.stdout.strip()


def main():
    tmpdir = tempfile.mkdtemp(prefix="p1558_")
    std_bin, bf_bin = compile_all(tmpdir)
    rng = random.Random(20261018)

    # 1. 随机对拍:小 L / 小 T / 小 O,覆盖各种操作组合
    for tc in range(400):
        L = rng.randint(1, 20)
        T = rng.randint(1, 5)
        O = rng.randint(1, 25)
        lines = [f"{L} {T} {O}"]
        for _ in range(O):
            if rng.random() < 0.6:
                a = rng.randint(1, L)
                b = rng.randint(1, L)
                c = rng.randint(1, T)
                lines.append(f"C {a} {b} {c}")
            else:
                a = rng.randint(1, L)
                b = rng.randint(1, L)
                lines.append(f"P {a} {b}")
        inp = "\n".join(lines) + "\n"
        a = run(std_bin, inp)
        b = run(bf_bin, inp)
        assert a == b, f"Mismatch:\n{inp}\nstd={a}\nbf={b}"

    # 2. 手工边界
    manual = [
        ("1 1 1\nP 1 1\n", "1"),
        ("5 3 2\nC 1 5 2\nP 1 5\n", "1"),
        ("5 3 3\nC 1 5 2\nC 2 4 3\nP 1 5\n", "2"),
        ("5 3 3\nC 3 3 2\nC 4 4 3\nP 1 5\n", "3"),
        ("4 2 2\nC 4 1 2\nP 1 4\n", "1"),   # A>B 自动交换
    ]
    for inp, exp in manual:
        got = run(std_bin, inp)
        assert got == exp, f"manual {inp!r}: got {got}, expected {exp}"

    # 3. 最坏规模计时: L=1e5, T=30, O=1e5 随机
    L, T, O = 100000, 30, 100000
    lines = [f"{L} {T} {O}"]
    for _ in range(O):
        if rng.random() < 0.5:
            a = rng.randint(1, L)
            b = rng.randint(1, L)
            c = rng.randint(1, T)
            lines.append(f"C {a} {b} {c}")
        else:
            a = rng.randint(1, L)
            b = rng.randint(1, L)
            lines.append(f"P {a} {b}")
    big_inp = "\n".join(lines) + "\n"
    t0 = time.perf_counter()
    run(std_bin, big_inp)
    dt = time.perf_counter() - t0

    print(f"P1558 check passed! 400 random cases + 5 manual; "
          f"worst L={L},T={T},O={O} in {dt:.4f}s")


if __name__ == "__main__":
    main()
