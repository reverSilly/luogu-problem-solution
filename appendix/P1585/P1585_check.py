import subprocess, random, time

def compile_all():
    subprocess.run(["g++", "-O2", "-std=c++17", "P1585.cpp", "-o", "P1585"], check=True)
    subprocess.run(["g++", "-O2", "-std=c++17", "P1585_brute.cpp", "-o", "P1585_brute"], check=True)

def run(bin_name, inp):
    p = subprocess.run([f"./{bin_name}"], input=inp, text=True, capture_output=True, check=True)
    return p.stdout.strip()

def main():
    compile_all()
    rng = random.Random(20261021)
    # 1. 小规模网格与无剪枝全搜索对拍 (n * m <= 24)
    dims = [(1, 4), (1, 6), (2, 2), (2, 3), (2, 4), (2, 5), (2, 6), (3, 2), (3, 4), (4, 2), (4, 3), (4, 4), (4, 5), (5, 4), (4, 6), (6, 4)]
    for n, m in dims:
        for _ in range(5):
            k1 = rng.randint(1, 32767)
            k2 = rng.randint(1, 32767)
            inp = f"{n} {m} {k1} {k2}\n"
            assert run("P1585", inp) == run("P1585_brute", inp)

    # 2. 满规模 50 格网格极限性能测试 (5x10, 10x5, 2x25, 25x2)
    t0 = time.perf_counter()
    for n, m in [(5, 10), (10, 5), (2, 25), (25, 2), (6, 8), (8, 6)]:
        run("P1585", f"{n} {m} 12345 23456\n")
    dt = time.perf_counter() - t0
    print(f"P1585 check passed! {len(dims)*5} exact checks + 6 max-size (n*m<=50) cases in {dt:.4f}s")

if __name__ == "__main__":
    main()
