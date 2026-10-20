import subprocess, random, time, os, tempfile

TMP = tempfile.gettempdir()
STD = os.path.join(TMP, "P1571")
BRU = os.path.join(TMP, "P1571_brute")

def compile_all():
    subprocess.run(["g++", "-O2", "-std=c++17", "P1571.cpp", "-o", STD], check=True)
    subprocess.run(["g++", "-O2", "-std=c++17", "P1571_brute.cpp", "-o", BRU], check=True)

def run(bin_name, inp):
    p = subprocess.run([bin_name], input=inp, text=True, capture_output=True, check=True)
    return p.stdout.strip()

def gen(rng):
    n = rng.randint(0, 8)
    m = rng.randint(0, 8)
    # 两行各自元素两两不同(题面保证)
    A = rng.sample(range(1, 15), n)
    B = rng.sample(range(1, 15), m)
    lines = [f"{n} {m}"]
    if n:
        lines.append(" ".join(map(str, A)))
    if m:
        lines.append(" ".join(map(str, B)))
    return "\n".join(lines) + "\n"

def main():
    compile_all()
    rng = random.Random(20261018)

    # 1. 随机/小规模对拍(含 n=0 / m=0 / 无交集)
    for tc in range(300):
        inp = gen(rng)
        o1 = run(STD, inp)
        o2 = run(BRU, inp)
        assert o1 == o2, f"Mismatch on:\n{inp}\nstd={o1}, bf={o2}"

    # 2. 手工边界(编号按第一行次序输出,无匹配则空行)
    edge = [
        ("3 3\n1 2 3\n3 2 1\n", "1 2 3"),
        ("3 2\n5 6 7\n1 2\n", ""),
        ("0 3\n1 2 3\n", ""),
        ("4 4\n9 3 8 1\n1 9 2 8\n", "9 8 1"),
        ("1 1\n1000000000\n1000000000\n", "1000000000"),
    ]
    for inp, exp in edge:
        got = run(STD, inp)
        assert got == exp, f"Edge mismatch {inp!r}: got={got}, exp={exp}"
        assert run(BRU, inp) == exp, f"Edge brute mismatch {inp!r}"

    # 3. 最坏规模计时: n=m=1e5,一半元素重叠
    big_ids = rng.sample(range(1, 2_000_000_000), 100_000)
    A = big_ids
    B = big_ids[:50_000] + rng.sample(range(1, 2_000_000_000), 50_000)
    big = f"{len(A)} {len(B)}\n" + " ".join(map(str, A)) + "\n" + " ".join(map(str, B)) + "\n"
    t0 = time.perf_counter()
    res = run(STD, big)
    dt = time.perf_counter() - t0
    cnt = len(res.split()) if res else 0

    print(f"P1571 check passed! 300 random + 5 edge cases; "
          f"n=m=100000 -> {cnt} matches in {dt:.4f}s")

if __name__ == "__main__":
    main()
