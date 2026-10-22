import random, subprocess, sys

# P2180 摆石子
# 暴力：枚举网格点集的全部 2^(N*M) 个子集，取恰好 K 枚石子的那些，
#       逐个数出四角都有石子的轴平行矩形，取最大值
def gen(rnd):
    n = rnd.randint(1, 4)
    m = rnd.randint(1, 4)
    k = rnd.randint(0, n * m)
    return f"{n} {m} {k}\n"

def main():
    rnd = random.Random(int(sys.argv[1]) if len(sys.argv) > 1 else 20260722)
    rounds = int(sys.argv[2]) if len(sys.argv) > 2 else 300
    bad = 0
    for _ in range(rounds):
        inp = gen(rnd)
        a = subprocess.run(["./P2180"], input=inp, capture_output=True, text=True).stdout.strip()
        b = subprocess.run(["./P2180_brute"], input=inp, capture_output=True, text=True).stdout.strip()
        if a != b:
            bad += 1
            print("MISMATCH\n" + inp)
            print("std:", a, "brute:", b)
            if bad >= 5:
                break
    print(f"{rounds} rounds, {bad} mismatches")

main()
