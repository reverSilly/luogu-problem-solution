import random, subprocess, sys

MAXV = 300

def gen(rnd):
    n = rnd.randint(1, 6)
    m = rnd.randint(1, 4)
    v = rnd.randint(1, 5)
    x = rnd.randint(1, n)
    y = rnd.randint(1, m)
    lines = [f"{n} {m} {v} {x} {y}"]
    for i in range(1, n + 1):
        row = []
        for j in range(1, m + 1):
            if i == x and j == y:
                row += [i, j]          # 同学提供自己的房间号
            else:
                row += [rnd.randint(1, n), rnd.randint(1, m)]
        lines.append(" ".join(map(str, row)))
    return "\n".join(lines) + "\n"

def main():
    rnd = random.Random(int(sys.argv[1]) if len(sys.argv) > 1 else 20260101)
    rounds = int(sys.argv[2]) if len(sys.argv) > 2 else 3000
    bad = 0
    for it in range(rounds):
        inp = gen(rnd)
        a = subprocess.run(["./P2083"], input=inp, capture_output=True, text=True).stdout.strip()
        b = subprocess.run(["./P2083_brute"], input=inp, capture_output=True, text=True).stdout.strip()
        if a != b:
            bad += 1
            print("MISMATCH")
            print(inp)
            print("std:", a)
            print("brute:", b)
            if bad >= 5:
                break
    print(f"{rounds} rounds, {bad} mismatches")

main()
