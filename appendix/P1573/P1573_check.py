import subprocess, random, time

def compile_all():
    subprocess.run(["g++", "-O2", "-std=c++17", "P1573.cpp", "-o", "P1573"], check=True)
    subprocess.run(["g++", "-O2", "-std=c++17", "P1573_brute.cpp", "-o", "P1573_brute"], check=True)

def run(bin_name, inp):
    p = subprocess.run([f"./{bin_name}"], input=inp, text=True, capture_output=True, check=True)
    return p.stdout.strip()

def main():
    compile_all()
    rng = random.Random(20261018)

    # 1. 随机对拍：n in [1,8]，BFS 暴力 vs 公式
    for tc in range(300):
        n = rng.randint(1, 8)
        inp = f"{n}\n"
        out_std = run("P1573", inp)
        out_bf = run("P1573_brute", inp)
        assert out_std == out_bf, f"Mismatch n={n}: std={out_std}, bf={out_bf}"

    # 2. 逐点扫描 n=1..10，覆盖公式分段边界(t 跳变处)
    for n in range(1, 11):
        inp = f"{n}\n"
        out_std = run("P1573", inp)
        out_bf = run("P1573_brute", inp)
        assert out_std == out_bf, f"Mismatch n={n}: std={out_std}, bf={out_bf}"

    # 3. 最坏规模计时 n=2e9
    inp = "2000000000\n"
    t0 = time.perf_counter()
    res = run("P1573", inp)
    dt = time.perf_counter() - t0

    print(f"P1573 check passed! 300 random (n<=8) + n=1..10 all std==brute; "
          f"n=2e9 -> {res} in {dt:.4f}s")

if __name__ == "__main__":
    main()
