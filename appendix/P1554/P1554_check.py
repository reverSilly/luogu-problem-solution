import subprocess, random, time

def compile_all():
    subprocess.run(["g++", "-O2", "-std=c++17", "P1554.cpp", "-o", "P1554"], check=True)
    subprocess.run(["g++", "-O2", "-std=c++17", "P1554_brute.cpp", "-o", "P1554_brute"], check=True)

def run(bin_name, inp):
    p = subprocess.run([f"./{bin_name}"], input=inp, text=True, capture_output=True, check=True)
    return p.stdout.strip()

# 与实现独立的 Python 参考: 逐个数逐位统计
def ref(m, n):
    c = [0] * 10
    for v in range(m, n + 1):
        for ch in str(v):
            c[ord(ch) - 48] += 1
    return ' '.join(map(str, c))

def main():
    compile_all()
    rng = random.Random(20261018)

    # 1. 手工边界
    manual = [(1, 1), (1, 9), (1, 10), (7, 7), (100, 100), (999999, 1000000), (1, 100)]
    for m, n in manual:
        exp = ref(m, n)
        a = run("P1554", f"{m} {n}\n")
        b = run("P1554_brute", f"{m} {n}\n")
        assert a == b == exp, f"m={m} n={n} std={a} brute={b} exp={exp}"

    # 2. 随机区间, 三方一致
    for _ in range(500):
        L = rng.randint(1, 3000)
        m = rng.randint(1, 2 * 10**9 - L)
        n = m + L
        exp = ref(m, n)
        a = run("P1554", f"{m} {n}\n")
        b = run("P1554_brute", f"{m} {n}\n")
        assert a == b == exp, f"m={m} n={n} std={a} brute={b} exp={exp}"

    # 3. 最坏规模: N-M = 5e5, 且数字全部是 10 位(靠近 2e9)
    m_max = 2 * 10**9 - 500000
    n_max = 2 * 10**9
    inp = f"{m_max} {n_max}\n"
    t0 = time.perf_counter()
    res = run("P1554", inp)
    dt = time.perf_counter() - t0

    print(f"P1554 check PASS! {len(manual)} manual + 500 random + worst N-M=5e5/10-digit "
          f"(std {dt:.4f}s, first digits: {res.split()[:3]})")

if __name__ == "__main__":
    main()
