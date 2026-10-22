import random, subprocess, sys

# P2185 公路通行税
# 暴力：Floyd 求所有点对最短路，最大值即图的直径（公路条数），再乘 100
def gen(rnd):
    n = rnd.randint(1, 40)
    # 先随机生成一棵生成树保证连通，再随机加边
    edges = []
    for v in range(2, n + 1):
        u = rnd.randint(1, v - 1)
        edges.append((u, v))
    extra = rnd.randint(0, n)
    for _ in range(extra):
        u = rnd.randint(1, n)
        v = rnd.randint(1, n)
        edges.append((u, v))
    rnd.shuffle(edges)
    s = f"{n} {len(edges)}\n" + "".join(f"{u} {v}\n" for u, v in edges)
    return s

def main():
    rnd = random.Random(int(sys.argv[1]) if len(sys.argv) > 1 else 20260722)
    rounds = int(sys.argv[2]) if len(sys.argv) > 2 else 300
    bad = 0
    for _ in range(rounds):
        inp = gen(rnd)
        a = subprocess.run(["./P2185"], input=inp + "0 0\n", capture_output=True, text=True).stdout.strip()
        b = subprocess.run(["./P2185_brute"], input=inp + "0 0\n", capture_output=True, text=True).stdout.strip()
        if a != b:
            bad += 1
            print("MISMATCH\n" + inp)
            print("std:", a, "brute:", b)
            if bad >= 5:
                break
    print(f"{rounds} rounds, {bad} mismatches")

main()
