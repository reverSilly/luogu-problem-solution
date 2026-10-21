import random, subprocess, sys

def run(exe, data):
    p = subprocess.run([exe], input=data, capture_output=True, text=True)
    return p.stdout.strip()

def brute(t, m, items):
    # 独立的完全背包：逐件物品无限次松弛（i64 模拟）
    dp = [0] * (t + 1)
    for a, b in items:
        for j in range(a, t + 1):
            if dp[j - a] + b > dp[j]:
                dp[j] = dp[j - a] + b
    return dp[t]

random.seed(1616)
N = 200
for it in range(N):
    t = random.randint(1, 2000)
    m = random.randint(1, 10)
    items = [(random.randint(1, min(t, 500)), random.randint(1, 10000)) for _ in range(m)]
    data = f"{t} {m}\n" + "".join(f"{a} {b}\n" for a, b in items)
    a = run("/tmp/P1616", data)
    b = run("/tmp/P1616_brute", data)
    exp = str(brute(t, m, items))
    if a != b or a != exp:
        print("MISMATCH", repr(data), a, b, exp)
        sys.exit(1)
print(f"P1616 OK: {N} cases")
