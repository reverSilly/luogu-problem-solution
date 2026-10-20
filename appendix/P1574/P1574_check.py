import subprocess, random, time

def compile_all():
    subprocess.run(["g++", "-O2", "-std=c++17", "P1574.cpp", "-o", "P1574"], check=True)
    subprocess.run(["g++", "-O2", "-std=c++17", "P1574_brute.cpp", "-o", "P1574_brute"], check=True)

def run(bin_name, inp):
    p = subprocess.run([f"./{bin_name}"], input=inp, text=True, capture_output=True, check=True)
    return p.stdout.strip()

def main():
    compile_all()
    rng = random.Random(20261018)

    # 1. 随机对拍：a_i in [1,1e6]，暴力筛法 vs 公式
    for tc in range(300):
        m = rng.randint(1, 20)
        qs = [rng.randint(1, 1000000) for _ in range(m)]
        inp = f"{m}\n" + "\n".join(map(str, qs)) + "\n"
        out_std = run("P1574", inp)
        out_bf = run("P1574_brute", inp)
        assert out_std == out_bf, f"Mismatch:\n{inp}\nstd={out_std}\nbf={out_bf}"

    # 2. 边界：查询恰好等于若干 supernumber 值及其 ±1
    base = [1, 2, 4, 6, 12, 24, 36, 48, 60, 120, 180, 240, 360, 720, 840]
    qs = []
    for v in base:
        qs += [max(1, v - 1), v, v + 1]
    inp = f"{len(qs)}\n" + "\n".join(map(str, qs)) + "\n"
    out_std = run("P1574", inp)
    out_bf = run("P1574_brute", inp)
    assert out_std == out_bf, f"Mismatch on boundary:\n{inp}\nstd={out_std}\nbf={out_bf}"

    # 2b. a_i=1 的最小边界与 a_i=1e6 的最大边界
    inp = "2\n1\n1000000\n"
    assert run("P1574", inp) == run("P1574_brute", inp), "boundary 1e6 mismatch"

    # 3. 最坏规模计时：m=1e5 全查询 1e17
    m = 100000
    inp = f"{m}\n" + "\n".join(["100000000000000000"] * m) + "\n"
    t0 = time.perf_counter()
    res = run("P1574", inp)
    dt = time.perf_counter() - t0
    last_line = res.splitlines()[-1]

    # 4. 单独测一个"巨值精确命中"的答案，打印便于人工核对
    print(f"P1574 check passed! 300 random (a<=1e6) + boundary tests all std==brute; "
          f"m=1e5,a=1e17 -> ans={last_line} in {dt:.4f}s")

if __name__ == "__main__":
    main()
