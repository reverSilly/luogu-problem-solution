import subprocess, random, time

def compile_all():
    subprocess.run(["g++", "-O2", "-std=c++17", "P1541.cpp", "-o", "P1541"], check=True)
    subprocess.run(["g++", "-O2", "-std=c++17", "P1541_brute.cpp", "-o", "P1541_brute"], check=True)

def run(bin_name, inp):
    p = subprocess.run([f"./{bin_name}"], input=inp, text=True, capture_output=True, check=True)
    return p.stdout.strip()

def main():
    compile_all()
    # 1. 官方样例
    sample = "9 5\n6 10 14 2 8 8 18 5 17\n1 3 1 2 1\n"
    assert run("P1541", sample) == "73"
    assert run("P1541_brute", sample) == "73"

    # 2. 随机对拍 300 组
    rng = random.Random(20261018)
    for _ in range(300):
        m = rng.randint(1, 25)
        cards = [rng.randint(1, 4) for _ in range(m)]
        n = sum(cards) + 1
        a = [rng.randint(0, 100) for _ in range(n)]
        inp = f"{n} {m}\n" + " ".join(map(str, a)) + "\n" + " ".join(map(str, cards)) + "\n"
        assert run("P1541", inp) == run("P1541_brute", inp)

    # 3. 极限规模 c1=c2=c3=c4=30 (m=120, n=301)
    cards = [1]*30 + [2]*30 + [3]*30 + [4]*30
    n = sum(cards) + 1
    a = [rng.randint(0, 100) for _ in range(n)]
    max_inp = f"{n} {len(cards)}\n" + " ".join(map(str, a)) + "\n" + " ".join(map(str, cards)) + "\n"
    t0 = time.perf_counter()
    run("P1541", max_inp)
    dt = time.perf_counter() - t0
    print(f"P1541 check passed! Sample + 300 random cases + max-size in {dt:.4f}s")

if __name__ == "__main__":
    main()
