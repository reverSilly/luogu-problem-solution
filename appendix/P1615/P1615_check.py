import random, subprocess, sys

# P1615 对拍：正解公式 vs 逐秒模拟
def run(exe, data):
    p = subprocess.run([exe], input=data, capture_output=True, text=True)
    return p.stdout.strip()

def fmt(t, pad):
    h, m, s = t // 3600, t // 60 % 60, t % 60
    if pad:
        return f"{h:02d}:{m:02d}:{s:02d}"
    return f"{h}:{m}:{s}"

random.seed(1615)
N = 200
for it in range(N):
    t1 = random.randint(0, 24 * 3600 - 2)
    t2 = random.randint(t1 + 1, 24 * 3600 - 1)
    n = random.randint(1, 32767)
    pad = random.random() < 0.5
    data = f"{fmt(t1,pad)}\n{fmt(t2,pad)}\n{n}\n"
    a = run("/tmp/P1615", data)
    b = run("/tmp/P1615_brute", data)
    exp = str((t2 - t1) * n)
    if a != b or a != exp:
        print("MISMATCH", repr(data), a, b, exp)
        sys.exit(1)
print(f"P1615 OK: {N} cases")
