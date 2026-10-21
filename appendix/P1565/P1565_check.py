import subprocess, random, time

def compile_all():
    subprocess.run(["g++", "-O2", "-std=c++17", "P1565.cpp", "-o", "P1565"], check=True)
    subprocess.run(["g++", "-O2", "-std=c++17", "P1565_brute.cpp", "-o", "P1565_brute"], check=True)

def run(bin_name, inp):
    p = subprocess.run([f"./{bin_name}"], input=inp, text=True, capture_output=True, check=True)
    return p.stdout.strip()

def main():
    compile_all()
    rng = random.Random(20261021)
    # 1. 随机网格对拍 300 组 (含全负数边界、大负数 -2e9 溢出测试)
    for tc in range(300):
        n = rng.randint(1, 18)
        m = rng.randint(1, 18)
        lines = [f"{n} {m}"]
        for _ in range(n):
            if tc % 10 == 0:
                row = [rng.randint(-2000000000, -1) for _ in range(m)]
            elif tc % 10 == 1:
                row = [rng.choice([-2000000000, 16315]) for _ in range(m)]
            else:
                row = [rng.randint(-50, 30) for _ in range(m)]
            lines.append(" ".join(map(str, row)))
        inp = "\n".join(lines) + "\n"
        assert run("P1565", inp) == run("P1565_brute", inp)

    # 2. 极限规模测试 N = M = 200
    lines = ["200 200"]
    for _ in range(200):
        lines.append(" ".join(str(rng.randint(-20000, 16315)) for _ in range(200)))
    max_inp = "\n".join(lines) + "\n"
    t0 = time.perf_counter()
    res = run("P1565", max_inp)
    dt = time.perf_counter() - t0
    print(f"P1565 check passed! 300 random grids + 200x200 worst-case (ans={res}) in {dt:.4f}s")

if __name__ == "__main__":
    main()
