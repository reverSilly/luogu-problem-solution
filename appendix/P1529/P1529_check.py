import subprocess, random, time, os

TMP = "/tmp/p1529_check"
os.makedirs(TMP, exist_ok=True)
STD = os.path.join(TMP, "std")
BF = os.path.join(TMP, "bf")

LAB = [chr(ord('a') + i) for i in range(26)] + [chr(ord('A') + i) for i in range(25)]
COW = [chr(ord('A') + i) for i in range(25)]   # 有牛的牧场 A..Y

def compile_all():
    subprocess.run(["g++", "-O2", "-std=c++17", "P1529.cpp", "-o", STD], check=True)
    subprocess.run(["g++", "-O2", "-std=c++17", "P1529_brute.cpp", "-o", BF], check=True)

def run(bin_name, inp):
    p = subprocess.run([bin_name], input=inp, text=True, capture_output=True, check=True)
    return p.stdout.strip()

def gen(rng):
    # 选 k 个互不相同牧场,必须含 Z 与至少一头牛,构随机连通图
    k = rng.randint(3, 8)
    pool = random.Random(rng.randint(0, 10**9))
    chosen = set(['Z'])
    chosen.add(rng.choice(COW))
    while len(chosen) < k:
        chosen.add(rng.choice(LAB))
    nodes = list(chosen)
    pool.shuffle(nodes)
    edges = []
    for i in range(1, len(nodes)):                 # 随机生成树,保证连通
        j = pool.randint(0, i - 1)
        edges.append((nodes[i], nodes[j], pool.randint(1, 1000)))
    extra = pool.randint(0, 4)                     # 额外重边
    for _ in range(extra):
        a = pool.choice(nodes); b = pool.choice(nodes)
        edges.append((a, b, pool.randint(1, 1000)))
    lines = [str(len(edges))]
    for a, b, w in edges:
        lines.append(f"{a} {b} {w}")
    return "\n".join(lines) + "\n"

def main():
    compile_all()
    rng = random.Random(20261018)
    for tc in range(300):
        inp = gen(rng)
        o1 = run(STD, inp)
        o2 = run(BF, inp)
        assert o1 == o2, f"Mismatch:\n{inp}std={o1}, bf={o2}"

    # 题面/USACO 经典样例: B 11
    sample = "5\nA d 6\nB d 3\nC e 9\nd Z 8\ne Z 3\n"
    assert run(STD, sample) == "B 11", f"sample got {run(STD, sample)}"

    # 最坏规模 P=10000 随机边
    pool = random.Random(1)
    lines = ["10000"]
    for _ in range(10000):
        lines.append(f"{pool.choice(LAB)} {pool.choice(LAB + ['Z'])} {pool.randint(1, 1000)}")
    big = "\n".join(lines) + "\n"
    t0 = time.perf_counter()
    res = run(STD, big)
    dt = time.perf_counter() - t0
    print(f"P1529 check PASS! 300 random cases matched brute; P=10000 worst-case -> {res} in {dt:.4f}s")

if __name__ == "__main__":
    main()
