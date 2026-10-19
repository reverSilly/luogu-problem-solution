import subprocess, random, time

SD = "/tmp/P1517"
BF = "/tmp/P1517_brute"


def compile_all():
    subprocess.run(["g++", "-O2", "-std=c++17", "P1517.cpp", "-o", SD], check=True)
    subprocess.run(["g++", "-O2", "-std=c++17", "P1517_brute.cpp", "-o", BF], check=True)


def run(bin_name, inp):
    p = subprocess.run([bin_name], input=inp, text=True, capture_output=True, check=True)
    return p.stdout.strip().split("\n")


def oracle(R, n):
    # 独立实现（Python 原生大整数）：R^n 精确值 + 题目输出格式
    dot = R.find('.')
    if dot == -1:
        P = int(R)
        k = 0
    else:
        P = int(R.replace('.', ''))
        k = len(R) - dot - 1
    num = P ** n
    e = k * n
    s = str(num)
    if e == 0:
        return s
    if len(s) <= e:
        s = '0' * (e - len(s)) + s
    ip = s[:len(s) - e]
    fp = s[len(s) - e:]
    fp = fp.rstrip('0')
    ip = ip.lstrip('0')
    if ip == '':
        return '.' + fp if fp else '0'
    return ip + ('.' + fp if fp else '')


def rand_R(rng):
    ip = rng.randint(0, 9999)
    fd = rng.randint(0, 3)
    if ip == 9999 and fd >= 2:      # R <= 9999.9
        fd = 1
    if fd == 0:
        R = str(ip)
    else:
        frac = ''.join(rng.choice('0123456789') for _ in range(fd))
        R = f"{ip}.{frac}"
    if float(R) == 0.0 or float(R) > 9999.9:
        return rand_R(rng)
    return R


def main():
    compile_all()
    rng = random.Random(20261018)

    # 1. 手工边界用例(可手推)
    hand = [("2.5", 2, "6.25"), ("0.5", 2, ".25"), ("1.0", 3, "1"),
            ("10", 2, "100"), ("0.01", 3, ".000001"), (".5", 1, ".5"),
            ("9999.9", 1, "9999.9")]
    inp = "".join(f"{r} {n}\n" for r, n, _ in hand)
    o1 = run(SD, inp)
    o2 = run(BF, inp)
    for (r, n, exp), a, b in zip(hand, o1, o2):
        assert a == exp, f"hand std {r}^{n}: got {a} expect {exp}"
        assert b == exp, f"hand brute {r}^{n}: got {b} expect {exp}"
    print("handcrafted edge cases:", " ".join(exp for _, _, exp in hand), "OK")

    # 2. 随机对拍(多组数据一次性喂入, 检验 EOF 循环)
    cases = []
    for _ in range(400):
        R = rand_R(rng)
        n = rng.randint(1, 250)
        cases.append((R, n))
    inp = "".join(f"{R} {n}\n" for R, n in cases)
    o1 = run(SD, inp)
    o2 = run(BF, inp)
    oo = [oracle(R, n) for R, n in cases]
    assert len(o1) == len(cases), f"std output count {len(o1)} != {len(cases)}"
    for (R, n), a, b, c in zip(cases, o1, o2, oo):
        assert a == b == c, f"Mismatch {R}^{n}: std={a} brute={b} oracle={c}"
    print(f"random cases: {len(cases)} std==brute==python-oracle OK")

    # 3. 最坏规模计时: R=9999.9, n=250 (答案约 1250 位)
    inp = "9999.9 250\n"
    t0 = time.perf_counter()
    res = run(SD, inp)
    dt = time.perf_counter() - t0
    assert res[0] == oracle("9999.9", 250), "worst-case std != oracle"
    assert run(BF, inp)[0] == res[0], "worst-case brute != std"
    print(f"P1517 check passed! 400 random + 7 edge cases (worst n=250, digits={len(res[0])}) in {dt:.4f}s")


if __name__ == "__main__":
    main()
