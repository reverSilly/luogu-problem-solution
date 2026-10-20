import subprocess, random, time

def compile_all():
    subprocess.run(["g++", "-O2", "-std=c++17", "P1561.cpp", "-o", "/tmp/P1561"], check=True)
    subprocess.run(["g++", "-O2", "-std=c++17", "P1561_brute.cpp", "-o", "/tmp/P1561_brute"], check=True)

def run(bin_name, inp):
    p = subprocess.run([bin_name], input=inp, text=True, capture_output=True, check=True)
    return p.stdout.strip()

def main():
    compile_all()
    rng = random.Random(20261018)
    # 1. 小规模随机对拍: 枚举上山/下山全排列的暴力 vs 贪心公式
    for tc in range(300):
        n = rng.randint(1, 6)
        lines = [str(n)]
        for _ in range(n):
            lines.append(f"{rng.randint(1, 50)} {rng.randint(1, 50)}")
        inp = "\n".join(lines) + "\n"
        out_std = run("/tmp/P1561", inp)
        out_bf = run("/tmp/P1561_brute", inp)
        assert out_std == out_bf, f"Mismatch on:\n{inp}\nstd={out_std}, bf={out_bf}"

    # 2. 手工边界: 两头奶牛 (1,10)/(10,1) -> 12; 全部相等 (5,5)/(5,5) -> 15
    for inp, exp in [("2\n1 10\n10 1\n", "12"), ("2\n5 5\n5 5\n", "15"),
                     ("1\n1 1\n", "2"), ("3\n3 3\n3 3\n3 3\n", "12")]:
        got = run("/tmp/P1561", inp)
        assert got == exp, f"hand case {inp!r}: got {got} exp {exp}"

    # 3. 最坏规模计时: n=25000, U=D=50000
    n = 25000
    lines = [str(n)] + ["50000 50000"] * n
    max_inp = "\n".join(lines) + "\n"
    t0 = time.perf_counter()
    res = run("/tmp/P1561", max_inp)
    dt = time.perf_counter() - t0
    print(f"PASS P1561! 300 random cases + hand cases + n=25000 worst-case (ans={res}) in {dt:.4f}s")

if __name__ == "__main__":
    main()
