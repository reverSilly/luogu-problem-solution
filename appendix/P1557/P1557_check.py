import os
import random
import subprocess
import sys
import tempfile
import time

HERE = os.path.dirname(os.path.abspath(__file__))


def compile_all(tmpdir):
    std_bin = os.path.join(tmpdir, "std")
    bf_bin = os.path.join(tmpdir, "bf")
    subprocess.run(["g++", "-O2", "-std=c++17", os.path.join(HERE, "P1557.cpp"), "-o", std_bin], check=True)
    subprocess.run(["g++", "-O2", "-std=c++17", os.path.join(HERE, "P1557_brute.cpp"), "-o", bf_bin], check=True)
    return std_bin, bf_bin


def run(bin_path, inp):
    p = subprocess.run([bin_path], input=inp, text=True, capture_output=True, check=True)
    return p.stdout.strip()


def rand_term(rng):
    # 随机生成合法 Kruscal 项: [符号串][数值] 或 [单符号](系数)[数值]
    val = str(rng.randint(0, 999))
    form = rng.randint(0, 3)
    if form == 0:                       # 裸数值(隐含 +1)
        return val
    if form == 1:                       # 连续同号串
        k = rng.randint(1, 4)
        c = rng.choice("+-")
        return c * k + val
    if form == 2:                       # 括号型
        c = rng.choice("+-")
        m = rng.randint(1, 6)
        return f"{c}({m}){val}"
    # 混合:仍属于"净符号数"语义,用于压测实现的对称性
    k = rng.randint(1, 3)
    sgns = "".join(rng.choice("+-") for _ in range(k))
    return sgns + val


def main():
    tmpdir = tempfile.mkdtemp(prefix="p1557_")
    std_bin, bf_bin = compile_all(tmpdir)
    rng = random.Random(20261018)

    # 1. 官方样例
    samples = [
        ("+(1)23+(3)15-(1)2", "66"),
        ("+23+++15-2", "66"),
        ("+23+(3)15-2", "66"),
        ("23+++15-2", "66"),
    ]
    for inp, exp in samples:
        got = run(std_bin, inp + "\n")
        assert got == exp, f"sample {inp}: got {got}, expected {exp}"

    # 2. 随机对拍 (小数值,保证暴力 long long 不溢出)
    for _ in range(400):
        terms = [rand_term(rng) for _ in range(rng.randint(1, 8))]
        expr = "".join(terms)
        inp = expr + "\n"
        a = run(std_bin, inp)
        b = run(bf_bin, inp)
        assert a == b, f"Mismatch on {expr!r}: std={a}, bf={b}"

    # 3. 边界/手工用例
    manual = [
        ("-15", "-15"),
        ("--15", "-30"),
        ("---15", "-45"),
        ("+++15", "45"),
        ("+(3)15", "45"),
        ("-(3)15", "-45"),
        ("+0", "0"),
        ("-0", "0"),
        ("+(1)0", "0"),
    ]
    for inp, exp in manual:
        got = run(std_bin, inp + "\n")
        assert got == exp, f"manual {inp}: got {got}, expected {exp}"

    # 4. 最坏规模计时: ~2000 位数的大数,长度接近 2000
    big = "+" + "9" * 1990 + "\n"
    t0 = time.perf_counter()
    res = run(std_bin, big)
    dt = time.perf_counter() - t0
    assert res == "9" * 1990, "bigcase wrong"
    assert len(big.strip()) <= 2000

    # 4b. 大量小项(接近 1000 项,长度仍 <=2000)
    parts = []
    cur = 0
    while cur < 1950:
        term = rng.choice("+-") + str(rng.randint(1, 99))
        parts.append(term)
        cur += len(term)
    many = "".join(parts)
    t1 = time.perf_counter()
    run(std_bin, many + "\n")
    dt2 = time.perf_counter() - t1
    assert len(many) <= 2000

    print(f"P1557 check passed! 400 random cases + 10 manual samples; "
          f"worst bigcase(len={len(big.strip())}) in {dt:.4f}s, many-terms(len={len(many)}) in {dt2:.4f}s")


if __name__ == "__main__":
    main()
