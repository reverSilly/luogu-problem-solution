import subprocess, random, time, os, tempfile

TMP = tempfile.gettempdir()
STD = os.path.join(TMP, "P1572")
BRU = os.path.join(TMP, "P1572_brute")

def compile_all():
    subprocess.run(["g++", "-O2", "-std=c++17", "P1572.cpp", "-o", STD], check=True)
    subprocess.run(["g++", "-O2", "-std=c++17", "P1572_brute.cpp", "-o", BRU], check=True)

def run(bin_name, inp):
    p = subprocess.run([bin_name], input=inp, text=True, capture_output=True, check=True)
    return p.stdout.strip()

def gen(rng):
    k = rng.randint(1, 5)
    parts = []
    for t in range(k):
        a = rng.randint(1, 30)
        b = rng.randint(1, 30)
        if t == 0:
            pre = "-" if rng.random() < 0.5 else ""
            parts.append(f"{pre}{a}/{b}")
        else:
            op = "-" if rng.random() < 0.5 else "+"
            parts.append(f"{op}{a}/{b}")
    return "".join(parts) + "\n"

def main():
    compile_all()
    rng = random.Random(20261018)

    # 1. 随机表达式对拍(逐步约分版 vs 公分母积版)
    for tc in range(300):
        inp = gen(rng)
        o1 = run(STD, inp)
        o2 = run(BRU, inp)
        assert o1 == o2, f"Mismatch on:\n{inp}\nstd={o1}, bf={o2}"

    # 2. 手工边界(期望值为手工精确推得)
    edge = [
        ("1/2+1/3\n", "5/6"),
        ("1/2-1/3\n", "1/6"),
        ("-1/2-1/3\n", "-5/6"),
        ("3/6\n", "1/2"),
        ("4/2\n", "2"),
        ("1/2+1/2\n", "1"),
        ("-3/4+3/4\n", "0"),
        ("1/2+1/3+1/6\n", "1"),
        ("0/7\n", "0"),
        ("-1/3+2/3\n", "1/3"),
    ]
    for inp, exp in edge:
        got = run(STD, inp)
        assert got == exp, f"Edge mismatch {inp!r}: got={got}, exp={exp}"
        assert run(BRU, inp) == exp, f"Edge brute mismatch {inp!r}"

    # 3. 最坏长度(~100 字符)计时
    terms = []
    for t in range(11):
        op = "+" if t % 2 == 0 else "-"
        terms.append(f"{op}999/1000" if t else "999/1000")
    big = "".join(terms)
    big = big[:99] if len(big) > 99 else big
    if not big[0].isdigit() and big[0] != '-':
        big = "1/2" + big
    t0 = time.perf_counter()
    res = run(STD, big)
    dt = time.perf_counter() - t0

    print(f"P1572 check passed! 300 random + 10 edge cases; "
          f"len={len(big)} expr -> {res} in {dt:.4f}s")

if __name__ == "__main__":
    main()
