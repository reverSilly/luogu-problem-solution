import subprocess, random, time

def compile_all():
    subprocess.run(["g++", "-O2", "-std=c++17", "P1600.cpp", "-o", "P1600"], check=True)
    subprocess.run(["g++", "-O2", "-std=c++17", "P1600_brute.cpp", "-o", "P1600_brute"], check=True)

def run(bin_name, inp):
    p = subprocess.run([f"./{bin_name}"], input=inp, text=True, capture_output=True, check=True)
    return p.stdout.strip()

def main():
    compile_all()
    # 1. NOIP 2016 官方样例
    sample = "6 3\n2 3\n1 2\n1 4\n4 5\n4 6\n0 2 5 1 2 3\n1 5\n1 3\n2 6\n"
    assert run("P1600", sample) == "2 0 0 1 1 1"
    assert run("P1600_brute", sample) == "2 0 0 1 1 1"

    # 2. 随机树与链对拍 300 组
    rng = random.Random(20261021)
    for tc in range(300):
        n = rng.randint(1, 80)
        m = rng.randint(1, 80)
        edges = []
        for i in range(2, n + 1):
            p = i - 1 if tc % 3 == 0 else rng.randint(1, i - 1)
            edges.append(f"{p} {i}")
        w = [rng.randint(0, min(n, 15)) for _ in range(n)]
        players = [f"{rng.randint(1, n)} {rng.randint(1, n)}" for _ in range(m)]
        inp = f"{n} {m}\n" + ("\n".join(edges) + "\n" if edges else "") + " ".join(map(str, w)) + "\n" + "\n".join(players) + "\n"
        out_std = run("P1600", inp)
        out_bf = run("P1600_brute", inp)
        assert out_std == out_bf, f"Mismatch on:\n{inp}\nstd={out_std}\nbf={out_bf}"

    # 3. 极限退化长链测试 n = m = 299,998 (验证非递归栈安全与速度)
    n_max = 299998
    edges = [f"{i} {i+1}" for i in range(1, n_max)]
    w = [i % 50 for i in range(n_max)]
    players = [f"{rng.randint(1, n_max)} {rng.randint(1, n_max)}" for _ in range(n_max)]
    max_inp = f"{n_max} {n_max}\n" + "\n".join(edges) + "\n" + " ".join(map(str, w)) + "\n" + "\n".join(players) + "\n"
    t0 = time.perf_counter()
    run("P1600", max_inp)
    dt = time.perf_counter() - t0
    print(f"P1600 check passed! Sample + 300 random trees + 299,998 chain worst-case in {dt:.4f}s")

if __name__ == "__main__":
    main()
