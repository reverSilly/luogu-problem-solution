import subprocess, random, time

BASE = [0, 990, 1010, 1970, 2030, 2940, 3060, 3930, 4060, 4970, 5030, 5990, 6010, 7000]
SD = "/tmp/P1515"
BF = "/tmp/P1515_brute"


def compile_all():
    subprocess.run(["g++", "-O2", "-std=c++17", "P1515.cpp", "-o", SD], check=True)
    subprocess.run(["g++", "-O2", "-std=c++17", "P1515_brute.cpp", "-o", BF], check=True)


def run(bin_name, inp):
    p = subprocess.run([bin_name], input=inp, text=True, capture_output=True, check=True)
    return p.stdout.strip()


def main():
    compile_all()
    rng = random.Random(20261018)
    used0 = set(BASE)

    # 1. 随机对拍
    cases = 0
    for tc in range(400):
        N = rng.randint(0, 8)
        A = rng.randint(1, 1200)
        B = rng.randint(A, 2200)
        used = set(used0)
        pos = []
        while len(pos) < N:
            x = rng.randint(1, 6999)
            if x in used:
                continue
            used.add(x)
            pos.append(x)
        rng.shuffle(pos)
        inp = f"{A}\n{B}\n{N}\n" + "".join(f"{x}\n" for x in pos)
        o1 = run(SD, inp)
        o2 = run(BF, inp)
        assert o1 == o2, f"Mismatch:\n{inp}\nstd={o1} brute={o2}"
        cases += 1

    # 2. 手工边界用例(答案可手推)
    #    A=1,B=7000,N=0: 12 个中间旅馆任选 => 2^12 = 4096
    o = run(SD, "1\n7000\n0\n")
    assert o == "4096", f"handcase1 got {o}"
    #    A=7000,B=7000,N=0: 只能 0->7000 直达 => 1
    o = run(SD, "7000\n7000\n0\n")
    assert o == "1", f"handcase2 got {o}"
    #    A>B 无可行日 => 0
    o = run(SD, "1000\n1\n0\n")
    assert o == "0", f"handcase3 got {o}"
    print("handcrafted edge cases: 4096 / 1 / 0  OK")

    # 3. 最坏规模计时: N=20, A=1, B=7000 => 全部中间旅馆任选, 方案数巨大
    N = 20
    used = set(used0)
    pos = []
    while len(pos) < N:
        x = rng.randint(1, 6999)
        if x in used:
            continue
        used.add(x)
        pos.append(x)
    inp = f"1\n7000\n{N}\n" + "".join(f"{x}\n" for x in pos)
    t0 = time.perf_counter()
    res = run(SD, inp)
    dt = time.perf_counter() - t0
    print(f"P1515 check passed! {cases} random cases + 3 edge cases + worst case (ans={res}) in {dt:.4f}s")


if __name__ == "__main__":
    main()
