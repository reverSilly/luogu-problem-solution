import random, subprocess, sys, os

DIR = os.path.dirname(os.path.abspath(__file__))
SOL = os.path.join(DIR, "P2038")
BRU = os.path.join(DIR, "P2038_brute")


def run(exe, data):
    p = subprocess.run([exe], input=data, capture_output=True, text=True, timeout=120)
    if p.returncode != 0:
        raise RuntimeError(f"{exe} failed rc={p.returncode} err={p.stderr[:300]}")
    return p.stdout.strip()


def main():
    random.seed(20380101)
    cases = 0
    for it in range(1500):
        d = random.randint(1, 20)
        n = random.randint(1, 20)
        pts = set()
        while len(pts) < n:
            pts.add((random.randint(0, 128), random.randint(0, 128)))
        pts = list(pts)
        lines = [f"{d}", f"{n}"]
        for x, y in pts:
            lines.append(f"{x} {y} {random.randint(1, 1000)}")
        data = "\n".join(lines) + "\n"
        x, y = run(SOL, data), run(BRU, data)
        if x != y:
            print("MISMATCH", data, repr(x), repr(y))
            sys.exit(1)
        cases += 1
    samp = "1\n2\n4 4 10\n6 6 20\n"
    assert run(SOL, samp) == "1 30", repr(run(SOL, samp))
    print(f"OK {cases} random cases + sample")


if __name__ == "__main__":
    main()
