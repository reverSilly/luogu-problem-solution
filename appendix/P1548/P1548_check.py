import subprocess, random, time, os

SRC = os.path.dirname(os.path.abspath(__file__))
TMP = "/tmp/P1548_bin"

def compile_all():
    os.makedirs(TMP, exist_ok=True)
    subprocess.run(["g++", "-O2", "-std=c++17", os.path.join(SRC, "P1548.cpp"),
                    "-o", os.path.join(TMP, "P1548")], check=True)
    subprocess.run(["g++", "-O2", "-std=c++17", os.path.join(SRC, "P1548_brute.cpp"),
                    "-o", os.path.join(TMP, "P1548_brute")], check=True)

def run(bin_name, inp):
    p = subprocess.run([os.path.join(TMP, bin_name)], input=inp, text=True,
                       capture_output=True, check=True)
    return p.stdout.strip()

def main():
    compile_all()
    rng = random.Random(20261018)
    # 1. 随机对拍
    for tc in range(300):
        n = rng.randint(1, 10)
        m = rng.randint(1, 10)
        inp = f"{n} {m}\n"
        out_std = run("P1548", inp)
        out_bf = run("P1548_brute", inp)
        assert out_std == out_bf, f"Mismatch on:\n{inp}\nstd={out_std}, bf={out_bf}"

    # 2. 题面样例 / 手工边界
    assert run("P1548", "2 3\n") == "8 10"           # 题面 N=2,M=3
    assert run("P1548", "1 1\n") == "1 0"
    assert run("P1548", "1 100\n") == "100 4950"     # 1 x M:仅 1x1 是正方形
    assert run("P1548", "100 100\n") == f"{sum(k*k for k in range(1,101))} {5050*5050-sum(k*k for k in range(1,101))}"

    # 3. 最坏规模计时
    t0 = time.perf_counter()
    res = run("P1548", "100 100\n")
    dt = time.perf_counter() - t0
    print(f"P1548 check PASS! 300 random + 4 hand cases; worst 100x100 ans={res} in {dt:.4f}s")

if __name__ == "__main__":
    main()
