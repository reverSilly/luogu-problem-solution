import subprocess, random, time

def compile_all():
    subprocess.run(["g++", "-O2", "-std=c++17", "P1564.cpp", "-o", "/tmp/P1564"], check=True)
    subprocess.run(["g++", "-O2", "-std=c++17", "P1564_brute.cpp", "-o", "/tmp/P1564_brute"], check=True)

def run(bin_name, inp):
    p = subprocess.run([bin_name], input=inp, text=True, capture_output=True, check=True)
    return p.stdout.strip()

def main():
    compile_all()
    rng = random.Random(20261018)
    # 1. 随机对拍: O(n^2) DP vs 枚举所有分段
    for tc in range(300):
        n = rng.randint(1, 10)
        m = rng.randint(0, 6)
        arr = [rng.randint(1, 2) for _ in range(n)]
        inp = f"{n} {m}\n" + "\n".join(map(str, arr)) + "\n"
        out_std = run("/tmp/P1564", inp)
        out_bf = run("/tmp/P1564_brute", inp)
        assert out_std == out_bf, f"Mismatch on:\n{inp}\nstd={out_std}, bf={out_bf}"

    # 2. 手工边界
    # 全同 -> 1 个机房
    assert run("/tmp/P1564", "5 0\n1\n1\n1\n1\n1\n") == "1"
    # m=0 时两派人数必须相等(或全同); 1,2,1 => 至少 2
    assert run("/tmp/P1564", "3 0\n1\n2\n1\n") == "2"
    # m 很大 -> 可整段当成一个机房(差<=m)
    assert run("/tmp/P1564", "4 2500\n1\n2\n1\n2\n") == "1"

    # 3. 最坏规模计时: n=2500, m=2500
    n = 2500
    arr = [rng.randint(1, 2) for _ in range(n)]
    max_inp = f"{n} 2500\n" + "\n".join(map(str, arr)) + "\n"
    t0 = time.perf_counter()
    res = run("/tmp/P1564", max_inp)
    dt = time.perf_counter() - t0
    print(f"PASS P1564! 300 random cases + hand cases + n=2500 worst-case (ans={res}) in {dt:.4f}s")

if __name__ == "__main__":
    main()
