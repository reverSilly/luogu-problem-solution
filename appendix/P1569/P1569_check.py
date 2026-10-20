import subprocess, random, time, os, tempfile

TMP = tempfile.gettempdir()
STD = os.path.join(TMP, "P1569")
BRU = os.path.join(TMP, "P1569_brute")

def compile_all():
    subprocess.run(["g++", "-O2", "-std=c++17", "P1569.cpp", "-o", STD], check=True)
    subprocess.run(["g++", "-O2", "-std=c++17", "P1569_brute.cpp", "-o", BRU], check=True)

def run(bin_name, inp):
    p = subprocess.run([bin_name], input=inp, text=True, capture_output=True, check=True)
    return p.stdout.strip()

def gen(rng):
    n = rng.randint(1, 12)
    # 混入正负与零,覆盖率尤其关注总和为负(Impossible)的情形
    vals = [rng.randint(-10, 10) for _ in range(n)]
    return f"{n}\n" + "\n".join(str(x) for x in vals) + "\n"

def main():
    compile_all()
    rng = random.Random(20261018)

    # 1. 随机/小规模对拍
    for tc in range(300):
        inp = gen(rng)
        o1 = run(STD, inp)
        o2 = run(BRU, inp)
        assert o1 == o2, f"Mismatch on:\n{inp}\nstd={o1}, bf={o2}"

    # 2. 手工边界
    edge = [
        ("1\n-5\n", "Impossible"),
        ("1\n0\n", "1"),
        ("4\n2\n3\n-3\n1\n", "3"),
        ("3\n-1\n-1\n-1\n", "Impossible"),
        ("5\n1\n1\n1\n1\n1\n", "5"),
        ("3\n5\n-1\n-1\n", "1"),
    ]
    for inp, exp in edge:
        got = run(STD, inp)
        assert got == exp, f"Edge mismatch {inp!r}: got={got}, exp={exp}"

    # 3. 最坏规模计时: n=1000, 全 1(答案 1000)
    big = "1000\n" + "\n".join(["1"] * 1000) + "\n"
    t0 = time.perf_counter()
    res = run(STD, big)
    dt = time.perf_counter() - t0
    # 交替就近取零的对抗数据
    big2 = "1000\n" + "\n".join(["1" if i % 2 == 0 else "-1" for i in range(1000)]) + "\n"
    t1 = time.perf_counter()
    res2 = run(STD, big2)
    dt2 = time.perf_counter() - t1

    print(f"P1569 check passed! 300 random + 6 edge cases; "
          f"n=1000 all-1 -> {res} in {dt:.4f}s; n=1000 alternating -> {res2} in {dt2:.4f}s")

if __name__ == "__main__":
    main()
