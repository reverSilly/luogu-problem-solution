import os, random, subprocess, sys, time

os.chdir(os.path.dirname(os.path.abspath(__file__)))  # 固定在本目录跑,相对路径才稳

STD, BRUTE = "P1583", "P1583_brute"

def compile_all():
    subprocess.run(["g++", "-O2", "-std=c++17", f"{STD}.cpp", "-o", STD], check=True)
    subprocess.run(["g++", "-O2", "-std=c++17", f"{BRUTE}.cpp", "-o", BRUTE], check=True)

def run(bin_name, inp):
    p = subprocess.run([f"./{bin_name}"], input=inp, text=True, capture_output=True, check=True)
    return p.stdout.split()          # 按 token 比,容忍行尾空格差异

def make_input(n, k, E, W):
    return f"{n} {k}\n{' '.join(map(str, E))}\n{' '.join(map(str, W))}\n"

def cmp_case(inp, tag):
    a, b = run(STD, inp), run(BRUTE, inp)
    if a != b:
        print(f"[{tag}] MISMATCH std!=brute")
        print("--- input ---")
        print(inp)
        print("--- std ---")
        print(" ".join(a))
        print("--- brute ---")
        print(" ".join(b))
        sys.exit(1)

def expect(inp, want, tag):
    got = run(STD, inp)
    if got != want.split():
        print(f"[{tag}] MISMATCH std!=expected")
        print("--- input ---")
        print(inp)
        print("--- std ---")
        print(" ".join(got))
        print("--- expected ---")
        print(want)
        sys.exit(1)
    cmp_case(inp, tag)               # 期望值与暴力再互相确认

def main():
    compile_all()
    rng = random.Random(20261020)

    # 1. 官方样例回归(洛谷原题页 输入#1/输出#1)
    sample_in = make_input(10, 10, list(range(1, 11)), [2, 4, 6, 8, 10, 12, 14, 16, 18, 20])
    expect(sample_in, "10 9 8 7 6 5 4 3 2 1", "sample")

    # 2. 随机对拍:小 n 保证 O(n^2) 暴力可跑;小数值域制造大量平局,大数值域检验 int
    for tc in range(3200):
        n = rng.randint(1, 60)
        k = rng.randint(1, n)
        wlim = rng.choice([2, 3, 5, 10, 50, 1000, 10**9])   # 题面:n<=20000,权值为 int 范围正整数
        elim = rng.choice([2, 3, 5, 10, 50, 1000, 10**9])
        E = [rng.randint(1, elim) for _ in range(10)]
        W = [rng.randint(1, wlim) for _ in range(n)]
        cmp_case(make_input(n, k, E, W), f"random#{tc}")

    # 3. 定向边界用例:每条对应题解里的一个坑点,期望输出均经手工核算
    ones = [1] * 10
    edges = [
        # (n, k, E, W, 期望输出, 覆盖的坑点)
        (1, 1, ones, [1], "1", "最小规模"),
        (9, 9, list(range(1, 11)), [9, 8, 7, 6, 5, 4, 3, 2, 1], "1 2 3 4 5 6 7 8 9", "最终权值全平局"),
        (10, 1, list(range(1, 10)) + [1000], [2, 4, 6, 8, 10, 12, 14, 16, 18, 20], "1", "D%10 差一(D=10 应为类 10)"),
        (11, 11, [5, 100, 1, 1, 1, 1, 1, 1, 1, 1], list(range(11, 0, -1)), "2 1 3 4 5 6 7 11 8 9 10", "类号按名次模 10 循环而非连续段"),
        (2, 2, [1, 2, 1, 1, 1, 1, 1, 1, 1, 1], [1, 2], "1 2", "第二次排序不可 stable_sort 复用旧序"),
        (12, 12, [10, 1, 1, 1, 1, 1, 1, 1, 1, 1], [5] * 12, "1 11 2 3 4 5 6 7 8 9 10 12", "第一次排序的平局规则决定类号"),
        (20, 20, ones, list(range(1, 21)), "20 19 18 17 16 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1", "D=20 类号边界"),
        (21, 21, ones, list(range(1, 22)), "21 20 19 18 17 16 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1", "D=21 类号回到 1"),
        (3, 2, ones, [5, 5, 1], "1 2", "k 边界平局取编号小者"),
        (3, 3, [10**9] * 10, [10**9] * 3, "1 2 3", "大权值(W+E=2e9 不溢 int)"),
        (11, 10, [100, 1, 1, 1, 1, 1, 1, 1, 1, 1], list(range(11, 0, -1)), "1 11 2 3 4 5 6 7 8 9", "初始前 k 名不是答案(跨类反超)"),
    ]
    for i, (n, k, E, W, want, why) in enumerate(edges):
        expect(make_input(n, k, E, W), want, f"edge#{i}({why})")

    # 4. 中规模随机(n 到 400,O(n^2) 暴力仍可接受)
    for tc in range(200):
        n = rng.randint(100, 400)
        k = rng.randint(1, n)
        wlim = rng.choice([5, 50, 10**6])
        E = [rng.randint(1, rng.choice([5, 50, 10**6])) for _ in range(10)]
        W = [rng.randint(1, wlim) for _ in range(n)]
        cmp_case(make_input(n, k, E, W), f"medium#{tc}")

    # 5. 最坏规模计时:只跑标程(暴力 O(n^2) 在此规模无意义),期望序用 Python 参考实现独立复算
    n = 20000
    W = [rng.randint(1, 10**9) for _ in range(n)]
    E = [rng.randint(1, 10**9) for _ in range(10)]
    t0 = time.perf_counter()
    res = run(STD, make_input(n, n, E, W))
    dt = time.perf_counter() - t0
    assert len(res) == n, f"worst-case output count {len(res)} != {n}"
    r1 = sorted(range(1, n + 1), key=lambda i: (-W[i - 1], i))     # 名次:权值降序,编号升序
    F = [0] * (n + 1)
    for rk, i in enumerate(r1):                                    # rk=0 → 名次 D=rk+1 → 类号 rk%10+1
        F[i] = W[i - 1] + E[rk % 10]                              # Python 侧 E 为 0 基:类号 rk%10+1 → E[rk%10]
    want = [str(i) for i in sorted(range(1, n + 1), key=lambda i: (-F[i], i))]
    assert res == want, "worst-case random mismatch vs python reference"

    W2 = [7] * n                              # 权值全同:比较器每次落到第二关键字
    t1 = time.perf_counter()
    res2 = run(STD, make_input(n, n, E, W2))
    dt2 = time.perf_counter() - t1
    want2 = [str(i) for i in sorted(range(1, n + 1), key=lambda i: (-(7 + E[(i - 1) % 10]), i))]
    assert res2 == want2, "all-equal case mismatch vs python reference"

    print(f"P1583 check passed! sample OK; 3200 random + 200 medium + {len(edges)} edge cases all std==brute; "
          f"n=k=20000 random {dt:.4f}s, all-equal {dt2:.4f}s")

if __name__ == "__main__":
    main()
