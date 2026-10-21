import subprocess, random, time

def compile_all():
    subprocess.run(["g++", "-O2", "-std=c++17", "P1557.cpp", "-o", "P1557"], check=True)
    subprocess.run(["g++", "-O2", "-std=c++17", "P1557_brute.cpp", "-o", "P1557_brute"], check=True)

def run(bin_name, inp):
    p = subprocess.run([f"./{bin_name}"], input=inp, text=True, capture_output=True, check=True)
    return p.stdout.strip()

def main():
    compile_all()
    # 1. 题面给出的全部 5 种等价表达式
    samples = [
        "23+++15-2",
        "23+(3)15-2",
        "+23+++15-2",
        "+23+(3)15-2",
        "+(1)23+(3)15-(1)2",
    ]
    for s in samples:
        assert run("P1557", s + "\n") == "66"
        assert run("P1557_brute", s + "\n") == "66"

    # 2. 随机算式对拍 500 组 (与 C++ __int128 及 Python 大整数双重核对)
    rng = random.Random(20261021)
    for _ in range(500):
        terms = rng.randint(1, 20)
        expr = []
        ans_py = 0
        for idx in range(terms):
            sgn = 1 if (idx == 0 and rng.random() < 0.3) else rng.choice([1, -1])
            mult = 1 if (idx == 0 and sgn == 1 and rng.random() < 0.5) else rng.randint(1, 500000)
            val = rng.randint(0, 10**12)
            ans_py += sgn * mult * val
            ch = "+" if sgn == 1 else "-"
            if idx == 0 and sgn == 1 and mult == 1 and rng.random() < 0.5:
                expr.append(f"{val}")
            elif mult <= 5 and rng.random() < 0.5:
                expr.append(ch * mult + f"{val}")
            else:
                expr.append(f"{ch}({mult}){val}")
        s = "".join(expr) + "\n"
        out_std = run("P1557", s)
        out_bf = run("P1557_brute", s)
        assert out_std == str(ans_py) and out_bf == str(ans_py)

    # 3. 长度 ~2000 的超大高精度极限测试
    big_k = "9" * 800
    big_v = "8" * 800
    big_s = f"+({big_k}){big_v}-({big_v}){big_k}+12345678901234567890\n"
    t0 = time.perf_counter()
    res = run("P1557", big_s)
    dt = time.perf_counter() - t0
    assert res == "12345678901234567890"
    print(f"P1557 check passed! 5 samples + 500 random expressions + 2000-char BigInt in {dt:.4f}s")

if __name__ == "__main__":
    main()
