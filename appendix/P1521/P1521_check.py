import subprocess, random, time, os

TMP = "/tmp/p1521_check"
os.makedirs(TMP, exist_ok=True)
STD = os.path.join(TMP, "std")
BF = os.path.join(TMP, "bf")

def compile_all():
    subprocess.run(["g++", "-O2", "-std=c++17", "P1521.cpp", "-o", STD], check=True)
    subprocess.run(["g++", "-O2", "-std=c++17", "P1521_brute.cpp", "-o", BF], check=True)

def run(bin_name, inp):
    p = subprocess.run([bin_name], input=inp, text=True, capture_output=True, check=True)
    return p.stdout.strip()

def main():
    compile_all()
    rng = random.Random(20261018)
    # 1. 随机对拍 (含 K 超出最大逆序对数 的情形)
    for tc in range(300):
        n = rng.randint(1, 7)
        maxk = n * (n - 1) // 2
        k = rng.randint(0, maxk + 2)
        inp = f"{n} {k}\n"
        o1 = run(STD, inp)
        o2 = run(BF, inp)
        assert o1 == o2, f"Mismatch:\n{inp}std={o1}, bf={o2}"

    # 2. 手工边界用例
    for n, k, exp in [(1, 0, 1), (5, 3, 15), (2, 1, 1), (2, 0, 1),
                      (3, 3, 1), (100, 0, 1), (7, 21, 1)]:
        o1 = run(STD, f"{n} {k}\n")
        assert o1 == str(exp), f"Edge fail n={n},k={k}: std={o1}, exp={exp}"

    # 3. 最坏规模计时 N=100, K=4950
    t0 = time.perf_counter()
    res = run(STD, "100 4950\n")
    dt = time.perf_counter() - t0
    print(f"P1521 check PASS! 300 random cases matched brute; N=100,K=4950 ans={res} in {dt:.4f}s")

if __name__ == "__main__":
    main()
