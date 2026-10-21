import subprocess, random, time

def compile_all():
    subprocess.run(["g++", "-O2", "-std=c++17", "P1558.cpp", "-o", "P1558"], check=True)
    subprocess.run(["g++", "-O2", "-std=c++17", "P1558_brute.cpp", "-o", "P1558_brute"], check=True)

def run(bin_name, inp):
    p = subprocess.run([f"./{bin_name}"], input=inp, text=True, capture_output=True, check=True)
    return p.stdout.strip()

def main():
    compile_all()
    # 1. 官方样例
    sample = "2 2 4\nC 1 1 2\nP 1 2\nC 2 2 2\nP 1 2\n"
    assert run("P1558", sample) == "2\n1"
    assert run("P1558_brute", sample) == "2\n1"

    # 2. 随机对拍 300 组 (含 A > B 逆序输入)
    rng = random.Random(20261021)
    for _ in range(300):
        L = rng.randint(1, 200)
        T = rng.randint(1, 30)
        O = rng.randint(1, 200)
        lines = [f"{L} {T} {O}"]
        for _ in range(O):
            a = rng.randint(1, L)
            b = rng.randint(1, L)
            if rng.random() < 0.5:
                c = rng.randint(1, T)
                lines.append(f"C {a} {b} {c}")
            else:
                lines.append(f"P {a} {b}")
        inp = "\n".join(lines) + "\n"
        assert run("P1558", inp) == run("P1558_brute", inp)

    # 3. 极限规模 L = 100000, T = 30, O = 100000
    L, T, O = 100000, 30, 100000
    lines = [f"{L} {T} {O}"]
    for i in range(O):
        a = rng.randint(1, L)
        b = rng.randint(1, L)
        if i % 2 == 0:
            lines.append(f"C {a} {b} {(i % T) + 1}")
        else:
            lines.append(f"P {a} {b}")
    max_inp = "\n".join(lines) + "\n"
    t0 = time.perf_counter()
    run("P1558", max_inp)
    dt = time.perf_counter() - t0
    print(f"P1558 check passed! Sample + 300 random cases + L=O=100,000 in {dt:.4f}s")

if __name__ == "__main__":
    main()
