import subprocess, random, time

def compile_all():
    subprocess.run(["g++", "-O2", "-std=c++17", "P1559.cpp", "-o", "P1559"], check=True)
    subprocess.run(["g++", "-O2", "-std=c++17", "P1559_brute.cpp", "-o", "P1559_brute"], check=True)

def run(bin_name, inp):
    p = subprocess.run([f"./{bin_name}"], input=inp, text=True, capture_output=True, check=True)
    return p.stdout.strip()

def main():
    compile_all()
    # 1. 官方样例
    sample = "3\n10 2 3\n2 3 4\n3 4 5\n2 2 2\n3 5 3\n4 5 1\n"
    assert run("P1559", sample) == "52"
    assert run("P1559_brute", sample) == "52"

    # 2. 随机对拍 500 组 (n in 1..14)
    rng = random.Random(20261021)
    for _ in range(500):
        n = rng.randint(1, 12)
        lines = [str(n)]
        for _ in range(2 * n):
            lines.append(" ".join(str(rng.randint(0, 100)) for _ in range(n)))
        inp = "\n".join(lines) + "\n"
        assert run("P1559", inp) == run("P1559_brute", inp)

    # 3. 极限规模 n = 20 与状压 DP 核对并测速
    lines = ["20"]
    for _ in range(40):
        lines.append(" ".join(str(rng.randint(1, 100)) for _ in range(20)))
    max_inp = "\n".join(lines) + "\n"
    t0 = time.perf_counter()
    res = run("P1559", max_inp)
    dt = time.perf_counter() - t0
    assert res == run("P1559_brute", max_inp)
    print(f"P1559 check passed! Sample + 500 random cases + n=20 (ans={res}) in {dt:.4f}s")

if __name__ == "__main__":
    main()
