import random, subprocess, sys, os

DIR = os.path.dirname(os.path.abspath(__file__))
SOL = os.path.join(DIR, "P2032")
BRU = os.path.join(DIR, "P2032_brute")


def run(exe, data):
    p = subprocess.run([exe], input=data, capture_output=True, text=True, timeout=120)
    if p.returncode != 0:
        raise RuntimeError(f"{exe} failed rc={p.returncode} err={p.stderr[:300]}")
    return p.stdout.strip()


def main():
    random.seed(20320101)
    cases = 0
    for it in range(1500):
        n = random.randint(1, 12)
        k = random.randint(1, n)
        a = [random.randint(1, 10000) for _ in range(n)]
        data = f"{n} {k}\n" + " ".join(map(str, a)) + "\n"
        x, y = run(SOL, data), run(BRU, data)
        if x != y:
            print("MISMATCH", data, x, y)
            sys.exit(1)
        cases += 1
    samp = "5 3\n1 5 3 4 2\n"
    assert run(SOL, samp) == "5\n5\n4", repr(run(SOL, samp))
    print(f"OK {cases} random cases + sample")


if __name__ == "__main__":
    main()
