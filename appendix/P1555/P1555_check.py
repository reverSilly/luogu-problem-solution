import subprocess, random, time

def compile_all():
    subprocess.run(["g++", "-O2", "-std=c++17", "P1555.cpp", "-o", "P1555"], check=True)
    subprocess.run(["g++", "-O2", "-std=c++17", "P1555_brute.cpp", "-o", "P1555_brute"], check=True)

def run(bin_name, inp):
    p = subprocess.run([f"./{bin_name}"], input=inp, text=True, capture_output=True, check=True)
    return p.stdout.strip()

def tob(v):
    return bin(v)[2:]

def tot(v):
    if v == 0:
        return "0"
    s = ""
    while v:
        s += str(v % 3)
        v //= 3
    return s[::-1]

# 与实现独立的 Python 参考(枚举所有候选值)
def all_solutions(b, t):
    Lb, Lt = len(b), len(t)
    res = []
    for v in range(1 << Lb):
        sb, st = tob(v), tot(v)
        if len(sb) != Lb or len(st) != Lt:
            continue
        if sum(x != y for x, y in zip(sb, b)) == 1 and sum(x != y for x, y in zip(st, t)) == 1:
            res.append(v)
    return res

def ref_enum(b, t):                       # 大数用: 枚举翻位(与标准解同思路, 独立重写)
    Lb, Lt = len(b), len(t)
    ans = 0
    for i in range(Lb):
        nb = b[:i] + ('1' if b[i] == '0' else '0') + b[i + 1:]
        v = int(nb, 2)
        sb, st = tob(v), tot(v)
        if len(sb) == Lb and len(st) == Lt and \
           sum(x != y for x, y in zip(sb, b)) == 1 and sum(x != y for x, y in zip(st, t)) == 1:
            ans = v
    return ans

def main():
    compile_all()
    rng = random.Random(20261018)

    # 1. 官方样例: 1010 / 212 -> 14(=1110_2 / 112_3)
    assert run("P1555", "1010\n212\n") == "14"
    assert run("P1555_brute", "1010\n212\n") == "14"

    # 2. 小规模随机: 唯一解且 std==brute==ref, 共 300 组
    made = 0
    tries = 0
    while made < 300 and tries < 20000:
        tries += 1
        Lb = rng.randint(1, 12)
        N = rng.randint(1, (1 << Lb) - 1)
        if len(tob(N)) != Lb:
            continue
        b = tob(N)
        t = tot(N)
        pos = rng.randrange(Lb)
        bl = list(b); bl[pos] = '1' if bl[pos] == '0' else '0'; bin_in = ''.join(bl)
        pos3 = rng.randrange(len(t))
        newd = rng.choice([d for d in "012" if d != t[pos3]])
        tl = list(t); tl[pos3] = newd; ter_in = ''.join(tl)
        sols = all_solutions(bin_in, ter_in)
        if sols != [N]:
            continue
        inp = f"{bin_in}\n{ter_in}\n"
        a = run("P1555", inp)
        c = run("P1555_brute", inp)
        assert a == c == str(N), f"inp={inp!r} std={a} brute={c} exp={N}"
        made += 1
    assert made == 300, f"only generated {made} unique cases"

    # 3. 大规模(接近 10^9, 二进制 30 位): std == 独立重写参考 == N
    big = 0
    for _ in range(60):
        N = rng.randint(2**28, 10**9)
        b = tob(N); t = tot(N)
        pos = rng.randrange(len(b))
        bl = list(b); bl[pos] = '1' if bl[pos] == '0' else '0'; bin_in = ''.join(bl)
        pos3 = rng.randrange(len(t))
        newd = rng.choice([d for d in "012" if d != t[pos3]])
        tl = list(t); tl[pos3] = newd; ter_in = ''.join(tl)
        inp = f"{bin_in}\n{ter_in}\n"
        a = run("P1555", inp)
        assert a == str(N) == str(ref_enum(bin_in, ter_in)), f"big inp={inp!r} std={a} exp={N}"
        big += 1

    # 4. 最坏规模计时(二进制 30 位)
    N = 10**9
    b = tob(N); t = tot(N)
    bl = list(b); bl[0] = '0'; bin_in = ''.join(bl)
    ter_in = '0' + t[1:]
    inp = f"{bin_in}\n{ter_in}\n"
    t0 = time.perf_counter()
    for _ in range(300):
        run("P1555", inp)
    dt = time.perf_counter() - t0

    print(f"P1555 check PASS! sample + 300 unique random + {big} big(30-bit) + worst timing "
          f"(300 runs) in {dt:.4f}s")

if __name__ == "__main__":
    main()
