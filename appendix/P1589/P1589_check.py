import random, subprocess, sys

for it in range(200):
    L = random.randint(1, 5)
    n = random.randint(1, 6)
    segs = []
    for _ in range(n):
        s = random.randint(0, 24)
        e = random.randint(s + 1, min(30, s + 8))
        segs.append((s, e))
    data = f"{n} {L}\n" + "".join(f"{s} {e}\n" for s, e in segs)
    a = subprocess.run(["/tmp/P1589"], input=data, capture_output=True, text=True).stdout
    b = subprocess.run(["/tmp/P1589_brute"], input=data, capture_output=True, text=True).stdout
    if a.split() != b.split():
        print("MISMATCH", data, a, b)
        sys.exit(1)
print("all 200 cases ok")
