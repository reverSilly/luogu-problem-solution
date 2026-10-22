import random, subprocess, sys

# P2190 小Z的车厢
# 暴力：逐条订票把人数直接累加到经过的每一段上，取最大段载客量后向上取整 /36
def gen(rnd):
    n = rnd.randint(2, 30)
    m = rnd.randint(1, 40)
    lines = []
    for _ in range(m):
        x = rnd.randint(1, n)
        y = rnd.randint(1, n)
        while y == x:
            y = rnd.randint(1, n)
        z = rnd.randint(1, 100)
        lines.append(f"{x} {y} {z}\n")
    return f"{n} {m}\n" + "".join(lines)

def main():
    rnd = random.Random(int(sys.argv[1]) if len(sys.argv) > 1 else 20260722)
    rounds = int(sys.argv[2]) if len(sys.argv) > 2 else 300
    bad = 0
    for _ in range(rounds):
        inp = gen(rnd)
        a = subprocess.run(["./P2190"], input=inp, capture_output=True, text=True).stdout.strip()
        b = subprocess.run(["./P2190_brute"], input=inp, capture_output=True, text=True).stdout.strip()
        if a != b:
            bad += 1
            print("MISMATCH\n" + inp)
            print("std:", a, "brute:", b)
            if bad >= 5:
                break
    print(f"{rounds} rounds, {bad} mismatches")

main()
