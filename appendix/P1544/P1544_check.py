import subprocess, random, time

def compile_all():
    subprocess.run(["g++", "-O2", "-std=c++17", "P1544.cpp", "-o", "P1544"], check=True)
    subprocess.run(["g++", "-O2", "-std=c++17", "P1544_brute.cpp", "-o", "P1544_brute"], check=True)

def run(bin_name, inp):
    p = subprocess.run([f"./{bin_name}"], input=inp, text=True, capture_output=True, check=True)
    return p.stdout.strip()

def gen(rng, n, k, lo, hi):
    lines = [f"{n} {k}"]
    for i in range(1, n + 1):
        lines.append(" ".join(str(rng.randint(lo, hi)) for _ in range(i)))
    return "\n".join(lines) + "\n"

def main():
    compile_all()

    # 1. 官方样例
    sample = "5 3\n7\n3 9\n8 1 0\n2 7 4 4\n4 5 2 6 5\n"
    assert run("P1544", sample) == "75"
    assert run("P1544_brute", sample) == "75"

    # 2. 随机对拍 400 组 (含负数、k=0 与 k 极大两类极端)
    rng = random.Random(20261018)
    for _ in range(400):
        n = rng.randint(1, 8)
        mx = n * (n + 1) // 2
        k = rng.choice([0, rng.randint(0, mx), mx, rng.randint(0, n)])
        inp = gen(rng, n, k, -100, 100)
        a = run("P1544", inp)
        b = run("P1544_brute", inp)
        assert a == b, f"Mismatch on:\n{inp}\nstd={a}, bf={b}"

    # 3. 手工边界:全负数, k 很大 => 三倍只会让负数更小, 故答案是不加倍的路径最大和 -9
    neg = "3 6\n-5\n-3 -9\n-8 -1 -2\n"
    assert run("P1544", neg) == run("P1544_brute", neg) == "-9"

    # 4. 最坏规模 n=100, k=5050, |a|<=1e9
    inp = gen(rng, 100, 5050, -1000000000, 1000000000)
    t0 = time.perf_counter()
    res = run("P1544", inp)
    dt = time.perf_counter() - t0
    print(f"P1544 check passed! Sample + 400 random + edge + max(n=100,k=5050,ans={res}) in {dt:.4f}s")

if __name__ == "__main__":
    main()
