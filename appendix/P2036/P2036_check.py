import random, subprocess, sys, os

DIR = os.path.dirname(os.path.abspath(__file__))
SOL = os.path.join(DIR, "P2036")
BRU = os.path.join(DIR, "P2036_brute")


def run(exe, data):
    p = subprocess.run([exe], input=data, capture_output=True, text=True, timeout=60)
    if p.returncode != 0:
        raise RuntimeError(f"{exe} failed rc={p.returncode} err={p.stderr[:300]}")
    return p.stdout.strip()


def main():
    random.seed(20360101)
    cases = 0
    for it in range(3000):
        n = random.randint(1, 10)
        items = [(random.randint(1, 30), random.randint(1, 30)) for _ in range(n)]
        data = str(n) + "\n" + "\n".join(f"{a} {b}" for a, b in items) + "\n"
        x, y = run(SOL, data), run(BRU, data)
        if x != y:
            print("MISMATCH", data, x, y)
            sys.exit(1)
        cases += 1
    for samp, exp in [("1\n3 10\n", "7"), ("2\n3 8\n5 8\n", "1"),
                      ("4\n1 7\n2 6\n3 8\n4 9\n", "1")]:
        assert run(SOL, samp) == exp, (samp, run(SOL, samp))
    print(f"OK {cases} random cases + 3 samples")


if __name__ == "__main__":
    main()
