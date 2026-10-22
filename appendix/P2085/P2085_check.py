import random, subprocess, sys

def gen(rnd):
    n = rnd.randint(1, 5)
    m = rnd.randint(1, 8)
    A = [rnd.randint(1, 10) for _ in range(n)]
    B = [rnd.randint(0, 20) for _ in range(n)]
    C = [rnd.randint(0, 50) for _ in range(n)]
    lines = [f"{n} {m}"]
    for i in range(n):
        lines.append(f"{A[i]} {B[i]} {C[i]}")
    return "\n".join(lines) + "\n"

def main():
    rnd = random.Random(int(sys.argv[1]) if len(sys.argv) > 1 else 20260101)
    rounds = int(sys.argv[2]) if len(sys.argv) > 2 else 2000
    bad = 0
    for _ in range(rounds):
        inp = gen(rnd)
        a = subprocess.run(["./P2085"], input=inp, capture_output=True, text=True).stdout.split()
        b = subprocess.run(["./P2085_brute"], input=inp, capture_output=True, text=True).stdout.split()
        if a != b:
            bad += 1
            print("MISMATCH\n" + inp)
            print("std:", a)
            print("brute:", b)
            if bad >= 5:
                break
    print(f"{rounds} rounds, {bad} mismatches")

main()
