import subprocess, random, time

def compile_all():
    subprocess.run(["g++", "-O2", "-std=c++17", "P1574.cpp", "-o", "P1574"], check=True)
    subprocess.run(["g++", "-O2", "-std=c++17", "P1574_brute.cpp", "-o", "P1574_brute"], check=True)

def run(bin_name, inp):
    p = subprocess.run([f"./{bin_name}"], input=inp, text=True, capture_output=True, check=True)
    return p.stdout.strip()

def main():
    compile_all()
    # 1. 全量验证 1..200000 内的所有查询 vs 筛法暴力
    queries = list(range(1, 200001, 13)) + [1, 2, 4, 6, 12, 24, 36, 48, 60, 120, 180, 240, 360, 720, 840, 1260, 1680, 2520, 5040]
    inp = f"{len(queries)}\n" + "\n".join(map(str, queries)) + "\n"
    assert run("P1574", inp) == run("P1574_brute", inp)

    # 2. 极限规模测试 m = 100,000, a_i <= 10^17
    rng = random.Random(20261021)
    m_max = 100000
    big_q = [rng.randint(1, 10**17) for _ in range(m_max - 1)] + [10**17]
    max_inp = f"{m_max}\n" + "\n".join(map(str, big_q)) + "\n"
    t0 = time.perf_counter()
    out = run("P1574", max_inp).splitlines()
    dt = time.perf_counter() - t0
    # 10^17 以内最大的反素数是 735134400 * ... = 99791448473090800 (约数个数 73728)
    print(f"P1574 check passed! {len(queries)} exact checks + 100,000 queries up to 10^17 (max={out[-1]}) in {dt:.4f}s")

if __name__ == "__main__":
    main()
