import subprocess, random, time

def compile_all():
    subprocess.run(["g++", "-O2", "-std=c++17", "P1584.cpp", "-o", "P1584"], check=True)
    subprocess.run(["g++", "-O2", "-std=c++17", "P1584_brute.cpp", "-o", "P1584_brute"], check=True)

def run(bin_name, inp):
    p = subprocess.run([f"./{bin_name}"], input=inp, text=True, capture_output=True, check=True)
    return p.stdout.strip()

def main():
    compile_all()
    # 1. 随机对拍 400 组
    rng = random.Random(20261021)
    for _ in range(400):
        n = rng.randint(1, 35)
        l = [rng.randint(1, 20) for _ in range(n)]
        m = [rng.randint(1, 100) for _ in range(n)]
        low = rng.randint(1, sum(l))
        high = rng.randint(low, sum(l) + 10)
        inp = f"{n} {low} {high}\n" + " ".join(map(str, l)) + "\n" + " ".join(map(str, m)) + "\n"
        assert run("P1584", inp) == run("P1584_brute", inp)

    # 2. 极限规模测试 n = 1000
    n_max = 1000
    l = [rng.randint(1, 100000) for _ in range(n_max)]
    m = [rng.randint(1, 100000) for _ in range(n_max)]
    max_inp = f"{n_max} 50000 5000000\n" + " ".join(map(str, l)) + "\n" + " ".join(map(str, m)) + "\n"
    t0 = time.perf_counter()
    res = run("P1584", max_inp)
    dt = time.perf_counter() - t0
    print(f"P1584 check passed! 400 random cases + n=1000 (ans={res}) in {dt:.4f}s")

if __name__ == "__main__":
    main()
