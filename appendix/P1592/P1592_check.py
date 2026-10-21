import random, subprocess, sys

random.seed(13)
cases = [(1, 1), (1, 100000000), (2, 1), (2, 5), (12, 3)]
for _ in range(150):
    n = random.randint(1, 60)
    k = random.randint(1, 400)
    cases.append((n, k))
for _ in range(50):
    n = random.randint(999900, 1000000)
    k = random.randint(1, 300)
    cases.append((n, k))

for n, k in cases:
    data = f"{n} {k}\n"
    a = subprocess.run(["/tmp/P1592"], input=data, capture_output=True, text=True).stdout
    b = subprocess.run(["/tmp/P1592_brute"], input=data, capture_output=True, text=True).stdout
    if a.split() != b.split():
        print("MISMATCH", n, k, a, b)
        sys.exit(1)
print(f"all {len(cases)} cases ok")
