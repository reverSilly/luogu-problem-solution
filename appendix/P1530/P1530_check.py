import subprocess, random, time, os

TMP = "/tmp/p1530_check"
os.makedirs(TMP, exist_ok=True)
STD = os.path.join(TMP, "std")
BF = os.path.join(TMP, "bf")

def compile_all():
    subprocess.run(["g++", "-O2", "-std=c++17", "P1530.cpp", "-o", STD], check=True)
    subprocess.run(["g++", "-O2", "-std=c++17", "P1530_brute.cpp", "-o", BF], check=True)

def run(bin_name, inp):
    p = subprocess.run([bin_name], input=inp, text=True, capture_output=True, check=True)
    return p.stdout.strip()

EXPECT = {
    "1 3": "0.(3)",
    "41 333": "0.(123)",
    "2 1": "2.0",
    "45 56": "0.803(571428)",
    "1 2": "0.5",
    "22 7": "3.(142857)",
    "1 4": "0.25",
    "4 2": "2.0",
    "1 1": "1.0",
    "6 3": "2.0",
    "1 6": "0.1(6)",
    "1 7": "0.(142857)",
}

def main():
    compile_all()
    rng = random.Random(20261018)
    # 1. 随机对拍(暴力 D 限在几百以内以保证 O(len^2) 扫描可跑)
    ds_pool = [2, 3, 6, 7, 11, 13, 17, 19, 23, 29, 97, 101, 109, 997]
    for tc in range(300):
        if tc % 4 == 0:
            d = rng.choice(ds_pool)          # 含长循环节(>76 字符)以触发换行
        else:
            d = rng.randint(2, 300)
        n = rng.randint(1, 100000)
        inp = f"{n} {d}\n"
        o1 = run(STD, inp)
        o2 = run(BF, inp)
        assert o1 == o2, f"Mismatch:\n{inp}std={o1}, bf={o2}"

    # 2. 手工边界/样例断言
    for inp, exp in EXPECT.items():
        got = run(STD, inp)
        assert got == exp, f"Edge fail [{inp}]: std={got}, exp={exp}"

    # 3. 最坏规模计时:分母取大素数,循环节可达 ~1e5 位
    t0 = time.perf_counter()
    res = run(STD, "1 99991\n")
    dt = time.perf_counter() - t0
    print(f"P1530 check PASS! 300 random cases matched brute; 1/99991 -> {len(res)} chars in {dt:.4f}s")

if __name__ == "__main__":
    main()
