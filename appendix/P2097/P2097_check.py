import random, subprocess, sys

def gen(rnd):
    n = rnd.randint(0, 12)
    m = rnd.randint(0, 20)
    lines = [f"{n} {m}"]
    for _ in range(m):
        u = rnd.randint(1, n) if n > 0 else 1
        v = rnd.randint(1, n) if n > 0 else 1
        lines.append(f"{u} {v}")
    return "\n".join(lines) + "\n"

def main():
    rnd = random.Random(int(sys.argv[1]) if len(sys.argv) > 1 else 20260101)
    rounds = int(sys.argv[2]) if len(sys.argv) > 2 else 2000
    bad = 0
    for _ in range(rounds):
        inp = gen(rnd)
        a = subprocess.run(["./P2097"], input=inp, capture_output=True, text=True).stdout.strip()
        b = subprocess.run(["./P2097_brute"], input=inp, capture_output=True, text=True).stdout.strip()
        if a != b:
            bad += 1
            print("MISMATCH\n" + inp)
            print("std:", a, "brute:", b)
            if bad >= 5:
                break
    print(f"{rounds} rounds, {bad} mismatches")

main()
