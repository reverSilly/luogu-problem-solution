import random, subprocess, sys

# P2181 对角线
# 暴力：枚举所有对角线两两组合，按端点在圆周上是否交替来判定是否相交
def gen(rnd):
    n = rnd.randint(3, 14)
    return f"{n}\n"

def main():
    rnd = random.Random(int(sys.argv[1]) if len(sys.argv) > 1 else 20260722)
    rounds = int(sys.argv[2]) if len(sys.argv) > 2 else 300
    bad = 0
    for _ in range(rounds):
        inp = gen(rnd)
        a = subprocess.run(["./P2181"], input=inp, capture_output=True, text=True).stdout.strip()
        b = subprocess.run(["./P2181_brute"], input=inp, capture_output=True, text=True).stdout.strip()
        if a != b:
            bad += 1
            print("MISMATCH\n" + inp)
            print("std:", a, "brute:", b)
            if bad >= 5:
                break
    print(f"{rounds} rounds, {bad} mismatches")

main()
