import random, subprocess, sys

def gen(rnd):
    L = rnd.randint(1, 100)
    # 保证 A/B <= L
    B = rnd.randint(1, 1000)
    A = rnd.randint(1, max(1, B * L))
    return f"{A} {B} {L}\n"

def main():
    rnd = random.Random(int(sys.argv[1]) if len(sys.argv) > 1 else 20260101)
    rounds = int(sys.argv[2]) if len(sys.argv) > 2 else 2000
    bad = 0
    for _ in range(rounds):
        inp = gen(rnd)
        a = subprocess.run(["./P2118"], input=inp, capture_output=True, text=True).stdout.strip()
        b = subprocess.run(["./P2118_brute"], input=inp, capture_output=True, text=True).stdout.strip()
        if a != b:
            bad += 1
            print("MISMATCH", inp.strip(), "std:", a, "brute:", b)
            if bad >= 5:
                break
    print(f"{rounds} rounds, {bad} mismatches")

main()
