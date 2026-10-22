import random, subprocess, sys

# P2170 选学霸
# 暴力：枚举全部 2^n 选人方案，检查每一对"实力相当"的人是否同选或同不选，
#       统计所有合法方案的人数，取与 m 最接近者（差相同取较小）
def gen(rnd):
    n = rnd.randint(1, 12)
    m = rnd.randint(0, n)
    k = rnd.randint(0, min(12, n * (n - 1) // 2))
    pairs = []
    for _ in range(k):
        u = rnd.randint(1, n)
        v = rnd.randint(1, n)
        while v == u:
            v = rnd.randint(1, n)
        pairs.append((u, v))
    s = f"{n} {m} {k}\n" + "".join(f"{u} {v}\n" for u, v in pairs)
    return s

def main():
    rnd = random.Random(int(sys.argv[1]) if len(sys.argv) > 1 else 20260722)
    rounds = int(sys.argv[2]) if len(sys.argv) > 2 else 500
    bad = 0
    for _ in range(rounds):
        inp = gen(rnd)
        a = subprocess.run(["./P2170"], input=inp, capture_output=True, text=True).stdout.strip()
        b = subprocess.run(["./P2170_brute"], input=inp, capture_output=True, text=True).stdout.strip()
        if a != b:
            bad += 1
            print("MISMATCH\n" + inp)
            print("std:", a, "brute:", b)
            if bad >= 5:
                break
    print(f"{rounds} rounds, {bad} mismatches")

main()
