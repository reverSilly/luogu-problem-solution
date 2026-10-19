import subprocess, random, time, os

TMP = "/tmp/p1532_check"
os.makedirs(TMP, exist_ok=True)
STD = os.path.join(TMP, "std")
BF = os.path.join(TMP, "bf")

def compile_all():
    subprocess.run(["g++", "-O2", "-std=c++17", "P1532.cpp", "-o", STD], check=True)
    subprocess.run(["g++", "-O2", "-std=c++17", "P1532_brute.cpp", "-o", BF], check=True)

def run(bin_name, inp):
    p = subprocess.run([bin_name], input=inp, text=True, capture_output=True, check=True)
    return p.stdout.strip()

def main():
    compile_all()
    rng = random.Random(20261018)

    # 1. 随机对拍:多行输入,行数不定,位数 1..10
    for tc in range(300):
        lines = []
        cnt = rng.randint(1, 5)
        for _ in range(cnt):
            k = rng.randint(1, 10)
            if rng.random() < 0.25:
                # 各位相同 -> 收敛到 0
                dd = rng.randint(0, 9)
                n = int(str(dd) * k)
            else:
                n = rng.randint(0, 10 ** k - 1)
            lines.append(str(n))
        inp = "\n".join(lines) + "\n"
        o1 = run(STD, inp)
        o2 = run(BF, inp)
        assert o1 == o2, f"Mismatch:\n{inp}std={o1}, bf={o2}"

    # 2. 官方样例
    sample = "4321\n54321\n"
    got = run(STD, sample)
    assert got == "6174\n82962 75933 63954 61974", f"sample got {got!r}"

    # 3. 手工边界
    assert run(STD, "1111\n") == "0", run(STD, "1111\n")
    assert run(STD, "0\n") == "0", run(STD, "0\n")
    assert run(STD, "1\n") == "0", run(STD, "1\n")

    # 4. 最坏规模计时:50000 行 10 位数
    pool = random.Random(7)
    big = "\n".join(str(pool.randint(0, 2147483646)) for _ in range(50000)) + "\n"
    t0 = time.perf_counter()
    run(STD, big)
    dt = time.perf_counter() - t0
    print(f"P1532 check PASS! 300 random cases matched brute; 50000 lines x 10-digit in {dt:.4f}s")

if __name__ == "__main__":
    main()
