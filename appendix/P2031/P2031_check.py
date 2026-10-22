import random, subprocess, sys, os

DIR = os.path.dirname(os.path.abspath(__file__))
SOL = os.path.join(DIR, "P2031")
BRU = os.path.join(DIR, "P2031_brute")


def run(exe, data):
    p = subprocess.run([exe], input=data, capture_output=True, text=True, timeout=120)
    if p.returncode != 0:
        raise RuntimeError(f"{exe} failed rc={p.returncode} err={p.stderr[:300]}")
    return p.stdout.strip()


def main():
    random.seed(20310101)
    letters = "abc"
    cases = 0
    for it in range(2000):
        n = random.randint(1, 12)
        s = "".join(random.choice(letters) for _ in range(n))
        N = random.randint(1, 4)
        words = []
        for _ in range(N):
            m = random.randint(1, n)
            words.append("".join(random.choice(letters) for _ in range(m)))
        data = s + "\n" + str(N) + "\n" + "\n".join(words) + "\n"
        a, c = run(SOL, data), run(BRU, data)
        if a != c:
            print("MISMATCH", repr(data), "sol=", a, "brute=", c)
            sys.exit(1)
        cases += 1
    samp = "asdsd\n3\nas\nsd\nds\n"
    assert run(SOL, samp) == "2", run(SOL, samp)
    print(f"OK {cases} random cases + sample")


if __name__ == "__main__":
    main()
