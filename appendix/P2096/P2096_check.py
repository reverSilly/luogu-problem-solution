import random, subprocess, sys

def gen(rnd):
    m = rnd.randint(1, 12)
    n = rnd.randint(1, 12)
    lines = [f"{m} {n}"]
    for _ in range(m):
        lines.append(" ".join(str(rnd.randint(-100, 100)) for _ in range(n)))
    return "\n".join(lines) + "\n"

def main():
    rnd = random.Random(int(sys.argv[1]) if len(sys.argv) > 1 else 20260101)
    rounds = int(sys.argv[2]) if len(sys.argv) > 2 else 2000
    bad = 0
    for _ in range(rounds):
        inp = gen(rnd)
        a = subprocess.run(["./P2096"], input=inp, capture_output=True, text=True).stdout.strip()
        b = subprocess.run(["./P2096_brute"], input=inp, capture_output=True, text=True).stdout.strip()
        if a != b:
            bad += 1
            print("MISMATCH\n" + inp)
            print("std:", a, "brute:", b)
            if bad >= 5:
                break
    print(f"{rounds} rounds, {bad} mismatches")

main()
