import random, subprocess, sys
from functools import lru_cache

def pascal_ok(x):
    return '7' not in str(x)

def count_dp(N):
    # 独立的数位 DP: [1,N] 中十进制不含 7 的整数个数
    s = str(N)
    L = len(s)
    @lru_cache(None)
    def dp(i, tight):
        if i == L:
            return 1
        lim = int(s[i]) if tight else 9
        return sum(dp(i + 1, tight and d == lim) for d in range(lim + 1) if d != 7)
    return dp(0, True) - 1

# 1) 小范围: 与枚举暴搜对拍
pool = [x for x in range(1, 300001) if pascal_ok(x)]
random.seed(7)
for it in range(200):
    t = random.randint(1, 10)
    ns = [random.choice(pool) for _ in range(t)]
    if it == 0:
        ns = [1, 6, 8, 9, 10, 66, 68, 69]
    data = f"{len(ns)}\n" + "\n".join(map(str, ns)) + "\n"
    a = subprocess.run(["/tmp/P1590"], input=data, capture_output=True, text=True).stdout
    b = subprocess.run(["/tmp/P1590_brute"], input=data, capture_output=True, text=True).stdout
    if a.split() != b.split():
        print("MISMATCH vs brute", data, a, b)
        sys.exit(1)

# 2) 大范围: 与数位 DP 对拍(输入必须自身是 Pascal 数)
bigs = [999999999, 1000000000, 3999999999, 4294966295, 2147483646, 4000000000]
bigs = [v for v in bigs if pascal_ok(v)]
while len(bigs) < 40:
    v = random.randint(1, 2 ** 32 - 1)
    if pascal_ok(v):
        bigs.append(v)
data = f"{len(bigs)}\n" + "\n".join(map(str, bigs)) + "\n"
a = subprocess.run(["/tmp/P1590"], input=data, capture_output=True, text=True).stdout.split()
truth = [str(count_dp(v)) for v in bigs]
if a != truth:
    print("MISMATCH vs digit DP", bigs, a, truth)
    sys.exit(1)
print(f"ok: 200 random small groups + {len(bigs)} large cases")
