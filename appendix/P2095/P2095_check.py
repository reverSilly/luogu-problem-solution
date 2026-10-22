import random, subprocess, sys

def gen(rnd):
    n = rnd.randint(1, 12)
    m = rnd.randint(1, 8)
    k = rnd.randint(1, 4)
    lines = [f"{n} {m} {k}"]
    lines.append(" ".join(str(rnd.randint(1, 3)) for _ in range(k)))
    for _ in range(n):
        lines.append(f"{rnd.randint(1, 20)} {rnd.randint(1, k)}")
    return "\n".join(lines) + "\n"

def main():
    rnd = random.Random(int(sys.argv[1]) if len(sys.argv) > 1 else 20260101)
    rounds = int(sys.argv[2]) if len(sys.argv) > 2 else 2000
    bad = 0
    for _ in range(rounds):
        inp = gen(rnd)
        a = subprocess.run(["./P2095"], input=inp, capture_output=True, text=True).stdout.strip()
        b = subprocess.run(["./P2095_brute"], input=inp, capture_output=True, text=True).stdout.strip()
        if a != b:
            bad += 1
            print("MISMATCH\n" + inp)
            print("std:", a, "brute:", b)
            if bad >= 5:
                break
    print(f"{rounds} rounds, {bad} mismatches")

main()
