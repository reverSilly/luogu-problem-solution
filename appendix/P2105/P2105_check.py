import random, subprocess, sys

def gen(rnd):
    n = rnd.randint(1, 12)
    m = rnd.randint(1, 12)
    k = rnd.randint(1, min(6, n * m))
    lines = [f"{n} {m} {k}"]
    cells = set()
    for _ in range(k):
        while True:
            x = rnd.randint(1, n)
            y = rnd.randint(1, m)
            if (x, y) not in cells:
                cells.add((x, y))
                lines.append(f"{x} {y}")
                break
    return "\n".join(lines) + "\n"

def main():
    rnd = random.Random(int(sys.argv[1]) if len(sys.argv) > 1 else 20260101)
    rounds = int(sys.argv[2]) if len(sys.argv) > 2 else 2000
    bad = 0
    for _ in range(rounds):
        inp = gen(rnd)
        a = subprocess.run(["./P2105"], input=inp, capture_output=True, text=True).stdout.strip()
        b = subprocess.run(["./P2105_brute"], input=inp, capture_output=True, text=True).stdout.strip()
        if a != b:
            bad += 1
            print("MISMATCH\n" + inp)
            print("std:", a, "brute:", b)
            if bad >= 5:
                break
    print(f"{rounds} rounds, {bad} mismatches")

main()
