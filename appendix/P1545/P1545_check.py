import subprocess, random, time

def compile_all():
    subprocess.run(["g++", "-O2", "-std=c++17", "P1545.cpp", "-o", "P1545"], check=True)
    subprocess.run(["g++", "-O2", "-std=c++17", "P1545_brute.cpp", "-o", "P1545_brute"], check=True)

def run(bin_name, inp):
    p = subprocess.run([f"./{bin_name}"], input=inp, text=True, capture_output=True, check=True)
    return p.stdout.strip()

def main():
    compile_all()
    # 1. 官方样例 (USACO 04DEC Dividing the Path)
    sample = "2 8\n1 2\n6 7\n3 6\n"
    assert run("P1545", sample) == "3"
    assert run("P1545_brute", sample) == "3"

    # 2. 随机对拍 500 组
    rng = random.Random(20261018)
    for _ in range(500):
        L = rng.randint(1, 200) * 2
        A = rng.randint(1, 20)
        B = rng.randint(A, 35)
        n = rng.randint(0, 15)
        cows = []
        for _ in range(n):
            s = rng.randint(1, L - 1)
            e = rng.randint(s + 1, L)
            cows.append(f"{s} {e}")
        inp = f"{n} {L}\n{A} {B}\n" + "\n".join(cows) + "\n"
        assert run("P1545", inp) == run("P1545_brute", inp)

    # 3. 极限规模测试 L = 1,000,000, N = 1000
    L_max = 1000000
    cows = [f"{i*900+10} {i*900+50}" for i in range(1000)]
    max_inp = f"1000 {L_max}\n100 500\n" + "\n".join(cows) + "\n"
    t0 = time.perf_counter()
    res = run("P1545", max_inp)
    dt = time.perf_counter() - t0
    print(f"P1545 check passed! Sample + 500 random cases + L=1,000,000 (ans={res}) in {dt:.4f}s")

if __name__ == "__main__":
    main()
