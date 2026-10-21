import random, subprocess, sys

def run(exe, data):
    p = subprocess.run([exe], input=data, capture_output=True, text=True)
    return p.stdout.strip()

random.seed(1622)
N = 200
cases = [(20, 3, [3, 6, 14]), (1, 1, [1]), (2, 2, [1, 2]), (5, 1, [3]), (30, 6, [2, 5, 9, 17, 23, 29])]
for _ in range(N):
    P = random.randint(1, 30)
    Q = random.randint(1, min(P, 6))
    lst = sorted(random.sample(range(1, P + 1), Q))
    cases.append((P, Q, lst))

for P, Q, lst in cases:
    data = f"{P} {Q}\n" + " ".join(map(str, lst)) + "\n"
    x = run("/tmp/P1622", data)
    y = run("/tmp/P1622_brute", data)
    if x != y:
        print("MISMATCH", data, "std=", x, "brute=", y)
        sys.exit(1)
print(f"P1622 OK: {len(cases)} cases")
