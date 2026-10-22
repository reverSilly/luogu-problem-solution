import random, subprocess, sys, os

DIR = os.path.dirname(os.path.abspath(__file__))
SOL = os.path.join(DIR, "P2029")
BRU = os.path.join(DIR, "P2029_brute")


def run(exe, data):
    p = subprocess.run([exe], input=data, capture_output=True, text=True, timeout=60)
    if p.returncode != 0:
        raise RuntimeError(f"{exe} failed rc={p.returncode} err={p.stderr[:300]}")
    return p.stdout.strip()


def main():
    random.seed(20291001)
    cases = 0
    for it in range(3000):
        n = random.randint(1, 12)
        t = random.randint(0, 6)
        s = [random.randint(0, 10000) for _ in range(n)]
        b = [random.randint(0, 10000) for _ in range(n)]
        data = f"{n} {t}\n" + " ".join(map(str, s)) + "\n" + " ".join(map(str, b)) + "\n"
        a, c = run(SOL, data), run(BRU, data)
        if a != c:
            print("MISMATCH", data, "sol=", a, "brute=", c)
            sys.exit(1)
        cases += 1
    # 样例
    samp = "6 3\n1 2 3 4 5 6\n1 1 1 20 1 1\n"
    assert run(SOL, samp) == "39", run(SOL, samp)
    print(f"OK {cases} random cases + sample")


if __name__ == "__main__":
    main()
