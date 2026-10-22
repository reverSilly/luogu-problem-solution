import random, subprocess, sys

def gen(rnd):
    n = rnd.randint(1, 6)
    q = rnd.randint(1, 12)
    lines = [f"{n} {q}"]
    for _ in range(n):
        lines.append(" ".join(str(rnd.randint(0, 1)) for _ in range(n)))
    for _ in range(q):
        op = rnd.choice([1, 2, 3])
        if op == 3:
            lines.append("3")
        else:
            lines.append(f"{op} {rnd.randint(1, n)}")
    return "\n".join(lines) + "\n"

def main():
    rnd = random.Random(int(sys.argv[1]) if len(sys.argv) > 1 else 20260101)
    rounds = int(sys.argv[2]) if len(sys.argv) > 2 else 2000
    bad = 0
    for _ in range(rounds):
        inp = gen(rnd)
        a = subprocess.run(["./P2117"], input=inp, capture_output=True, text=True).stdout.strip()
        b = subprocess.run(["./P2117_brute"], input=inp, capture_output=True, text=True).stdout.strip()
        if a != b:
            bad += 1
            print("MATCH FAIL\n" + inp)
            print("std:", a, "brute:", b)
            if bad >= 5:
                break
    print(f"{rounds} rounds, {bad} mismatches")

main()
