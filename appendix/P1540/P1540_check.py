import subprocess, random, time

def compile_all():
    subprocess.run(["g++", "-O2", "-std=c++17", "P1540.cpp", "-o", "P1540"], check=True)
    subprocess.run(["g++", "-O2", "-std=c++17", "P1540_brute.cpp", "-o", "P1540_brute"], check=True)

def run(bin_name, inp):
    p = subprocess.run([f"./{bin_name}"], input=inp, text=True, capture_output=True, check=True)
    return p.stdout.strip()

def main():
    compile_all()

    # 1. 官方样例
    sample = "3 7\n1 2 1 5 4 4 1\n"
    assert run("P1540", sample) == "5"
    assert run("P1540_brute", sample) == "5"

    # 2. 随机对拍 400 组 (小 M 强制频繁换页, 编号范围小以制造重复)
    rng = random.Random(20261018)
    for _ in range(400):
        m = rng.randint(1, 6)
        n = rng.randint(1, 40)
        w = [rng.randint(0, 8) for _ in range(n)]
        inp = f"{m} {n}\n" + " ".join(map(str, w)) + "\n"
        a = run("P1540", inp)
        b = run("P1540_brute", inp)
        assert a == b, f"Mismatch on:\n{inp}\nstd={a}, bf={b}"

    # 3. 最坏规模 M=100, N=1000, 全部互不相同 => 每次都未命中
    m, n = 100, 1000
    w = list(range(n))               # 0..999 两两不同
    inp = f"{m} {n}\n" + " ".join(map(str, w)) + "\n"
    t0 = time.perf_counter()
    res = run("P1540", inp)
    dt = time.perf_counter() - t0
    assert res == "1000"
    print(f"P1540 check passed! Sample + 400 random cases + max(M=100,N=1000,ans={res}) in {dt:.4f}s")

if __name__ == "__main__":
    main()
