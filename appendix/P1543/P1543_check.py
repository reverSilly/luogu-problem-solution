import subprocess, random, time

def compile_all():
    subprocess.run(["g++", "-O2", "-std=c++17", "P1543.cpp", "-o", "P1543"], check=True)
    subprocess.run(["g++", "-O2", "-std=c++17", "P1543_brute.cpp", "-o", "P1543_brute"], check=True)

def run(bin_name, inp):
    p = subprocess.run([f"./{bin_name}"], input=inp, text=True, capture_output=True, check=True)
    return p.stdout.strip()

def main():
    compile_all()
    rng = random.Random(20261018)
    # 1. 随机基环森林对拍 2000 组 (n in 2..16)
    for _ in range(2000):
        n = rng.randint(2, 16)
        a = []
        for i in range(1, n + 1):
            choices = [x for x in range(1, n + 1) if x != i]
            a.append(rng.choice(choices))
        inp = f"{n}\n" + "\n".join(map(str, a)) + "\n"
        out_std = run("P1543", inp)
        out_bf = run("P1543_brute", inp)
        assert out_std == out_bf, f"Mismatch on:\n{inp}\nstd={out_std}, bf={out_bf}"

    # 2. 极限规模测试 n = 1,000,000
    n_max = 1000000
    a_max = [(i % n_max) + 1 if rng.random() < 0.3 else rng.randint(1, i if i > 1 else 2) for i in range(1, n_max + 1)]
    for i in range(n_max):
        if a_max[i] == i + 1:
            a_max[i] = 1 if i > 0 else 2
    max_inp = f"{n_max}\n" + "\n".join(map(str, a_max)) + "\n"
    t0 = time.perf_counter()
    res = run("P1543", max_inp)
    dt = time.perf_counter() - t0
    print(f"P1543 check passed! 2000 random graphs + n=1,000,000 (ans={res}) in {dt:.4f}s")

if __name__ == "__main__":
    main()
