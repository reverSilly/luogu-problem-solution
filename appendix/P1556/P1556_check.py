import subprocess, random, time
from itertools import permutations

def compile_all():
    subprocess.run(["g++", "-O2", "-std=c++17", "P1556.cpp", "-o", "P1556"], check=True)
    subprocess.run(["g++", "-O2", "-std=c++17", "P1556_brute.cpp", "-o", "P1556_brute"], check=True)

def run(bin_name, inp):
    p = subprocess.run([f"./{bin_name}"], input=inp, text=True, capture_output=True, check=True)
    return p.stdout.strip()

# 与实现独立的 Python 参考: 全排列 + 显式几何判定
def pref(cows):
    n = len(cows)
    cnt = 0
    for perm in permutations(range(n)):
        px, py = 0, 0
        inx, iny = 0, 0
        has = False
        ok = True
        for idx in perm:
            cx, cy = cows[idx]
            if px != cx and py != cy:
                ok = False; break
            ox = (cx > px) - (cx < px)
            oy = (cy > py) - (cy < py)
            if has and ox == inx and oy == iny:
                ok = False; break
            inx, iny = ox, oy
            has = True
            px, py = cx, cy
        if ok:
            if px != 0 and py != 0:
                ok = False
            else:
                ox = (0 > px) - (0 < px)
                oy = (0 > py) - (0 < py)
                if ox == inx and oy == iny:
                    ok = False
        if ok:
            cnt += 1
    return cnt

def inp_of(cows):
    return f"{len(cows)}\n" + "\n".join(f"{x} {y}" for x, y in cows) + "\n"

def main():
    compile_all()
    rng = random.Random(20261018)

    # 1. 官方样例: 4 头牛 (0,1)(2,1)(2,0)(2,-5) -> 2 (路线 1-2-4-3 与 3-4-2-1)
    sample = [(0, 1), (2, 1), (2, 0), (2, -5)]
    assert run("P1556", inp_of(sample)) == "2"
    assert run("P1556_brute", inp_of(sample)) == "2"
    assert pref(sample) == 2

    # 2. 手工边界
    manual = [
        [(0, 5)],                                   # 单牛且在原点同列 -> 1
        [(3, 5)],                                   # 单牛与原点不同行不同列 -> 0
        [(1, 0), (1, 1), (0, 1)],                   # 三牛直角回路 -> 2
        [(0, 1), (0, 2)],                           # 共线同侧 -> 0
        [(1, 0), (2, 0), (3, 0)],                   # 共线 -> 0
    ]
    for cows in manual:
        exp = pref(cows)
        a = run("P1556", inp_of(cows))
        b = run("P1556_brute", inp_of(cows))
        assert a == b == str(exp), f"cows={cows} std={a} brute={b} exp={exp}"

    # 3. 随机(小格点集, 易产生对齐): n<=6 三方一致
    pool = [(i, j) for i in range(4) for j in range(4) if (i, j) != (0, 0)]
    for _ in range(250):
        n = rng.randint(1, 6)
        cows = rng.sample(pool, n)
        exp = pref(cows)
        a = run("P1556", inp_of(cows))
        b = run("P1556_brute", inp_of(cows))
        assert a == b == str(exp), f"cows={cows} std={a} brute={b} exp={exp}"

    # 4. 随机(含负坐标): n<=6 三方一致, 另 n=7,8 只对拍 std==brute
    pool2 = [(i, j) for i in range(-2, 3) for j in range(-2, 3) if (i, j) != (0, 0)]
    for _ in range(150):
        n = rng.randint(1, 6)
        cows = rng.sample(pool2, n)
        exp = pref(cows)
        a = run("P1556", inp_of(cows))
        b = run("P1556_brute", inp_of(cows))
        assert a == b == str(exp), f"cows={cows} std={a} brute={b} exp={exp}"
    for _ in range(80):
        n = rng.randint(7, 8)
        cows = rng.sample(pool, n)
        a = run("P1556", inp_of(cows))
        b = run("P1556_brute", inp_of(cows))
        assert a == b, f"cows={cows} std={a} brute={b}"

    # 5. 最坏规模 n=10: 阶梯(两列) 与 共线, 计时
    ladder = [(1, y) for y in range(5)] + [(2, y) for y in range(5)]
    collinear = [(i, 0) for i in range(1, 11)]
    assert run("P1556", inp_of(ladder)) == run("P1556_brute", inp_of(ladder))
    assert run("P1556", inp_of(collinear)) == run("P1556_brute", inp_of(collinear))
    t0 = time.perf_counter()
    for _ in range(20):
        r1 = run("P1556", inp_of(ladder))
        r2 = run("P1556", inp_of(collinear))
    dt = time.perf_counter() - t0

    print(f"P1556 check PASS! sample + {len(manual)} manual + 400 random + n=8 + worst n=10 "
          f"(ladder ans={r1}, collinear ans={r2}, 20x2 runs in {dt:.4f}s)")

if __name__ == "__main__":
    main()
