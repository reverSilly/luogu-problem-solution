import random, subprocess, sys

def run(exe, data):
    p = subprocess.run([exe], input=data, capture_output=True, text=True)
    return p.stdout.strip().split("\n")

random.seed(1620)
N = 300
lines = []
for _ in range(N):
    co = random.randint(0, 20)
    cx = random.randint(0, 20)
    mo = random.randint(0, 20)
    mx = random.randint(0, 20)
    lines.append(f"{co} {cx} {mo} {mx}")
for _ in range(150):
    if random.random() < 0.8:
        lines.append(f"{random.randint(0,60)} {random.randint(0,60)} {random.randint(0,60)} {random.randint(0,60)}")
    else:
        lines.append(f"{random.randint(0,60)} {random.randint(0,60)} {random.randint(0,8)} {random.randint(0,8)}")
data = "\n".join(lines) + "\n"
a = run("/tmp/P1620", data)
b = run("/tmp/P1620_brute", data)
if len(a) != len(b):
    print("line count", len(a), len(b))
    sys.exit(1)
for i, (x, y) in enumerate(zip(a, b)):
    if x != y:
        print("MISMATCH", lines[i], "std=", x, "brute=", y)
        sys.exit(1)
print(f"P1620 OK: {len(lines)} cases (300 组 0..20 + 150 组 0..60)")
