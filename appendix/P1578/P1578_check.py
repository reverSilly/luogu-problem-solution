import subprocess, random, time

def compile_all():
    subprocess.run(["g++", "-O2", "-std=c++17", "P1578.cpp", "-o", "P1578"], check=True)
    subprocess.run(["g++", "-O2", "-std=c++17", "P1578_brute.cpp", "-o", "P1578_brute"], check=True)

def run(bin_name, inp):
    p = subprocess.run([f"./{bin_name}"], input=inp, text=True, capture_output=True, check=True)
    return p.stdout.strip()

def main():
    compile_all()
    # 1. WC2002 官方样例 (10x10, 4 points -> 40)
    sample = "10 10\n4\n1 1\n9 1\n1 9\n9 9\n"
    assert run("P1578", sample) == "80"
    assert run("P1578_brute", sample) == "80"

    # 2. 随机与共线 Hack 数据对拍 400 组 (含 n=0, 重复点, 同一行/列多点)
    rng = random.Random(20261021)
    for tc in range(400):
        L = rng.randint(1, 50)
        W = rng.randint(1, 50)
        n = 0 if tc == 0 else rng.randint(1, 25)
        pts = [(rng.randint(0, L), rng.randint(0, W)) for _ in range(n)]
        inp = f"{L} {W}\n{n}\n" + "\n".join(f"{x} {y}" for x, y in pts) + "\n"
        out_std = run("P1578", inp)
        out_bf = run("P1578_brute", inp)
        assert out_std == out_bf, f"Mismatch on:\n{inp}\nstd={out_std}, bf={out_bf}"

    # 3. 极限规模 L = W = 30000, n = 5000
    n_max = 5000
    pts = [(rng.randint(0, 30000), rng.randint(0, 30000)) for _ in range(n_max)]
    max_inp = f"30000 30000\n{n_max}\n" + "\n".join(f"{x} {y}" for x, y in pts) + "\n"
    t0 = time.perf_counter()
    res = run("P1578", max_inp)
    dt = time.perf_counter() - t0
    print(f"P1578 check passed! Sample + 400 random/collinear cases + n=5000 (ans={res}) in {dt:.4f}s")

if __name__ == "__main__":
    main()
