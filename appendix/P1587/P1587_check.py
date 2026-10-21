import subprocess, random, time

def compile_all():
    subprocess.run(["g++", "-O2", "-std=c++17", "P1587.cpp", "-o", "P1587"], check=True)
    subprocess.run(["g++", "-O2", "-std=c++17", "P1587_brute.cpp", "-o", "P1587_brute"], check=True)

def run(bin_name, inp):
    p = subprocess.run([f"./{bin_name}"], input=inp, text=True, capture_output=True, check=True)
    return p.stdout.strip()

def main():
    compile_all()
    # 1. NOI 2016 官方样例 1
    assert run("P1587", "2 6 10\n") == "4"
    assert run("P1587_brute", "2 6 10\n") == "4"

    # 2. 随机对拍 200 组
    rng = random.Random(20261021)
    for _ in range(200):
        n = rng.randint(1, 300)
        m = rng.randint(1, 300)
        k = rng.randint(2, 2000)
        inp = f"{n} {m} {k}\n"
        assert run("P1587", inp) == run("P1587_brute", inp)

    # 3. 极限规模测试 n = 10^9, m = 10^9, k = 1998
    t0 = time.perf_counter()
    res = run("P1587", "1000000000 1000000000 1998\n")
    dt = time.perf_counter() - t0
    print(f"P1587 check passed! Sample + 200 random cases + n=m=10^9 (ans={res}) in {dt:.4f}s")

if __name__ == "__main__":
    main()
