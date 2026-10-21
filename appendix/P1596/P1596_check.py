import random, subprocess, sys

random.seed(17)

def run(bin, data):
    return subprocess.run([bin], input=data, capture_output=True, text=True).stdout.split()

fixed = ["1 1\nW\n", "1 1\n.\n", "3 3\nWWW\nWWW\nWWW\n", "2 2\nW.\n.W\n"]
cases = list(fixed)
for _ in range(200):
    n = random.randint(1, 20)
    m = random.randint(1, 20)
    p = random.choice([0.2, 0.4, 0.6, 0.8])
    rows = ["".join("W" if random.random() < p else "." for _ in range(m)) for _ in range(n)]
    cases.append(f"{n} {m}\n" + "\n".join(rows) + "\n")

for data in cases:
    a = run("/tmp/P1596", data)
    b = run("/tmp/P1596_brute", data)
    if a != b:
        print("MISMATCH", data, a, b)
        sys.exit(1)
print(f"all {len(cases)} cases ok")
