import subprocess, random, time

def compile_all():
    subprocess.run(["g++", "-O2", "-std=c++17", "P1539.cpp", "-o", "P1539"], check=True)
    subprocess.run(["g++", "-O2", "-std=c++17", "P1539_brute.cpp", "-o", "P1539_brute"], check=True)

def run(bin_name, inp):
    p = subprocess.run([f"./{bin_name}"], input=inp, text=True, capture_output=True, check=True)
    return p.stdout.strip()

def main():
    compile_all()
    rng = random.Random(20261018)
    # 1. 边界与随机对拍 (含 n < m 需要转置的情形)
    for tc in range(300):
        n = rng.randint(1, 6)
        m = rng.randint(1, 6)
        if tc % 3 == 0:
            n, m = 2, 10
        elif tc % 3 == 1:
            n, m = 10, 2
        grid = []
        for _ in range(n):
            row = "".join(rng.choices([".", "0", "1"], weights=[7, 2, 1], k=m))
            grid.append(row)
        inp = f"{n} {m}\n" + "\n".join(grid) + "\n"
        out_std = run("P1539", inp)
        out_bf = run("P1539_brute", inp)
        assert out_std == out_bf, f"Mismatch on:\n{inp}\nstd={out_std}, bf={out_bf}"

    # 2. 极限规模测试 n=15, m=15 全 '.'
    max_inp = "15 15\n" + "\n".join(["." * 15 for _ in range(15)]) + "\n"
    t0 = time.perf_counter()
    res = run("P1539", max_inp)
    dt = time.perf_counter() - t0
    print(f"P1539 check passed! 300 random cases + 15x15 worst-case (ans={res}) in {dt:.4f}s")

if __name__ == "__main__":
    main()
