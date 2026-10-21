import random, subprocess, sys

def run(exe, data):
    p = subprocess.run([exe], input=data, capture_output=True, text=True)
    return p.stdout.strip()

random.seed(1621)
N = 200
cases = [(10, 20, 3), (1, 1, 2), (1, 100, 2), (100, 300, 7), (1, 300, 300), (5, 5, 5)]
for _ in range(N):
    b = random.randint(1, 300)
    a = random.randint(1, b)
    if b < 2:
        b = 2
    p = random.randint(2, b)
    cases.append((a, b, p))

for a, b, p in cases:
    data = f"{a} {b} {p}\n"
    x = run("/tmp/P1621", data)
    y = run("/tmp/P1621_brute", data)
    if x != y:
        print("MISMATCH", data, "std=", x, "brute=", y)
        sys.exit(1)
print(f"P1621 OK: {len(cases)} cases")
