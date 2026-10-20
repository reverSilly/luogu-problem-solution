import subprocess, random, time, os, tempfile
from fractions import Fraction
from itertools import combinations

TMP = tempfile.gettempdir()
STD = os.path.join(TMP, "P1570")
BRU = os.path.join(TMP, "P1570_brute")

def compile_all():
    subprocess.run(["g++", "-O2", "-std=c++17", "P1570.cpp", "-o", STD], check=True)
    subprocess.run(["g++", "-O2", "-std=c++17", "P1570_brute.cpp", "-o", BRU], check=True)

def run(bin_name, inp):
    p = subprocess.run([bin_name], input=inp, text=True, capture_output=True, check=True)
    return p.stdout.strip()

def best_ratio(v, c, m):
    best = None
    for idx in combinations(range(len(v)), m):
        p = sum(v[i] for i in idx)
        q = sum(c[i] for i in idx)
        fr = Fraction(p, q)
        if best is None or fr > best:
            best = fr
    return best

def near_tie(fr):
    # 真值*1000 的小数部分落在 0.5 附近 => 3 位小数存在四舍五入歧义,跳过
    t = fr * 1000
    frac = t - (t.numerator // t.denominator)
    return abs(frac - Fraction(1, 2)) < Fraction(1, 1_000_000)

def gen(rng):
    while True:
        n = rng.randint(1, 10)
        m = rng.randint(1, n)
        v = [rng.randint(1, 30) for _ in range(n)]
        c = [rng.randint(1, 30) for _ in range(n)]
        if near_tie(best_ratio(v, c, m)):
            continue
        return f"{n} {m}\n" + " ".join(map(str, v)) + "\n" + " ".join(map(str, c)) + "\n"

def main():
    compile_all()
    rng = random.Random(20261018)

    # 1. 随机/小规模对拍(二分实数版 vs 组合枚举精确版),跳过四舍五入歧义的平局用例
    for tc in range(300):
        inp = gen(rng)
        o1 = run(STD, inp)
        o2 = run(BRU, inp)
        assert o1 == o2, f"Mismatch on:\n{inp}\nstd={o1}, bf={o2}"

    # 2. 手工边界(期望值均为手工推得/对齐题面提示)
    edge = [
        ("1 1\n7\n3\n", "2.333"),              # 单选 -> 7/3
        ("2 2\n1 1\n1 1\n", "1.000"),
        ("3 2\n2 3 1\n2 1 10\n", "1.667"),     # 与题面解释同构: (2+3)/(2+1)
        ("2 1\n1000 5\n1 9\n", "1000.000"),    # 触到答案上界 1000
    ]
    for inp, exp in edge:
        got = run(STD, inp)
        assert got == exp, f"Edge mismatch {inp!r}: got={got}, exp={exp}"
        assert run(BRU, inp) == exp, f"Edge brute mismatch {inp!r}"

    # 3. 最坏规模计时: n=200, m=100
    n, m = 200, 100
    v = [rng.randint(1, 10000) for _ in range(n)]
    c = [rng.randint(1, 10000) for _ in range(n)]
    big = f"{n} {m}\n" + " ".join(map(str, v)) + "\n" + " ".join(map(str, c)) + "\n"
    t0 = time.perf_counter()
    res = run(STD, big)
    dt = time.perf_counter() - t0

    print(f"P1570 check passed! 300 random + 4 edge cases; "
          f"n=200,m=100 -> {res} in {dt:.4f}s")

if __name__ == "__main__":
    main()
