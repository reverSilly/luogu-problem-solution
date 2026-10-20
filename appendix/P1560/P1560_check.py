import os
import random
import subprocess
import tempfile
import time

HERE = os.path.dirname(os.path.abspath(__file__))


def compile_all(tmpdir):
    std_bin = os.path.join(tmpdir, "std")
    bf_bin = os.path.join(tmpdir, "bf")
    subprocess.run(["g++", "-O2", "-std=c++17", os.path.join(HERE, "P1560.cpp"), "-o", std_bin], check=True)
    subprocess.run(["g++", "-O2", "-std=c++17", os.path.join(HERE, "P1560_brute.cpp"), "-o", bf_bin], check=True)
    return std_bin, bf_bin


def run(bin_path, inp):
    p = subprocess.run([bin_path], input=inp, text=True, capture_output=True, check=True)
    return p.stdout.strip()


def main():
    tmpdir = tempfile.mkdtemp(prefix="p1560_")
    std_bin, bf_bin = compile_all(tmpdir)
    rng = random.Random(20261018)

    # 1. 官方样例 (snail.in) -> 33
    sample = "8 4\nE2\nA6\nG1\nF5\n"
    got = run(std_bin, sample)
    assert got == "33", f"sample: got {got}, expected 33"
    got_bf = run(bf_bin, sample)
    assert got_bf == "33", f"sample brute: got {got_bf}"

    # 2. 随机小棋盘对拍 (N<=8,暴力与标程模型一致)
    for tc in range(350):
        N = rng.randint(2, 8)
        cells = [(r, c) for r in range(1, N + 1) for c in range(1, N + 1) if (r, c) != (1, 1)]
        rng.shuffle(cells)
        B = rng.randint(1, min(len(cells), 6))
        chosen = cells[:B]
        lines = [f"{N} {B}"]
        for r, c in chosen:
            lines.append(chr(ord('A') + c - 1) + str(r))
        inp = "\n".join(lines) + "\n"
        a = run(std_bin, inp)
        b = run(bf_bin, inp)
        assert a == b, f"Mismatch:\n{inp}\nstd={a}\nbf={b}"

    # 3. 手工边界:无路可走 / 只有一条路
    manual = [
        ("1 1\nA1\n", "1"),        # N=1 只有起点(题面 1<N,这里退化测试)
        ("2 1\nB2\n", None),       # 交给暴力核对
    ]
    for inp, exp in manual:
        a = run(std_bin, inp)
        b = run(bf_bin, inp)
        assert a == b, f"manual {inp!r}: std={a} bf={b}"
        if exp is not None:
            assert a == exp, f"manual {inp!r}: got {a}, expected {exp}"

    # 4. 最坏规模计时: N=120,随机障碍
    N = 120
    cells = set()
    while len(cells) < 200:
        r = rng.randint(1, N)
        c = rng.randint(1, min(N, 26))
        if (r, c) == (1, 1):
            continue
        cells.add((r, c))
    chosen = list(cells)[:200]
    lines = [f"{N} {len(chosen)}"]
    for r, c in chosen:
        lines.append(chr(ord('A') + c - 1) + str(r))
    big_inp = "\n".join(lines) + "\n"
    t0 = time.perf_counter()
    res = run(std_bin, big_inp)
    dt = time.perf_counter() - t0

    # 4b. 空棋盘 N=120
    empty_inp = f"120 1\nZ120\n"
    t1 = time.perf_counter()
    res2 = run(std_bin, empty_inp)
    dt2 = time.perf_counter() - t1

    print(f"P1560 check passed! sample=33, 350 random cases (N<=8) + 2 manual; "
          f"N=120 random barriers in {dt:.4f}s (ans={res}), N=120 empty in {dt2:.4f}s (ans={res2})")


if __name__ == "__main__":
    main()
