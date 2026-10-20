#!/usr/bin/env python3
# P1582《倒水》对拍脚本
# 流程:
#   [1] 编译标程与暴力(g++ -O2 -std=c++17)
#   [2] 官方样例回归(洛谷原题页 3 组)
#   [3] 引理独立复核:纯模拟的最小瓶数 == popcount(T),T=1..64
#   [4] 随机对拍:标程 vs C++ 真·模拟暴力(3200 组,N<=100)
#   [5] 穷举对拍:N=1..128 x K=1..40 全组合(5120 组)std vs brute
#   [6] 端到端:标程 vs Python 纯模拟(300 组,N<=24)
#   [7] 大规模交叉验证:标程 vs 两个独立参考实现(2000 组,N<=2e9),
#       答案较小时再叠加定义级线性扫描复核(300 组,N<=1e5)
#   [8] 边界用例(手算期望值 + 两个参考实现)
#   [9] 最坏规模计时
# 任何一组不一致:立即打印该组输入与双方输出,sys.exit(1)
import os, random, subprocess, sys, time

HERE = os.path.dirname(os.path.abspath(__file__))
os.chdir(HERE)

MAIN = "P1582"
BRUTE = "P1582_brute"
N_LIM = 2000000000


def compile_all():
    for src, out in ((f"{MAIN}.cpp", MAIN), (f"{BRUTE}.cpp", BRUTE)):
        subprocess.run(["g++", "-O2", "-std=c++17", src, "-o", out], check=True)


def run(bin_name, inp):
    p = subprocess.run([f"./{bin_name}"], input=inp, text=True, capture_output=True, timeout=60)
    if p.returncode != 0:
        print(f"[运行异常] {bin_name} 退出码 {p.returncode}")
        print(f"input : {inp}")
        print(f"stderr: {p.stderr}")
        sys.exit(1)
    return p.stdout.strip()


def fail(tag, inp, a, b):
    print(f"[不一致] {tag}")
    print(f"input : {inp}")
    print(f"std   : {a}")
    print(f"other : {b}")
    sys.exit(1)


def pc(x):
    return bin(x).count("1")


# ---------- 独立参考实现 1:候选枚举(引理 4) ----------
def ref_candidate(n, k):
    best = 0 if pc(n) <= k else None
    for i in range(32):
        if n >> i & 1:
            continue
        t = (n >> (i + 1) << (i + 1)) | (1 << i)
        if pc(t) <= k:
            d = t - n
            if best is None or d < best:
                best = d
    return best


# ---------- 独立参考实现 2:逢 1 进位 ----------
def ref_carry(n, k):
    t = n
    while pc(t) > k:
        t += t & (-t)
    return t - n


# ---------- 独立参考实现 3:定义级线性扫描 ----------
def ref_scan(n, k, cap):
    x = 0
    while True:
        if pc(n + x) <= k:
            return x
        x += 1
        if x > cap:
            return None


# ---------- 纯模拟:不借用 popcount,穷举合并顺序 ----------
SIM_L = 7           # 水位层数 2^0..2^6,支持 T<=127
_sim_memo = {}


def _sim_dfs(c):
    r = _sim_memo.get(c)
    if r is not None:
        return r
    ans = sum(c)
    for j in range(SIM_L - 1):
        if c[j] >= 2:
            d = list(c)
            d[j] -= 2
            d[j + 1] += 1
            v = _sim_dfs(tuple(d))
            if v < ans:
                ans = v
    _sim_memo[c] = ans
    return ans


def sim_mincount(T):
    assert 1 <= T <= 127, "sim 只支持小规模"
    c = [0] * SIM_L
    c[0] = T
    return _sim_dfs(tuple(c))


def sim_solve(n, k):
    x = 0
    while True:
        if sim_mincount(n + x) <= k:
            return x
        x += 1


def gen_n(rng):
    r = rng.random()
    if r < 0.45:
        return max(1, min(N_LIM, rng.randint(1, N_LIM)))
    if r < 0.75:
        return max(1, min(N_LIM, rng.getrandbits(rng.randint(1, 31))))
    a = rng.randint(0, 30)
    b = rng.randint(0, 30)
    n = (1 << a) + rng.choice((-2, -1, 0, 1, 2, 1 << b, -(1 << b)))
    return max(1, min(N_LIM, n))


def main():
    t_start = time.perf_counter()
    compile_all()
    rng = random.Random(20261020)

    # [1] 官方样例回归(取自洛谷原题页 https://www.luogu.com.cn/problem/P1582)
    samples = [("3 1", "1"), ("13 2", "3"), ("1000000 5", "15808")]
    for inp, exp in samples:
        got = run(MAIN, inp + "\n")
        if got != exp:
            fail(f"官方样例 {inp}", inp, got, exp)
    print(f"[1] 官方样例回归:{len(samples)}/{len(samples)} 通过")

    # [2] 引理独立复核:模拟最小瓶数 == popcount(T)
    for T in range(1, 65):
        s = sim_mincount(T)
        if s != pc(T):
            fail("引理2 反例", f"T={T}", s, pc(T))
    print("[2] 引理复核:sim_mincount(T)==popcount(T),T=1..64 全部通过")

    # [3] 随机对拍:标程 vs C++ 真·模拟暴力
    cnt = 0
    for _ in range(3200):
        n = rng.randint(1, 100)
        k = rng.randint(1, 32) if rng.random() < 0.7 else rng.randint(33, 1000)
        inp = f"{n} {k}\n"
        a = run(MAIN, inp)
        b = run(BRUTE, inp)
        if a != b:
            fail("随机对拍 std vs brute", inp, a, b)
        cnt += 1
    print(f"[3] 随机对拍:{cnt} 组(N<=100) std==brute 全部一致")

    # [4] 穷举对拍:N=1..128 x K=1..40 全组合
    cnt = 0
    for n in range(1, 129):
        for k in range(1, 41):
            inp = f"{n} {k}\n"
            a = run(MAIN, inp)
            b = run(BRUTE, inp)
            if a != b:
                fail("穷举对拍 std vs brute", inp, a, b)
            cnt += 1
    print(f"[4] 穷举对拍:{cnt} 组(N=1..128,K=1..40) std==brute 全部一致")

    # [5] 端到端:标程 vs Python 纯模拟(整条链零 popcount 假设)
    cnt = 0
    for _ in range(300):
        n = rng.randint(1, 24)
        k = rng.randint(1, 32)
        inp = f"{n} {k}\n"
        a = run(MAIN, inp)
        b = str(sim_solve(n, k))
        if a != b:
            fail("端到端 std vs sim", inp, a, b)
        cnt += 1
    print(f"[5] 端到端 sim 对拍:{cnt} 组(N<=24) 全部一致")

    # [6] 大规模交叉验证:标程 vs 两个独立参考实现
    cnt = 0
    for _ in range(2000):
        n = gen_n(rng)
        k = rng.randint(1, 32) if rng.random() < 0.7 else rng.randint(33, 1000)
        inp = f"{n} {k}\n"
        r1 = ref_candidate(n, k)
        r2 = ref_carry(n, k)
        if r1 != r2:
            fail("参考实现互斥", inp, r1, r2)
        a = run(MAIN, inp)
        if a != str(r1):
            fail("大规模 std vs ref", inp, a, r1)
        cnt += 1
    print(f"[6] 大规模交叉验证:{cnt} 组(N<=2e9) std==ref_candidate==ref_carry 全部一致")

    # [7] 定义级线性扫描叠加复核(答案较小才跑得动)
    cnt = 0
    for _ in range(300):
        n = rng.randint(1, 100000)
        k = rng.randint(1, 32)
        inp = f"{n} {k}\n"
        r = ref_scan(n, k, 200000)
        if r is None:
            continue
        a = run(MAIN, inp)
        if a != str(r):
            fail("线性扫描 std vs scan", inp, a, r)
        cnt += 1
    print(f"[7] 线性扫描复核:{cnt} 组(N<=1e5) std==scan 全部一致")

    # [8] 边界用例:手算期望值,并与两个参考实现互校
    edges = [
        ("1 1", "0"), ("1 1000", "0"), ("2 1", "0"), ("5 1", "3"),
        ("6 2", "0"), ("7 1", "1"), ("8 1", "0"), ("10 1", "6"),
        ("11 2", "1"), ("12 2", "0"), ("63 1", "1"), ("64 1", "0"),
        ("65 1", "63"), ("100 2", "28"), ("100 3", "0"),
        ("1023 1", "1"), ("1024 1", "0"), ("1025 1", "1023"), ("1025 2", "0"),
        ("3 1", "1"), ("13 2", "3"), ("1000000 5", "15808"),
        ("1073741823 1", "1"), ("1073741824 1", "0"), ("1073741825 1", "1073741823"),
        ("1999999999 1", "147483649"),
        ("2000000000 1", "147483648"), ("2000000000 2", "147483648"),
        ("2000000000 12", "3072"), ("2000000000 13", "0"), ("2000000000 1000", "0"),
    ]
    for inp, exp in edges:
        n, k = map(int, inp.split())
        r1 = ref_candidate(n, k)
        r2 = ref_carry(n, k)
        if str(r1) != exp or str(r2) != exp:
            fail("边界用例手算值有误", inp, f"candidate={r1},carry={r2}", exp)
        a = run(MAIN, inp + "\n")
        if a != exp:
            fail("边界用例 std", inp, a, exp)
    print(f"[8] 边界用例:{len(edges)}/{len(edges)} 通过(手算期望 + 两个参考实现互校)")

    # [9] 最坏规模计时
    reps = 50
    worst = "2000000000 1\n"
    ans_worst = run(MAIN, worst)
    t0 = time.perf_counter()
    for _ in range(reps):
        run(MAIN, worst)
    dt = (time.perf_counter() - t0) / reps
    for inp in ("2000000000 1000\n", "1073741825 1\n"):
        res = run(MAIN, inp)
        print(f"    附加计时 {inp.strip()} -> {res}")
    total = time.perf_counter() - t_start
    print(f"[9] 最坏规模计时:N=2000000000,K=1 -> ans={ans_worst},{reps} 次平均 {dt:.4f}s/次(含进程启动)")
    print(f"P1582 check passed! 总耗时 {total:.1f}s")


if __name__ == "__main__":
    main()
