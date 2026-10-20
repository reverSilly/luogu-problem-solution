import os
import subprocess, random, sys, time

def compile_all():
    subprocess.run(["g++", "-O2", "-std=c++17", "P1577.cpp", "-o", "P1577"], check=True)
    subprocess.run(["g++", "-O2", "-std=c++17", "P1577_brute.cpp", "-o", "P1577_brute"], check=True)

def run(bin_name, inp):
    p = subprocess.run([f"./{bin_name}"], input=inp, text=True, capture_output=True, check=True)
    return p.stdout.strip()

def pyref(inp):
    # 独立的 Python 参考实现:字符串精确解析 -> 整数分 -> 整数二分,取最大可行分
    tok = inp.split()
    n, k = int(tok[0]), int(tok[1])
    cents = []
    for s in tok[2:2 + n]:
        if '.' in s:
            ip, fp = s.split('.')
            cents.append(int(ip) * 100 + int((fp + "00")[:2]))
        else:
            cents.append(int(s) * 100)
    lo, hi, ans = 1, max(cents), 0
    while lo <= hi:
        mid = (lo + hi) // 2
        if sum(c // mid for c in cents) >= k:
            ans, lo = mid, mid + 1
        else:
            hi = mid - 1
    return f"{ans // 100}.{ans % 100:02d}"

def fail(tag, inp, outs):
    print(f"[{tag}] MISMATCH")
    print("---- input ----")
    print(inp)
    for name, out in outs:
        print(f"---- {name} ----")
        print(repr(out))
    sys.exit(1)

def check3(inp, tag):
    # 三方对拍:标程 vs 精确有理数暴力 vs Python 参考
    o1 = run("P1577", inp)
    o2 = run("P1577_brute", inp)
    o3 = pyref(inp)
    if not (o1 == o2 == o3):
        fail(tag, inp, [("std", o1), ("brute", o2), ("pyref", o3)])

def check_py(inp, tag):
    # 大规模用例:暴力 O(n*k*n) 太慢,只与 Python 参考对拍
    o1 = run("P1577", inp)
    o3 = pyref(inp)
    if o1 != o3:
        fail(tag, inp, [("std", o1), ("pyref", o3)])

def mk(n, k, lens):
    return f"{n} {k}\n" + "\n".join(lens) + "\n"

def main():
    os.chdir(os.path.dirname(os.path.abspath(__file__)))
    compile_all()
    rng = random.Random(20261020)
    total = 0

    # 0. 官方样例回归(https://www.luogu.com.cn/problem/P1577)
    sample = "4 11\n8.02\n7.43\n4.57\n5.39\n"
    o = run("P1577", sample)
    assert o == "2.00", f"sample mismatch: std={o!r}, expected 2.00"
    check3(sample, "sample")
    print("sample ok: '4 11 / 8.02 7.43 4.57 5.39' -> 2.00 (std==brute==pyref)")

    # 1. 随机对拍 A:小长度密档(n<=8,k<=30,L<=5.00)
    for tc in range(3000):
        n = rng.randint(1, 8)
        k = rng.randint(1, 30)
        lens = [f"{rng.uniform(0.01, 5.00):.2f}" for _ in range(n)]
        check3(mk(n, k, lens), f"randA#{tc}")
        total += 1
    print(f"randA ok: 3000 cases (n<=8, k<=30, L in [0.01,5.00]) std==brute==pyref")

    # 2. 随机对拍 B:中长度档(n<=20,k<=40,L<=100.00)
    for tc in range(500):
        n = rng.randint(1, 20)
        k = rng.randint(1, 40)
        lens = [f"{rng.uniform(0.01, 100.00):.2f}" for _ in range(n)]
        check3(mk(n, k, lens), f"randB#{tc}")
        total += 1
    print(f"randB ok: 500 cases (n<=20, k<=40, L in [0.01,100.00]) std==brute==pyref")

    # 3. 随机对拍 C:全值域档(n<=6,k<=50,L<=100000.00,暴力耗时只与 n*k*n 有关)
    for tc in range(200):
        n = rng.randint(1, 6)
        k = rng.randint(1, 50)
        lens = [f"{rng.uniform(0.01, 100000.00):.2f}" for _ in range(n)]
        check3(mk(n, k, lens), f"randC#{tc}")
        total += 1
    print(f"randC ok: 200 cases (n<=6, k<=50, L in [0.01,100000.00]) std==brute==pyref")

    # 4. 随机对拍 D:极短绳档(L<=0.10),大量制造 0.00 / 0.01 边界答案
    for tc in range(300):
        n = rng.randint(1, 8)
        k = rng.randint(1, 30)
        lens = [f"{rng.uniform(0.01, 0.10):.2f}" for _ in range(n)]
        check3(mk(n, k, lens), f"randD#{tc}")
        total += 1
    print(f"randD ok: 300 cases (n<=8, k<=30, L in [0.01,0.10]) std==brute==pyref")

    # 5. 边界用例(含手工核算的期望值,全部过三方)
    cases = [
        ("n=1,k=1,L=0.01",            "1 1\n0.01\n",                 "0.01"),
        ("n=1,k=2,L=0.01",            "1 2\n0.01\n",                 "0.00"),
        ("n=1,k=10000,L=100000.00",   "1 10000\n100000.00\n",        "10.00"),
        ("n=2,k=1,L=100000.00,0.01",  "2 1\n100000.00\n0.01\n",      "100000.00"),
        ("n=1,k=9999,L=99.98",        "1 9999\n99.98\n",             "0.00"),
        ("n=1,k=9999,L=99.99",       "1 9999\n99.99\n",             "0.01"),  # x*=9999*0.01,恰压边界
        ("n=1,k=9999,L=100.00",       "1 9999\n100.00\n",            "0.01"),
        ("n=2,k=3,L=1.00,1.00",       "2 3\n1.00\n1.00\n",           "0.50"),
        ("n=1,k=3,L=2.00",          "1 3\n2.00\n",                "0.66"),  # x*=2/3,截断
        ("n=2,k=4,L=1.00,1.00",       "2 4\n1.00\n1.00\n",           "0.50"),
        ("n=3,k=3,L=1.00,1.00,1.00",  "3 3\n1.00\n1.00\n1.00\n",     "1.00"),
        ("n=2,k=2,L=2.00,2.00",       "2 2\n2.00\n2.00\n",           "2.00"),  # x* 恰为整数分
        ("n=1,k=1,L=100000.00",       "1 1\n100000.00\n",            "100000.00"),
    ]
    for tag, inp, exp in cases:
        check3(inp, tag)
        assert run("P1577", inp) == exp, f"{tag}: expect {exp}, got {run('P1577', inp)!r}"
        total += 1
    print(f"boundary ok: {len(cases)} hand-checked cases (0.00 / 0.01 / integer-cent edges)")

    # 6. 大规模交叉验证(暴力太慢,仅对 Python 参考)+ 最坏规模计时
    big = [
        ("n=10000,k=1,all max",   "10000 1\n" + "100000.00\n" * 10000,  "100000.00"),
        ("n=10000,k=10000,all max","10000 10000\n" + "100000.00\n" * 10000, "100000.00"),
    ]
    for tag, inp, exp in big:
        check_py(inp, tag)
        assert run("P1577", inp) == exp, f"{tag}: expect {exp}"
        total += 1
    print("big-scale ok: n=10000 k=1 / k=10000 all-max vs pyref")

    worst = 0.0
    for tag, n, k in [("n=1e4,k=1e4,all max", 10000, 10000),
                      ("n=1e4,k=9999,rand", 10000, 9999),
                      ("n=1e4,k=1,rand", 10000, 1),
                      ("n=1e4,k=5000,rand", 10000, 5000)]:
        if "all max" in tag:
            lens = ["100000.00"] * n
        else:
            lens = [f"{rng.uniform(0.01, 100000.00):.2f}" for _ in range(n)]
        inp = mk(n, k, lens)
        check_py(inp, tag)
        t0 = time.perf_counter()
        res = run("P1577", inp)
        dt = time.perf_counter() - t0
        worst = max(worst, dt)
        print(f"timing {tag}: ans={res} in {dt:.4f}s (vs pyref ok)")

    print(f"P1577 check passed! {total} random+boundary cases all std==brute(==pyref); "
          f"worst-scale (n=k=1e4, L=1e5.00) ran in {worst:.4f}s")

if __name__ == "__main__":
    main()
