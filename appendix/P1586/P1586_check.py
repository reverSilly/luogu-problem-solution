import os
import subprocess, random, time, sys

def compile_all():
    subprocess.run(["g++", "-O2", "-std=c++17", "P1586.cpp", "-o", "P1586"], check=True)
    subprocess.run(["g++", "-O2", "-std=c++17", "P1586_brute.cpp", "-o", "P1586_brute"], check=True)

def run(bin_name, inp):
    p = subprocess.run([f"./{bin_name}"], input=inp, text=True, capture_output=True, check=True)
    return p.stdout.strip()

def check_group(inp, tag):
    out_std = run("P1586", inp)
    out_bf = run("P1586_brute", inp)
    if out_std != out_bf:
        print(f"Mismatch on {tag}!\n--- input ---\n{inp}\n--- std ---\n{out_std}\n--- brute ---\n{out_bf}")
        sys.exit(1)

def main():
    os.chdir(os.path.dirname(os.path.abspath(__file__)))
    compile_all()
    rng = random.Random(20261020)

    # 1. 官方样例回归(洛谷 P1586):1 / 2003 -> 48
    inp = "1\n2003\n"
    out = run("P1586", inp)
    assert out == "48", f"sample regression failed: got {out!r}, expect '48'"
    check_group(inp, "sample")

    # 2. 边界批量:小值全枚举 + 完全平方数附近 + 值域端点 + 样例值
    base = list(range(1, 41)) + [2003, 32760, 32761, 32764, 32765, 32767, 32768]
    for k in range(1, 40):
        base += [k * k - 1, k * k, k * k + 1]
    base = [v for v in base if 1 <= v <= 32768]
    # 题面 t<=100,分批送检
    for i in range(0, len(base), 100):
        batch = base[i:i + 100]
        check_group(f"{len(batch)}\n" + "\n".join(map(str, batch)) + "\n", f"boundary batch {i // 100}")

    # 3. 随机对拍(小规模):t in [1,100], n in [1,2000],3000 组
    for tc in range(3000):
        t = rng.randint(1, 100)
        qs = [rng.randint(1, 2000) for _ in range(t)]
        inp = f"{t}\n" + "\n".join(map(str, qs)) + "\n"
        check_group(inp, f"random small #{tc}")

    # 4. 随机对拍(全值域):t in [1,100], n in [1,32768],100 组
    for tc in range(100):
        t = rng.randint(1, 100)
        qs = [rng.randint(1, 32768) for _ in range(t)]
        inp = f"{t}\n" + "\n".join(map(str, qs)) + "\n"
        check_group(inp, f"random full-range #{tc}")

    # 5. 最坏规模计时:t=100,全部 n=32768;同时记录全值域最大答案(供 int 安全性佐证)
    inp = "100\n" + "\n".join(["32768"] * 100) + "\n"
    t0 = time.perf_counter()
    res = run("P1586", inp)
    dt = time.perf_counter() - t0
    assert res.splitlines() == ["1"] * 100, "Lagrange sanity check failed: ans(32768) != 1"
    mx = 0
    for i in range(1, 32769, 100):
        batch = list(range(i, min(i + 100, 32769)))
        out = run("P1586", f"{len(batch)}\n" + "\n".join(map(str, batch)) + "\n")
        mx = max(mx, max(map(int, out.split())))
    print(f"P1586 check passed! sample 2003->48; boundary+3100 random groups all std==brute; "
          f"t=100,n=32768 worst input in {dt:.4f}s; max answer over n in [1,32768] = {mx}")

if __name__ == "__main__":
    main()
