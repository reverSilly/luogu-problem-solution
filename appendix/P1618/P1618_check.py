import random, subprocess, sys

def run(exe, data):
    p = subprocess.run([exe], input=data, capture_output=True, text=True)
    return p.stdout.strip()

random.seed(1618)
N = 200
cases = [(1, 2, 3), (123, 456, 789), (0, 1, 2), (2, 4, 6), (3, 4, 5), (1, 3, 5)]
for _ in range(N):
    A = random.randint(0, 900)
    B = random.randint(A + 1, 950)
    C = random.randint(B + 1, 999)
    cases.append((A, B, C))

for A, B, C in cases:
    data = f"{A} {B} {C}\n"
    a = run("/tmp/P1618", data)
    b = run("/tmp/P1618_brute", data)
    if a != b:
        print("MISMATCH", data, "\n--std--\n", a, "\n--brute--\n", b)
        sys.exit(1)
print(f"P1618 OK: {len(cases)} cases")
