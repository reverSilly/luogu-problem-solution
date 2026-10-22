import random, subprocess, sys, os

DIR = os.path.dirname(os.path.abspath(__file__))
SOL = os.path.join(DIR, "P2037")
BRU = os.path.join(DIR, "P2037_brute")

LETTERS = "ABCDEFGHIJKLMNOPRSTUVWXY"  # 无 Q、Z


def run(exe, data):
    p = subprocess.run([exe], input=data, capture_output=True, text=True, timeout=60)
    if p.returncode != 0:
        raise RuntimeError(f"{exe} failed rc={p.returncode} err={p.stderr[:300]}")
    return p.stdout.strip()


def rand_num():
    digits = [random.choice("0123456789") for _ in range(7)]
    out = []
    for i, d in enumerate(digits):
        if random.random() < 0.4:
            out.append(random.choice(LETTERS))
        else:
            out.append(d)
        if random.random() < 0.3:
            out.append("-")
    return "".join(out)


def main():
    random.seed(20370101)
    cases = 0
    for it in range(1200):
        n = random.randint(1, 12)
        nums = [rand_num() for _ in range(n)]
        data = str(n) + "\n" + "\n".join(nums) + "\n"
        x, y = run(SOL, data), run(BRU, data)
        if x != y:
            print("MISMATCH", data, repr(x), repr(y))
            sys.exit(1)
        cases += 1
    samp = "3\nTUT-GLOP\n3-10-10-10\n310-1010\n"
    assert run(SOL, samp) == "310-1010 2", repr(run(SOL, samp))
    print(f"OK {cases} random cases + sample")


if __name__ == "__main__":
    main()
