import subprocess, random, time

def compile_all():
    subprocess.run(["g++", "-O2", "-std=c++17", "P1556.cpp", "-o", "P1556"], check=True)
    subprocess.run(["g++", "-O2", "-std=c++17", "P1556_brute.cpp", "-o", "P1556_brute"], check=True)

def run(bin_name, inp):
    p = subprocess.run([f"./{bin_name}"], input=inp, text=True, capture_output=True, check=True)
    return p.stdout.strip()

def main():
    compile_all()
    # 1. USACO12MAR 官方样例
    sample = "4\n0 1\n2 1\n2 0\n2 -5\n"
    assert run("P1556", sample) == "2"
    assert run("P1556_brute", sample) == "2"

    # 2. 随机对拍 500 组 (n <= 8)
    rng = random.Random(20261021)
    for _ in range(500):
        n = rng.randint(1, 8)
        pts = set([(0, 0)])
        coords = []
        while len(coords) < n:
            if rng.random() < 0.7 and coords:
                bx, by = rng.choice(coords + [(0, 0)])
                pt = (bx, rng.randint(-3, 3)) if rng.random() < 0.5 else (rng.randint(-3, 3), by)
            else:
                pt = (rng.randint(-3, 3), rng.randint(-3, 3))
            if pt not in pts:
                pts.add(pt)
                coords.append(pt)
        inp = f"{n}\n" + "\n".join(f"{x} {y}" for x, y in coords) + "\n"
        assert run("P1556", inp) == run("P1556_brute", inp)

    # 3. 极限规模 n = 10
    max_inp = "10\n0 1\n1 1\n1 2\n2 2\n2 3\n3 3\n3 4\n4 4\n4 0\n0 4\n"
    t0 = time.perf_counter()
    res = run("P1556", max_inp)
    dt = time.perf_counter() - t0
    assert res == run("P1556_brute", max_inp)
    print(f"P1556 check passed! Sample + 500 random cases + n=10 (ans={res}) in {dt:.4f}s")

if __name__ == "__main__":
    main()
